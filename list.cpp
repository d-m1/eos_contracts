#include <eosio/eosio.hpp>
#include <ctime>


using namespace eosio;

class [[eosio::contract("smartlist")]] smartlist : public eosio::contract {

public:
  
  smartlist(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  [[eosio::action]]
  void upsert(name user, std::string first_name, std::string last_name, std::string street, std::string city, std::string state) {
    require_auth( user );
    address_index addresses( get_self(), get_first_receiver().value );
    auto iterator = addresses.find(user.value);
    if( iterator == addresses.end() )
    {
      addresses.emplace(user, [&]( auto& row ) {
       row.key = user;
       row.first_name = first_name;
       row.last_name = last_name;
       row.street = street;
       row.city = city;
       row.state = state;
      });
    }
    else {
      addresses.modify(iterator, user, [&]( auto& row ) {
        row.key = user;
        row.first_name = first_name;
        row.last_name = last_name;
        row.street = street;
        row.city = city;
        row.state = state;
      });
    }
  }

  [[eosio::action]]
  void erase(name user) {
    require_auth(user);

    address_index addresses( get_self(), get_first_receiver().value);

    auto iterator = addresses.find(user.value);
    check(iterator != addresses.end(), "Record does not exist");
    addresses.erase(iterator);
  }

private:
  struct [[eosio::table]] person {
    std::string key;
    std::string topic;
    name user;
	std::time_t timestamp;
	uint64_t temp;
	uint64_t temp_per_action;
	uint64_t entropy;
	uint64_t proofs_number;
    std::string street;
    std::string city;
    std::string state;
    uint64_t primary_key() const { return key.value; }
  };
  typedef eosio::multi_index<"people"_n, person> address_index;

};  