#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>

using namespace eosio;

class [[eosio::contract("hemerton")]] hemerton : public eosio::contract {

public:
  
  hemerton(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds), total(_self, _self.value) {}

  [[eosio::action]]
  void open(name user) {
    require_auth( user );
    index default_ind;
    auto results = total.get_or_create(_self, default_ind);
    uint32_t request_index = results.total_requests + 1;
    data_index table( get_self(), get_first_receiver().value );
    table.emplace(user, [&]( auto& row ) {
      row.key = request_index;
      row.user = user;
      row.data = "Empty";
    });
    total.set(index{request_index}, user);
  }

  [[eosio::action]]
  void erase(int request, name user) {
    require_auth(user);

    data_index table( get_self(), get_first_receiver().value);

    auto iterator = table.find(request);
    check(iterator != table.end(), "Record does not exist");
    table.erase(iterator);
  }

  [[eosio::action]]
  void getindex() {
    auto result = total.get();
    print(result.total_requests);
  }

  [[eosio::action]]
  void upload(int request, name user, std::string data) {
    require_auth(user);

    data_index table( get_self(), get_first_receiver().value);

    auto iterator = table.find(request);
    check(iterator != table.end(), "Record does not exist");
    table.modify(iterator, user, [&]( auto& row ) {
        row.data = data;
    });
  }

private:
  struct [[eosio::table]] data {
    int key;
    name user;
    std::string data;
    uint64_t primary_key() const { return key; }
  };
  typedef eosio::multi_index<"proofs"_n, data> data_index;

  struct index {
    uint32_t total_requests = 0;
  };
  typedef eosio::singleton<"index"_n, index> index_table;

  index_table total;

};