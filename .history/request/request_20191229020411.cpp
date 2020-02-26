#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("request")]] request : public eosio::contract {

public:
  
  request(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
  void append(name id, std::string user, time_t timestamp, std::vector<name> net_user, std::vector<name> net_subuser) {
    require_auth( id );
    user_index users( get_self(), get_first_receiver().value );
    auto iterator = users.find(id.value);
    if( iterator == users.end() )
    {
      users.emplace(id, [&]( auto& row ) {
        row.id = id;
        row.role = role;
        row.timestamp = timestamp;
        row.net_user = net_user;
        row.net_subuser = net_subuser;
      });
    }
    else {
      users.modify(iterator, id, [&]( auto& row ) {
        row.id = id;
        row.role = role;
        row.timestamp = timestamp;
        row.net_user = net_user;
        row.net_subuser = net_subuser;
      });
    }
  }

  [[eosio::action]]
  void remove(name id) {
    require_auth( id );

    user_index users( get_self(), get_first_receiver().value);

    auto iterator = users.find(id.value);
    check(iterator != users.end(), "Record does not exist");
    users.erase(iterator);
  }

private:
  struct [[eosio::table]] user {
    name id;
    std::string user;
    time_t timestamp;
    uint64_t primary_key() const { return id.value; }
  };
  typedef eosio::multi_index<"users"_n, user> user_index;

};
