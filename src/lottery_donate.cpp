#include <lottery_donate.hpp>

ACTION lottery_donate::join(name user) {}
ACTION lottery_donate::refundinit(uint64_t amount) {}

EOSIO_DISPATCH(lottery_donate, (join)(refundinit))
