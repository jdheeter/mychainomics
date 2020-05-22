#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <cmath>
#include <eosio/asset.hpp>
#include <eosio/print.hpp>
#include <eosio/singleton.hpp>
#include <string>
#define SECONDS_DAY 86400
using eosio::microseconds;

namespace eosiosystem {
class system_contract;
}

namespace eosio {

using std::string;

class [[eosio::contract("mychainomics")]] mychainomics : public contract {
 public:
  using contract::contract;

  ACTION adduser(const name& owner, const name& chainomics_id);
  ACTION addtoken(extended_asset extended_asset);
  ACTION internalxfer(const name& from_chainomic_account, const name& to_chainomic_account, asset& quantity, const std::string& memo);
  ACTION externalxfer(const name& from_chainomic_account, const name& to_chain_account, asset& quantity, const std::string& memo);
  ACTION setlevel(const name chainomic_account, const uint8_t level);

  //DEBUG_ONLY
  ACTION delaccounts();
  ACTION deltokens();
  //DEBUG_ONLY

  [[eosio::on_notify("*::transfer")]] void handlexfer(const name& from, const name& to, const asset& quantity, const std::string& memo);

  // TABLE config_ {

  // };
  // typedef eosio::singleton<"config"_n, config_> config;
  
  // typedef struct datatype {

  // };

  private:
  TABLE providers_ {
    name owner;
    name provider_id;
    string meta;
    std::vector<eosio::asset> datatype;
    uint64_t primary_key() const { return provider_id.value; }
  };
  typedef eosio::multi_index<"providers"_n, providers_> providers;

  TABLE consumers_ {
    name owner;
    name consumer_id;
    string meta;
    std::vector<eosio::asset> datatype;
    uint64_t primary_key() const { return consumer_id.value; }
  };
  typedef eosio::multi_index<"consumers"_n, consumers_> consumers;

  TABLE events_ {
    name dconsumer_owner;
    name event_id;
    uint32_t start_time_ms;
    uint32_t end_time_ms;
    string meta;
    std::vector<eosio::name> required_datatypes;
    std::vector<eosio::name> participants;
    uint64_t primary_key() const { return event_id.value; }
  };
  typedef eosio::multi_index<"events"_n, events_> events;

  TABLE accounts_ {
    name chainomics_id;
    name owner;
    uint8_t level;
    bool verifified;
    string meta;
    std::vector<eosio::asset> tokens;
    std::vector<eosio::name> authorized_data_types;
    std::vector<eosio::name> authorized_data_providers;
    std::vector<eosio::name> authorized_data_consumers;
    uint64_t primary_key() const { return chainomics_id.value; }
  };
  typedef eosio::multi_index<"accounts"_n, accounts_> accounts;

  TABLE tokens_ {
    extended_asset token;
    uint64_t primary_key() const {
      return token.get_extended_symbol().get_symbol().raw();
    }
  };
  typedef eosio::multi_index<"tokens"_n, tokens_> tokens;

  microseconds now() {
    return microseconds(eosio::current_time_point().sec_since_epoch());
  }
  const symbol chainomic_symbol = symbol(symbol_code("mychainomics"), 0);

  uint64_t now_ms = eosio::current_time_point().time_since_epoch().count() / 1000;
  uint32_t now_sec = eosio::current_time_point().sec_since_epoch();
  // auto get_config();

  name name_from_memo(const string memo);
  void add_account_token(accounts & accounts_t, eosio::mychainomics::accounts::const_iterator to_itr, const asset& quantity);
  void sub_account_token(accounts & accounts_t, eosio::mychainomics::accounts::const_iterator from_itr, const asset& quantity);


};

}  // namespace eosio
