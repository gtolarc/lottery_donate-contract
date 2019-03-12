#include <eosiolib/eosio.hpp>
using namespace eosio;

CONTRACT lottery_donate : public contract {
   public:
      using contract::contract;

      ACTION hi( name nm );

      using hi_action = action_wrapper<"hi"_n, &lottery_donate::hi>;
};