namespace eosio {
  name mychainomics::name_from_memo(const string memo) {
    const string error_string = "You must specify chainomic_account_name=chainomicaccountname in the memo.";
    const uint32_t account_name_index = memo.find("chainomic_account_name=");
    check(account_name_index != std::string::npos, error_string);
    const uint32_t name_start = account_name_index + 18;
    check(name_start != std::string::npos, error_string);
    const uint32_t name_end = memo.find_first_of(" ", name_start);
    print(name_start, "\n", name_end, "\n");
    const uint32_t name_length = name_end - name_start;
    print(name_length, "\n");
    const name destination_account = name(memo.substr(name_start, name_length));
    print("valid name: ", destination_account);
    return destination_account;
  }
  // auto mychainomics::get_config(){
  //   config_ _config(get_self(), get_self().value);
  //   auto conf = _config.get_or_create(get_self(), config());
  //   return conf;
  // }

}