#include <lottery_donate.hpp>

ACTION lottery_donate::transfer(name from, name to) {
    auto transfer_data = unpack_action_data<st_transfer>();
    int64_t amount = transfer_data.quantity.amount;

    if (transfer_data.from == _self) {       // withdraw
    } else if (transfer_data.to == _self) {  // deposit
        if (amount == 1000 || amount == 10000 || amount == 100000 || amount == 1000000) {
            auto itr = _games.find(amount);
            if (itr != _games.end()) {
                _games.modify(itr, _self, [](auto& g) { g.count += 1; });
            } else {
                _games.emplace(_self, [&](auto& g) {
                    g.amount = amount;
                    g.count = 1;
                });
            }
        } else {
            char msg[256];
            snprintf(msg, 256, "%.4f EOS is an unacceptable value as an input.", amount / 10000.0);
            eosio_assert(false, msg);
        }
    }
}

void withdraw_eos(name from, name to, uint64_t quantity, std::string memo) {
    action(permission_level{from, "active"_n}, "eosio.token"_n, "transfer"_n,
           std::make_tuple(from, to, asset(quantity, symbol("EOS", 4)), memo))
        .send();
}

uint8_t get_random_number() {  // Do not use this random function for important logic.
    capi_checksum256 hash;
    auto tapos = tapos_block_prefix() * tapos_block_num();
    const char* seed = reinterpret_cast<const char*>(&tapos);
    sha256(seed, sizeof(seed), &hash);
    uint8_t r = std::accumulate(hash.hash, hash.hash + 32, 0) % 10 + 1;
    return r;
}

#define EOSIO_DISPATCH_EX(TYPE, MEMBERS)                                                       \
    extern "C" {                                                                               \
    void apply(uint64_t receiver, uint64_t code, uint64_t action) {                            \
        if (action == "onerror"_n.value) {                                                     \
            eosio_assert(code == "eosio"_n.value,                                              \
                         "onerror action's are only valid from the \"eosio\" system account"); \
        }                                                                                      \
        if (code == receiver && action != "transfer"_n.value ||                                \
            code == "eosio.token"_n.value && action == "transfer"_n.value) {                   \
            switch (action) { EOSIO_DISPATCH_HELPER(TYPE, MEMBERS) }                           \
            /* does not allow destructor of thiscontract to run: eosio_exit(0); */             \
        }                                                                                      \
    }                                                                                          \
    }

EOSIO_DISPATCH_EX(lottery_donate, (transfer))
