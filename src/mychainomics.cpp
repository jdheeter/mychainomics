#include <mychainomics.hpp>
#include <token_functions.cpp>
#include <datatype_functions.cpp>


ACTION mychainomics::adduser(const name& owner, const name& chainomics_id) {
  require_auth(owner);
  users users_t(get_self(), get_self().value);
  auto existing = users_t.find(chainomics_id.value);
  check(existing == users_t.end(), "chainomics_id is already registered.");
  users_t.emplace(owner, [&](auto& row) {
    row.chainomics_id = chainomics_id;
    row.owner = owner;
    row.level = 0;
    row.verified = false;
  });
}

ACTION mychainomics::addprovider(const name& owner, const name& provider_id) {
  require_auth(owner);
  providers providers_t(get_self(), get_self().value);
  auto existing = providers_t.find(provider_id.value);
  check(existing == providers_t.end(), "provider_id is already registered.");
  providers_t.emplace(owner, [&](auto& row) {
    row.owner = owner;
    row.provider_id = provider_id;
  });
};

ACTION mychainomics::addconsumer(const name& owner, const name& consumer_id) {
  require_auth(owner);
  consumers consumers_t(get_self(), get_self().value);
  auto existing = consumers_t.find(consumer_id.value);
  check(existing == consumers_t.end(), "consumer_id is already registered.");
  consumers_t.emplace(owner, [&](auto& row) {
    row.owner = owner;
    row.consumer_id = consumer_id;
  });
};

ACTION mychainomics::freezeuser(const name& chainomics_id, const bool& freeze){
  require_auth(get_self());
  users users_t(get_self(), get_self().value);
  auto chainomic_account_itr = users_t.find(chainomics_id.value);
  check(chainomic_account_itr != users_t.end(), "chainomics_id does not exist.");
  users_t.modify(chainomic_account_itr, get_self(), [&](auto& row) {
    row.admin_freeze = freeze;
  });
}


ACTION mychainomics::verifyuser(const name& chainomics_id, const string& id_verification_hash) {
  require_auth(get_self());
  users users_t(get_self(), get_self().value);
  auto chainomic_account_itr = users_t.find(chainomics_id.value);
  check(chainomic_account_itr != users_t.end(), "chainomics_id does not exist.");
  users_t.modify(chainomic_account_itr, get_self(), [&](auto& row) {
    row.verified = true;
  });
}

ACTION mychainomics::setusermeta(const name& chainomics_id, const string& new_user_meta) {
  require_auth(get_self());
  users users_t(get_self(), get_self().value);
  auto chainomic_account_itr = users_t.find(chainomics_id.value);
  check(chainomic_account_itr != users_t.end(), "chainomics_id does not exist.");
  users_t.modify(chainomic_account_itr, get_self(), [&](auto& row) {
    row.meta = new_user_meta;
  });
}

ACTION mychainomics::providermeta(const name& provider_id, const string& new_provider_meta) {
  require_auth(get_self());
  providers providers_t(get_self(), get_self().value);
  auto provider_itr = providers_t.find(provider_id.value);
  check(provider_itr != providers_t.end(), "provider_id does not exist.");
  providers_t.modify(provider_itr, get_self(), [&](auto& row) {
    row.meta = new_provider_meta;
  });
}

ACTION mychainomics::consumermeta(const name& consumer_id, const string& new_consumer_meta) {
  require_auth(get_self());
  consumers consumers_t(get_self(), get_self().value);
  auto consumer_itr = consumers_t.find(consumer_id.value);
  check(consumer_itr != consumers_t.end(), "consumer_id does not exist.");
  consumers_t.modify(consumer_itr, get_self(), [&](auto& row) {
    row.meta = new_consumer_meta;
  });
}

ACTION mychainomics::authdataprov(const name& chainomics_id, const name& provider_id, const bool& remove) {
  users users_t(get_self(), get_self().value);
  auto chainomics_account_itr = users_t.require_find(chainomics_id.value, "chainomics_id does not exist.");
  require_auth(chainomics_account_itr->owner);
  modify_authorized_provider(users_t, chainomics_account_itr, provider_id,remove);
}

