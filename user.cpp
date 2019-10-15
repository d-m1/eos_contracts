#include <eosio/eosio.hpp>
#include <ctime>

using namespace eosio;

class [[eosio::contract("user")]] user : public eosio::contract {

public:
  
  user(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
  void append(name id, std::string role, time timestamp, std::vector<name> net_user, std::vector<name> net_subuser;) {
    require_auth( id );
    user_index users( get_self(), get_first_receiver().value );
    auto iterator = users.find(id.value);
    if( iterator == users.end() )
    {
      users.emplace(id, [&]( auto& row ) {
       row.id = id;
       row.role = role;
       row.timestamp = std::time(nullptr);
       row.net_user = street;
       row.net_subuser = city;
      });
    }
    else {
      users.modify(iterator, user, [&]( auto& row ) {
        row.id = id;
        row.role = role;
        row.timestamp = std::time(nullptr);
        row.net_user = street;
        row.net_subuser = city;
      });
    }
  }

  [[eosio::action]]
  void delete(name id) {
    require_auth( id );

    user_index users( get_self(), get_first_receiver().value);

    auto iterator = users.find(user.value);
    check(iterator != users.end(), "Record does not exist");
    users.erase(iterator);
  }

private:
  struct [[eosio::table]] user {
    name id;
    std::string role;
    std::time_t timestamp;
    std::vector<name> net_user;
    std::vector<name> net_subuser;
    uint64_t primary_key() const { return id.value; }
  };
  typedef eosio::multi_index<"users"_n, user> user_index;

};
