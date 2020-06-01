void mychainomics::modify_authorized_provider(users& users_t, mychainomics::users::const_iterator chainomics_account_itr, const name& provider_id, const bool& remove) {
  users_t.modify(chainomics_account_itr, get_self(), [&](auto& row) {
    auto provider_itr = std::find_if(row.authorized_data_providers.begin(), row.authorized_data_providers.end(), [&](auto& existing_provider_id) {
      if (existing_provider_id.value == provider_id.value)
        return true;
      else
        return false;
    });
    if (provider_itr == row.authorized_data_providers.end()) {
      check(!remove, "Specified provider can't be removed as it is not registered for this user.");
      row.authorized_data_providers.push_back(provider_id);
    } else {
      check(remove, "Specified provider is already authorized for this user.");
      row.authorized_data_providers.erase(provider_itr);
    }
  });
}

void mychainomics::modify_authorized_consumer(users& users_t, mychainomics::users::const_iterator chainomics_account_itr, const name& consumer_id, const bool& remove) {
  users_t.modify(chainomics_account_itr, get_self(), [&](auto& row) {
    auto consumer_itr = std::find_if(row.authorized_data_consumers.begin(), row.authorized_data_consumers.end(), [&](auto& existing_consumer_id) {
      if (existing_consumer_id.value == consumer_id.value)
        return true;
      else
        return false;
    });
    if (consumer_itr == row.authorized_data_consumers.end()) {
      check(!remove, "Specified consumer can't be removed as it is not registered for this user.");
      row.authorized_data_consumers.push_back(consumer_id);
    } else {
      check(remove, "Specified consumer is already authorized for this user.");
      row.authorized_data_consumers.erase(consumer_itr);
    }
  });
}

void mychainomics::modify_consumer_supported_datatype(consumers& consumers_t, mychainomics::consumers::const_iterator consumer_account_itr, const name& consumer_id, const bool& remove) {
  consumers_t.modify(consumer_account_itr, get_self(), [&](auto& row) {
    auto consumer_itr = std::find_if(row.supported_datatypes.begin(), row.supported_datatypes.end(), [&](auto& existing_consumer_id) {
      if (existing_consumer_id.value == consumer_id.value)
        return true;
      else
        return false;
    });
    if (consumer_itr == row.supported_datatypes.end()) {
      check(!remove, "Specified datatype can't be removed as it is not registered for this consumer.");
      row.supported_datatypes.push_back(consumer_id);
    } else {
      check(remove, "Specified datatype is already registered for this consumer.");
      row.supported_datatypes.erase(consumer_itr);
    }
  });
}

void mychainomics::modify_provider_supported_datatype(providers& providers_t, mychainomics::providers::const_iterator provider_account_itr, const name& provider_id, const bool& remove) {
  providers_t.modify(provider_account_itr, get_self(), [&](auto& row) {
    auto provider_itr = std::find_if(row.supported_datatypes.begin(), row.supported_datatypes.end(), [&](auto& existing_provider_id) {
      if (existing_provider_id.value == provider_id.value)
        return true;
      else
        return false;
    });
    if (provider_itr == row.supported_datatypes.end()) {
      check(!remove, "Specified datatype can't be removed as it is not registered for this provider.");
      row.supported_datatypes.push_back(provider_id);
    } else {
      check(remove, "Specified datatype is already registered for this provider.");
      row.supported_datatypes.erase(provider_itr);
    }
  });
}

