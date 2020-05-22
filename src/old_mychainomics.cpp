#include <mychainomics.hpp>
#include <token_functions.cpp>
#include <utility_functions.cpp>

namespace eosio {

ACTION mychainomics::adduser(const name& owner, const name& chainomics_id) {
  require_auth(owner);
  accounts accounts_t(get_self(), get_self().value);
  auto existing = accounts_t.find(chainomics_id.value);
  check(existing == accounts_t.end(), "Username is already registered.");
  accounts_t.emplace(owner, [&](auto& row) {
    row.chainomics_id = chainomics_id;
    row.owner = owner;
    row.level = 1;
    row.verified = false;
  });
};

  ACTION mychainomics::addtoken(const extended_asset extended_asset) {
    require_auth(get_self());
    tokens tokens_t(get_self(), get_self().value);
    tokens_t.emplace(get_self(), [&](auto &row) {
      row.token = extended_asset;
    });
  }

  ACTION mychainomics::internalxfer(const name& from_chainomic_account, const name& to_chainomic_account, asset& quantity, const std::string& memo){
    
    accounts accounts_t(get_self(), get_self().value);
    auto from_chainomic_account_itr = accounts_t.require_find(from_chainomic_account.value,"from_chainomic_account does not exist.");
    require_auth(from_chainomic_account_itr->owner);
    auto to_chainomic_account_itr = accounts_t.require_find(to_chainomic_account.value,"to_chainomic_account does not exist.");

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    tokens tokens_t(get_self(), get_self().value);
    auto valid_token = tokens_t.require_find(quantity.symbol.raw(), "Invalid token type.");

    check( quantity.symbol == valid_token->token.get_extended_symbol().get_symbol(), "symbol precision mismatch" );
    
    sub_account_token(accounts_t,from_chainomic_account_itr, quantity);
    add_account_token(accounts_t,to_chainomic_account_itr, quantity);
  }

  ACTION mychainomics::externalxfer(const name& from_chainomic_account, const name& to_chain_account, asset& quantity, const std::string& memo){
    accounts accounts_t(get_self(), get_self().value);
    auto from_chainomic_account_itr = accounts_t.require_find(from_chainomic_account.value,"from_chainomic_account does not exist.");
    require_auth(from_chainomic_account_itr->owner);
    check(is_account(to_chain_account),"to_chain_account does not exist.");

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    sub_account_token(accounts_t,from_chainomic_account_itr, quantity);

    tokens tokens_t(get_self(), get_self().value);
    auto token_itr = tokens_t.require_find(quantity.symbol.raw(), "Token must be whitelisted to be sent from this contract.");

    eosio::action(permission_level{get_self(), "active"_n}, token_itr->token.contract, "transfer"_n, std::make_tuple(get_self(),to_chain_account,quantity,memo)).send();
    
  }

  ACTION setlevel(const name chainomic_account, const uint8_t level) {
    require_auth(get_self());
    accounts accounts_t(get_self(), get_self().value);
    auto chainomic_account_itr = accounts_t.require_find(chainomic_account.value,"chainomic_account does not exist.");
    check(power >= 0, "Can only have zero or positive chainomicpower.");
    accounts_t.modify(chainomic_account_itr, get_self(), [&](auto &row) { 
      row.level = level;
    });
  }

  void mychainomics::handlexfer(const name& from, const name& to, const asset& quantity, const std::string& memo) {
    if (to != get_self()) return;
    if (from == get_self()) return;

    check( quantity.is_valid(), "invalid quantity" );
    check( quantity.amount > 0, "must transfer positive quantity" );
    check( memo.size() <= 256, "memo has more than 256 bytes" );

    tokens tokens_t(get_self(), get_self().value);
    auto token_itr = tokens_t.require_find(quantity.symbol.raw(), "Token must be whitelisted to be sent to/from this contract.");
    check(get_first_receiver() == token_itr->token.contract, "Invalid origin token contract.");

    const name destination_account = name_from_memo(memo);
    if (destination_account == get_self()) return;

    accounts accounts_t(get_self(), get_self().value);
    auto destination_account_itr = accounts_t.require_find(destination_account.value,"destination_account does not exist.");

    add_account_token(accounts_t,destination_account_itr, quantity);

    tokens_t.modify(token_itr, get_self(), [&](auto &row) { 
      row.token.quantity += quantity;
    });
  }

}
