#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/transaction.hpp>
#include <numeric>

using namespace eosio;

CONTRACT lottery_donate : public contract {
   public:
    lottery_donate(name receiver, name code, datastream<const char*> ds)
        : contract(receiver, code, ds),
          _balances(receiver, receiver.value),
          _games(receiver, receiver.value),
          _histories(receiver, receiver.value) {}

    ACTION transfer(name from, name to);

   private:
    using transfer_action = action_wrapper<"transfer"_n, &lottery_donate::transfer>;

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
