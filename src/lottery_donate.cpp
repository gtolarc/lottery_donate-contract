#include <lottery_donate.hpp>

void transfer_eos(name from, name to, uint64_t quantity, std::string memo) {
    action(permission_level{from, "active"_n}, "eosio.token"_n, "transfer"_n,
           std::make_tuple(from, to, asset(quantity, symbol("EOS", 4)), memo))
        .send();
}

uint8_t get_random_number() {  // Do not use this random function for important logic.
    auto tapos = tapos_block_prefix() * tapos_block_num();
    const char* seed = reinterpret_cast<const char*>(&tapos);
    auto hash = sha256(seed, sizeof(seed)).data();
    uint8_t r = (hash[0] + hash[1]) % 10 + 1;
    return r;
}

NOTIFY("eosio.token::transfer") lottery_donate::transfer(name from, name to, asset quantity, std::string memo) {
    check(_first_receiver == "eosio.token"_n, "should be eosio.token");

    auto transfer_data = unpack_action_data<st_transfer>();
    uint64_t amount = transfer_data.quantity.amount;

    if (transfer_data.from == _self) {       // withdraw
    } else if (transfer_data.to == _self) {  // deposit
        if (amount == 1000 || amount == 10000 || amount == 100000 || amount == 1000000) {
            auto existing_game = _games.find(amount);
            if (existing_game != _games.end()) {
                _games.modify(existing_game, _self, [](auto& g) { g.count += 1; });
            } else {
                _games.emplace(_self, [&](auto& g) {
                    g.amount = amount;
                    g.count = 1;
                });
            }

            uint8_t i = existing_game->count == 0 ? 1 : existing_game->count;
            auto existing_balance = _balances.find(amount);
            if (existing_balance == _balances.end()) {
                _balances.emplace(_self, [&](auto& b) {
                    b.num = i + amount;
                    b.holder = transfer_data.from;
                });
            }

            if (i == 10) {
                uint8_t random_number = get_random_number();
                uint64_t pos = random_number + amount;
                auto existing_balance = _balances.find(pos);
                if (existing_balance != _balances.end()) {
                    transfer_eos(_self, existing_balance->holder, 4.9 * amount,
                                 "Congratulations! You won the lottery.");
                    transfer_eos(_self, "humaneloveio"_n, 4.9 * amount, "Donations!");

                    for (uint8_t i = 1; i <= 10; i++) {
                        auto existing = _balances.find(i + amount);
                        if (existing != _balances.end())
                            _balances.erase(existing);
                    }

                    auto existing = _games.find(amount);
                    if (existing != _games.end())
                        _games.erase(existing);

                    _histories.emplace(_self, [&](auto& h) {
                        h.seq = _histories.available_primary_key();
                        h.amount = amount;
                        h.holder = existing_balance->holder;
                        h.count = random_number;
                    });
                } else {
                    check(false, "could not find the winner");
                }
            }
        } else {
            char msg[256];
            snprintf(msg, 256, "%.4f EOS is an unacceptable value as an input", amount / 10000.0);
            check(false, msg);
        }
    }
}