ACTION mychainomics::authdatacons(const name& chainomics_id, const name& consumer_id, const bool& remove) {
  users users_t(get_self(), get_self().value);
  auto chainomics_account_itr = users_t.require_find(chainomics_id.value, "chainomics_id does not exist.");
  require_auth(chainomics_account_itr->owner);
  modify_authorized_consumer(users_t, chainomics_account_itr, consumer_id, remove);
}

ACTION mychainomics::addtoken(const extended_asset extended_asset) {
  require_auth(get_self());
  tokens tokens_t(get_self(), get_self().value);
  tokens_t.emplace(get_self(), [&](auto& row) {
    row.token = extended_asset;
  });
};

ACTION mychainomics::delusers() {
  require_auth(get_self());
  const uint32_t rows = 1000;
  cleanTable<users>(get_self(), get_self().value, rows);
}

ACTION mychainomics::deltokens() {
  require_auth(get_self());
  const uint32_t rows = 1000;
  cleanTable<tokens>(get_self(), get_self().value, rows);
}

ACTION mychainomics::delproviders() {
  require_auth(get_self());
  const uint32_t rows = 1000;
  cleanTable<providers>(get_self(), get_self().value, rows);
}

ACTION mychainomics::delconsumers() {
  require_auth(get_self());
  const uint32_t rows = 1000;
  cleanTable<consumers>(get_self(), get_self().value, rows);
}

ACTION mychainomics::internalxfer(const name& from_chainomic_account, const name& to_chainomic_account, asset& quantity, const std::string& memo) {
  users users_t(get_self(), get_self().value);
  auto from_chainomic_account_itr = users_t.require_find(from_chainomic_account.value, "from_chainomic_account does not exist.");
  require_auth(from_chainomic_account_itr->owner);
  auto to_chainomic_account_itr = users_t.require_find(to_chainomic_account.value, "to_chainomic_account does not exist.");

  check(quantity.is_valid(), "invalid quantity");
  check(quantity.amount > 0, "must transfer positive quantity");
  check(memo.size() <= 256, "memo has more than 256 bytes");

  tokens tokens_t(get_self(), get_self().value);
  auto valid_token = tokens_t.require_find(quantity.symbol.raw(), "Invalid token type.");

  check(quantity.symbol == valid_token->token.get_extended_symbol().get_symbol(), "symbol precision mismatch");

  sub_account_token(users_t, from_chainomic_account_itr, quantity);
  add_account_token(users_t, to_chainomic_account_itr, quantity);
}

ACTION mychainomics::externalxfer(const name& from_chainomic_account, const name& to_chain_account, asset& quantity, const std::string& memo) {
  users users_t(get_self(), get_self().value);
  auto from_chainomic_account_itr = users_t.require_find(from_chainomic_account.value, "from_chainomic_account does not exist.");
  require_auth(from_chainomic_account_itr->owner);
  check(is_account(to_chain_account), "to_chain_account does not exist.");

  check(quantity.is_valid(), "invalid quantity");
  check(quantity.amount > 0, "must transfer positive quantity");
  check(memo.size() <= 256, "memo has more than 256 bytes");

  sub_account_token(users_t, from_chainomic_account_itr, quantity);

  tokens tokens_t(get_self(), get_self().value);
  auto token_itr = tokens_t.require_find(quantity.symbol.raw(), "Token must be whitelisted to be sent from this contract.");

  eosio::action(permission_level{get_self(), "active"_n}, token_itr->token.contract, "transfer"_n, std::make_tuple(get_self(), to_chain_account, quantity, memo)).send();
}

ACTION mychainomics::setlevel(const name& chainomic_account, const uint8_t& level) {
  require_auth(get_self());
  users users_t(get_self(), get_self().value);
  auto chainomic_account_itr = users_t.require_find(chainomic_account.value, "chainomic_account does not exist.");
  check(level >= 0, "level must be 0 or more.");
  users_t.modify(chainomic_account_itr, get_self(), [&](auto& row) {
    row.level = level;
  });
}
