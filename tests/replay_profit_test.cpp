#include "jacks/profit.hpp"
#include "jacks/replay.hpp"

#include <gtest/gtest.h>

TEST(ReplayProfit, DepositsExplainAllValueGrowth) {
    const auto replay = jacks::replay_deposits({10000, 2, 3000, 0, 0}, {{5000, 1}, {2000, 2}});
    ASSERT_TRUE(std::holds_alternative<jacks::ReplayReport>(replay));
    const auto& report = std::get<jacks::ReplayReport>(replay);
    EXPECT_FALSE(report.failure.has_value());
    EXPECT_EQ(report.accepted.size(), 2u);
    EXPECT_EQ(report.opening_equity.minor_units, 16000);
    EXPECT_EQ(report.final_equity.minor_units, 23000);
    EXPECT_EQ(report.final_state.net_deposits_minor_units, 7000);
    const auto profit = jacks::calculate_profit(report.opening_equity,
        report.final_equity, report.final_state.net_deposits_minor_units);
    ASSERT_TRUE(std::holds_alternative<jacks::Profit>(profit));
    EXPECT_EQ(std::get<jacks::Profit>(profit).minor_units, 0);
}

TEST(ReplayProfit, RejectionSummaryCoversOnlyAcceptedPrefix) {
    const auto replay = jacks::replay_deposits({10000, 2, 3000, 0, 0},
        {{5000, 1}, {2000, 2}, {500, 2}, {100, 3}});
    ASSERT_TRUE(std::holds_alternative<jacks::ReplayReport>(replay));
    const auto& report = std::get<jacks::ReplayReport>(replay);
    ASSERT_TRUE(report.failure.has_value());
    EXPECT_EQ(report.failure->input_index, 2u);
    EXPECT_EQ(report.failure->error, jacks::DepositError::invalid_sequence);
    EXPECT_EQ(report.accepted.size(), 2u);
    EXPECT_EQ(report.opening_equity.minor_units, 16000);
    EXPECT_EQ(report.final_equity.minor_units, 23000);
    EXPECT_EQ(report.final_state.net_deposits_minor_units, 7000);
    const auto profit = jacks::calculate_profit(report.opening_equity,
        report.final_equity, report.final_state.net_deposits_minor_units);
    ASSERT_TRUE(std::holds_alternative<jacks::Profit>(profit));
    EXPECT_EQ(std::get<jacks::Profit>(profit).minor_units, 0);
}
