#include <cmath>
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>
#include <string>
#define SECONDS_DAY 86400
using eosio::microseconds;

using namespace std;
using namespace eosio;

CONTRACT mychainomics : public contract {
 public:
  using contract::contract;

  ACTION adduser(const name& owner, const name& chainomics_id);
  ACTION addtoken(extended_asset extended_asset);
  ACTION internalxfer(const name& from_chainomic_account, const name& to_chainomic_account, asset& quantity, const std::string& memo);
  ACTION externalxfer(const name& from_chainomic_account, const name& to_chain_account, asset& quantity, const std::string& memo);
  ACTION setlevel(const name& chainomic_account, const uint8_t& level);
  ACTION verifyuser(const name& chainomics_id, const string& id_verification_hash);
  ACTION freezeuser(const name& chainomics_id, const bool& freeze);

  ACTION setusermeta(const name& chainomics_id, const string& new_user_meta);
  ACTION authdataprov(const name& chainomics_id, const name& provider_id, const bool& remove);
  ACTION authdatacons(const name& chainomics_id, const name& provider_id, const bool& remove);

  ACTION addconsumer(const name& owner, const name& consumer_id);
  ACTION addprovider(const name& owner, const name& provider_id);

  ACTION providermeta(const name& provider_id, const string& new_provider_meta);
  ACTION consumermeta(const name& consumer_id, const string& new_consumer_meta);

  ACTION delusers();
  ACTION deltokens();
  ACTION delconsumers();
  ACTION delproviders();


 private:
  
  //-------------------//
  // Tables            //
  //-------------------//

  TABLE users_ {
    name chainomics_id;
    name owner;
    uint8_t level;
    bool verified;
    bool admin_freeze;
    string meta;
    std::vector<eosio::asset> tokens;
    std::vector<eosio::name> authorized_data_types;
    std::vector<eosio::name> authorized_data_providers;
    std::vector<eosio::name> authorized_data_consumers;
    uint64_t primary_key() const { return chainomics_id.value; }
  };
  typedef eosio::multi_index<"users"_n, users_> users;

  TABLE tokens_ {
    extended_asset token;
    uint64_t primary_key() const {
      return token.get_extended_symbol().get_symbol().raw();
    }
  };
  typedef eosio::multi_index<"tokens"_n, tokens_> tokens;

  TABLE providers_ {
    name owner;
    name provider_id;
    string meta;
    std::vector<eosio::name> supported_datatypes;
    uint64_t primary_key() const { return provider_id.value; }
  };
  typedef eosio::multi_index<"providers"_n, providers_> providers;

  TABLE consumers_ {
    name owner;
    name consumer_id;
    string meta;
    std::vector<eosio::name> supported_datatypes;
    uint64_t primary_key() const { return consumer_id.value; }
  };
  typedef eosio::multi_index<"consumers"_n, consumers_> consumers;

  TABLE events_ {
    name consumer_owner;
    name event_id;
    uint32_t start_time_ms;
    uint32_t end_time_ms;
    string meta;
    std::vector<eosio::name> required_datatypes;
    std::vector<eosio::name> participants;
    uint64_t primary_key() const { return event_id.value; }
  };
  typedef eosio::multi_index<"events"_n, events_> events;



  //-------------------//
  // Utility Functions //
  //-------------------//
  microseconds now() {
    return microseconds(eosio::current_time_point().sec_since_epoch());
  }

  uint64_t now_ms = eosio::current_time_point().time_since_epoch().count() / 1000;
  uint32_t now_sec = eosio::current_time_point().sec_since_epoch();

  void add_account_token(users & users_t, mychainomics::users::const_iterator to_itr, const asset& quantity);
  void sub_account_token(users & users_t, mychainomics::users::const_iterator from_itr, const asset& quantity);

  void modify_authorized_provider(users & users_t, mychainomics::users::const_iterator chainomics_account_itr, const name& provider_id, const bool& remove);
  void modify_authorized_consumer(users & users_t, mychainomics::users::const_iterator chainomics_account_itr, const name& consumer_id, const bool& remove);
  void modify_consumer_supported_datatype(consumers& consumers_t, mychainomics::consumers::const_iterator consumer_account_itr, const name& consumer_id, const bool& remove);
  void modify_provider_supported_datatype(consumers & providers_t, mychainomics::providers::const_iterator consumer_account_itr, const name& consumer_id, const bool& remove);

  template <typename T>
  void cleanTable(name code, uint64_t account, const uint32_t batchSize) {
    T db(code, account);
    uint32_t counter = 0;
    auto itr = db.begin();
    while (itr != db.end() && counter++ < batchSize) {
      itr = db.erase(itr);
    }
  }
};
