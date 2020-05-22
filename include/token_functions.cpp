void mychainomics::add_account_token(users& users_t, mychainomics::users::const_iterator to_itr, const asset& quantity) {
  users_t.modify(to_itr, get_self(), [&](auto& row) {
    auto token_itr = std::find_if(row.tokens.begin(), row.tokens.end(), [&](auto& token) {
      if (token.symbol == quantity.symbol)
        return true;
      else
        return false;
    });
    if (token_itr == row.tokens.end())
      row.tokens.push_back(quantity);
    else
      token_itr->set_amount(token_itr->amount + quantity.amount);
  });
}

void mychainomics::sub_account_token(users& users_t, mychainomics::users::const_iterator from_itr, const asset& quantity ) {

  users_t.modify(from_itr, get_self(), [&](auto &row) {
    auto token_itr = std::find_if(row.tokens.begin(), row.tokens.end(), [&](auto &token){
      if (token.symbol == quantity.symbol) return true;
      else return false;
    });
    check(token_itr != row.tokens.end(),"From account does not own this token.");
    check(token_itr->amount >= quantity.amount, "overdrawn balance" );
    auto new_balance = token_itr->amount - quantity.amount;
    if (new_balance > 0) token_itr->set_amount(new_balance);
    else row.tokens.erase(token_itr);
  });
}