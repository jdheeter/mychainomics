  name mychainomics::name_from_memo(const string memo) {
    const string key = "chainomic_account_name=";
    const string error_string = "You must specify chainomic_account_name=chainomicaccountname in the memo.";
    const uint32_t account_name_index = memo.find(key);
    check(account_name_index != std::string::npos, error_string);
    const uint32_t name_start = account_name_index + key.length();
    check(name_start != std::string::npos, error_string);
    const uint32_t name_end = memo.find_first_of(" ", name_start);
    print(name_start, "\n", name_end, "\n");
    const uint32_t name_length = name_end - name_start;
    print(name_length, "\n");
    const name destination_account = name(memo.substr(name_start, name_length));
    print("valid name: ", destination_account);
    return destination_account;
  }
  void mychainomics::modify_event_applicant(events& events_t, mychainomics::events::const_iterator event_account_itr, const name& chainomics_id, const bool& remove) {
    events_t.modify(event_account_itr, get_self(), [&](auto& row) {
      auto event_itr = std::find_if(row.applicants.begin(), row.applicants.end(), [&](auto& existing_chainomics_id) {
        if (existing_chainomics_id.value == chainomics_id.value)
          return true;
        else
          return false;
      });
      if (event_itr == row.applicants.end()) {
        check(!remove, "Specified applicant can't be removed as it is not registered for this event.");
        row.applicants.push_back(chainomics_id);
      } else {
        check(remove, "Specified applicant is already registered for this event.");
        row.applicants.erase(event_itr);
      }
    });
  }

  void mychainomics::modify_event_participant(events& events_t, mychainomics::events::const_iterator event_account_itr, const name& chainomics_id, const bool& remove) {
    events_t.modify(event_account_itr, get_self(), [&](auto& row) {
      auto event_itr = std::find_if(row.participants.begin(), row.participants.end(), [&](auto& existing_chainomics_id) {
        if (existing_chainomics_id.value == chainomics_id.value)
          return true;
        else
          return false;
      });
      if (event_itr == row.participants.end()) {
        check(!remove, "Specified participant can't be removed as it is not registered for this event.");
        row.participants.push_back(chainomics_id);
      } else {
        check(remove, "Specified participant is already registered for this event.");
        row.participants.erase(event_itr);
      }
    });
  }