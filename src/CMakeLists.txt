project(lottery_donate)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( lottery_donate lottery_donate lottery_donate.cpp )
target_include_directories( lottery_donate PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( lottery_donate ${CMAKE_SOURCE_DIR}/../ricardian )