#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>
#include <eosio/eosio.hpp>
#include <eosio/transaction.hpp>

#define NOTIFY(action) [[eosio::on_notify(action)]] void

using namespace eosio;

CONTRACT lottery_donate : public contract {
   public:
    lottery_donate(name self, name first_receiver, datastream<const char*> ds)
        : contract(self, first_receiver, ds),
          _balances(self, self.value),
          _games(self, self.value),
          _histories(self, self.value) {}

    NOTIFY("eosio.token::transfer") transfer(name from, name to, asset quantity, std::string memo);

   private:
    TABLE balance {
        uint64_t num;
        name holder;
        uint64_t primary_key() const { return num; }
    };

    TABLE game {
        uint64_t amount;
        uint8_t count;
        uint64_t primary_key() const { return amount; }
    };

    TABLE history {
        uint64_t seq;
        name holder;
        uint64_t amount;
        uint8_t count;
        uint64_t primary_key() const { return seq; }
    };

    typedef eosio::multi_index<"balances"_n, balance> balances;
    typedef eosio::multi_index<"games"_n, game> games;
    typedef eosio::multi_index<"histories"_n, history> histories;

    balances _balances;
    games _games;
    histories _histories;

    struct st_transfer {
        name from;
        name to;
        asset quantity;
        std::string memo;
    };
};
