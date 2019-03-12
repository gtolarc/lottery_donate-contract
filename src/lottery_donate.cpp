#include <lottery_donate.hpp>
ACTION lottery_donate::hi( name nm ) {
   /* fill in action body */
   print_f("Name : %\n",nm);
}

EOSIO_DISPATCH( lottery_donate, (hi) )