cmake_minimum_required(VERSION 3.20)
project(Jacks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(cmake/JacksWarnings.cmake)

add_library(jacks_valuation STATIC src/valuation.cpp)
target_include_directories(jacks_valuation PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
jacks_enable_warnings(jacks_valuation)

add_library(jacks_deposit STATIC src/deposit.cpp)
target_link_libraries(jacks_deposit PUBLIC jacks_valuation)
jacks_enable_warnings(jacks_deposit)

add_library(jacks_replay STATIC src/replay.cpp)
target_link_libraries(jacks_replay PUBLIC jacks_deposit)
jacks_enable_warnings(jacks_replay)

add_library(jacks_profit STATIC src/profit.cpp)
target_link_libraries(jacks_profit PUBLIC jacks_valuation)
jacks_enable_warnings(jacks_profit)

add_library(jacks_mark STATIC src/mark.cpp)
target_link_libraries(jacks_mark PUBLIC jacks_deposit)
jacks_enable_warnings(jacks_mark)

add_library(jacks_fill STATIC src/fill.cpp)
target_link_libraries(jacks_fill PUBLIC jacks_deposit)
jacks_enable_warnings(jacks_fill)

add_library(jacks_account_replay STATIC src/account_replay.cpp)
target_link_libraries(jacks_account_replay PUBLIC jacks_deposit jacks_mark jacks_fill)
jacks_enable_warnings(jacks_account_replay)

add_executable(jacks_account_demo examples/account_demo.cpp)
target_link_libraries(jacks_account_demo PRIVATE jacks_replay jacks_profit)
jacks_enable_warnings(jacks_account_demo)

add_executable(jacks_trading_demo examples/trading_demo.cpp)
target_link_libraries(jacks_trading_demo PRIVATE jacks_deposit jacks_fill jacks_mark jacks_profit)
jacks_enable_warnings(jacks_trading_demo)

include(CTest)
if(BUILD_TESTING)
  include(cmake/JacksTesting.cmake)
  jacks_add_tests(jacks_tests
    SOURCES tests/infrastructure_test.cpp tests/valuation_test.cpp tests/deposit_test.cpp
    LIBRARIES jacks_deposit)
  jacks_add_tests(jacks_replay_tests SOURCES tests/replay_test.cpp LIBRARIES jacks_replay)
  add_subdirectory(tests/research)
  add_subdirectory(tests/risk)
  jacks_add_tests(jacks_profit_tests SOURCES tests/profit_test.cpp LIBRARIES jacks_profit)
  jacks_add_tests(jacks_mark_tests SOURCES tests/mark_test.cpp LIBRARIES jacks_mark)
  jacks_add_tests(jacks_fill_tests SOURCES tests/fill_test.cpp LIBRARIES jacks_fill)
  # Mixed account replay remains apprentice-owned; its placeholder is an explicit skip.
  jacks_add_tests(jacks_account_replay_tests
    SOURCES tests/account_replay_test.cpp LIBRARIES jacks_account_replay)
  jacks_add_tests(jacks_replay_profit_tests
    SOURCES tests/replay_profit_test.cpp LIBRARIES jacks_replay jacks_profit)
  add_test(NAME AccountDemo.Runs COMMAND jacks_account_demo)
  add_test(NAME TradingDemo.Reconciles COMMAND jacks_trading_demo)
endif()
