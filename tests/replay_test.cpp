#include "jacks/replay.hpp"

#include <gtest/gtest.h>

namespace {

void expect_state(const jacks::DepositState& actual, const jacks::DepositState& expected) {
    EXPECT_EQ(actual.cash_minor_units, expected.cash_minor_units);
    EXPECT_EQ(actual.quantity_units, expected.quantity_units);
    EXPECT_EQ(actual.mark_minor_units, expected.mark_minor_units);
    EXPECT_EQ(actual.net_deposits_minor_units, expected.net_deposits_minor_units);
    EXPECT_EQ(actual.last_sequence, expected.last_sequence);
}

TEST(Replay, EmptyRunRetainsOpeningSnapshot) {
    const jacks::DepositState opening{100, 0, std::nullopt, 0, 0};
    const auto result = jacks::replay_deposits(opening, {});
    ASSERT_TRUE(std::holds_alternative<jacks::ReplayReport>(result));
    const auto& report = std::get<jacks::ReplayReport>(result);
    expect_state(report.opening_state, opening);
    expect_state(report.final_state, opening);
    EXPECT_EQ(report.opening_equity.minor_units, 100);
    EXPECT_EQ(report.final_equity.minor_units, 100);
    EXPECT_TRUE(report.accepted.empty());
    EXPECT_FALSE(report.failure.has_value());
}

TEST(Replay, InvalidOpeningRejectsBeforeEventsIncludingEmptyInput) {
    const std::vector<jacks::DepositState> invalid_openings{
        {-1, 0, std::nullopt, 0, 0},
        {0, -1, 1, 0, 0},
        {0, 0, -1, 0, 0},
        {0, 1, std::nullopt, 0, 0},
        {INT64_MAX, 1, 1, 0, 0},
        {0, 0, std::nullopt, -1, 0},
        {0, 0, std::nullopt, 1, 0},
        {0, 0, std::nullopt, 0, -1},
        {0, 0, std::nullopt, 0, 1}
    };
    for (std::size_t index = 0; index < invalid_openings.size(); ++index) {
        SCOPED_TRACE(index);
        for (const auto& events : std::vector<std::vector<jacks::Deposit>>{{}, {{0, 0}}}) {
            const auto result = jacks::replay_deposits(invalid_openings[index], events);
            ASSERT_TRUE(std::holds_alternative<jacks::ReplayError>(result));
            EXPECT_EQ(std::get<jacks::ReplayError>(result), jacks::ReplayError::invalid_initial_state);
        }
    }
}

TEST(Replay, RecordsEachAcceptedResultInCallerOrder) {
    const jacks::DepositState opening{100, 2, 10, 0, 0};
    const std::vector<jacks::Deposit> events{{25, 1}, {7, 3}};
    const auto result = jacks::replay_deposits(opening, events);
    ASSERT_TRUE(std::holds_alternative<jacks::ReplayReport>(result));
    const auto& report = std::get<jacks::ReplayReport>(result);
    ASSERT_EQ(report.accepted.size(), 2u);
    EXPECT_FALSE(report.failure.has_value());
    expect_state(report.opening_state, opening);
    EXPECT_EQ(report.opening_equity.minor_units, 120);
    EXPECT_EQ(report.accepted[0].input_index, 0u);
    expect_state(report.accepted[0].applied.state, {125, 2, 10, 25, 1});
    EXPECT_EQ(report.accepted[0].applied.equity.minor_units, 145);
    EXPECT_EQ(report.accepted[1].input_index, 1u);
    expect_state(report.accepted[1].applied.state, {132, 2, 10, 32, 3});
    EXPECT_EQ(report.accepted[1].applied.equity.minor_units, 152);
    expect_state(report.final_state, {132, 2, 10, 32, 3});
    EXPECT_EQ(report.final_equity.minor_units, 152);
}

TEST(Replay, RejectionKeepsPrefixAndInputsWithoutProcessingSuffix) {
    jacks::DepositState opening{100, 0, std::nullopt, 0, 0};
    std::vector<jacks::Deposit> events{{5, 3}, {7, 2}, {11, 4}};
    const auto opening_before = opening;
    const auto events_before = events;
    const auto result = jacks::replay_deposits(opening, events);
    ASSERT_TRUE(std::holds_alternative<jacks::ReplayReport>(result));
    const auto& report = std::get<jacks::ReplayReport>(result);
    ASSERT_EQ(report.accepted.size(), 1u);
    ASSERT_TRUE(report.failure.has_value());
    EXPECT_EQ(report.failure->input_index, 1u);
    EXPECT_EQ(report.failure->error, jacks::DepositError::invalid_sequence);
    EXPECT_EQ(report.accepted[0].input_index, 0u);
    expect_state(report.accepted[0].applied.state, {105, 0, std::nullopt, 5, 3});
    EXPECT_EQ(report.accepted[0].applied.equity.minor_units, 105);
    expect_state(report.final_state, {105, 0, std::nullopt, 5, 3});
    EXPECT_EQ(report.final_equity.minor_units, 105);
    expect_state(report.opening_state, opening_before);
    EXPECT_EQ(report.opening_equity.minor_units, 100);
    expect_state(opening, opening_before);
    ASSERT_EQ(events.size(), events_before.size());
    for (std::size_t index = 0; index < events.size(); ++index) {
        EXPECT_EQ(events[index].amount_minor_units, events_before[index].amount_minor_units);
        EXPECT_EQ(events[index].sequence, events_before[index].sequence);
    }
}

} // namespace
