#include "jacks/replay.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <vector>

// Senior research acceptance scenarios, separate from apprentice unit tests.
// Literal expected balances: company/briefs/RES-003-deposit-fixtures.md.
// No alternate transition algorithm or profit implementation is used here.
namespace {

void expect_state(const jacks::DepositState& actual,
                  const jacks::DepositState& expected) {
    EXPECT_EQ(actual.cash_minor_units, expected.cash_minor_units);
    EXPECT_EQ(actual.quantity_units, expected.quantity_units);
    EXPECT_EQ(actual.mark_minor_units, expected.mark_minor_units);
    EXPECT_EQ(actual.net_deposits_minor_units, expected.net_deposits_minor_units);
    EXPECT_EQ(actual.last_sequence, expected.last_sequence);
}

void expect_prefixes(const jacks::DepositState& opening,
                     const jacks::Equity opening_equity,
                     const std::vector<jacks::Deposit>& events,
                     const std::vector<jacks::DepositApplied>& expected) {
    ASSERT_EQ(events.size(), expected.size());
    // Includes empty input and each nonempty caller-ordered prefix.
    for (std::size_t count = 0; count <= events.size(); ++count) {
        SCOPED_TRACE(::testing::Message() << "accepted prefix length " << count);
        std::vector<jacks::Deposit> input;
        for (std::size_t i = 0; i < count; ++i) {
            input.push_back(events[i]);
        }
        const auto saved_input = input;
        auto initial = opening;
        const auto result = jacks::replay_deposits(initial, input);
        expect_state(initial, opening);
        ASSERT_EQ(input.size(), saved_input.size());
        for (std::size_t i = 0; i < input.size(); ++i) {
            EXPECT_EQ(input[i].amount_minor_units, saved_input[i].amount_minor_units);
            EXPECT_EQ(input[i].sequence, saved_input[i].sequence);
        }

        ASSERT_TRUE(std::holds_alternative<jacks::ReplayReport>(result));
        const auto& report = std::get<jacks::ReplayReport>(result);
        EXPECT_FALSE(report.failure.has_value());
        expect_state(report.opening_state, opening);
        EXPECT_EQ(report.opening_equity.minor_units, opening_equity.minor_units);
        ASSERT_EQ(report.accepted.size(), count);
        for (std::size_t i = 0; i < count; ++i) {
            SCOPED_TRACE(::testing::Message() << "input index " << i);
            EXPECT_EQ(report.accepted[i].input_index, i);
            expect_state(report.accepted[i].applied.state, expected[i].state);
            EXPECT_EQ(report.accepted[i].applied.equity.minor_units,
                      expected[i].equity.minor_units);
        }
        if (count == 0) {
            expect_state(report.final_state, opening);
            EXPECT_EQ(report.final_equity.minor_units, opening_equity.minor_units);
        } else {
            expect_state(report.final_state, expected[count - 1].state);
            EXPECT_EQ(report.final_equity.minor_units,
                      expected[count - 1].equity.minor_units);
        }
    }
}

TEST(ReplayEconomic, CashOnlyFundingWithAbsentMarkAndSequenceGap) {
    const jacks::DepositState opening{10000, 0, std::nullopt, 0, 0};
    const std::vector<jacks::Deposit> events{{2500, 1}, {750, 3}};
    const std::vector<jacks::DepositApplied> expected{
        {{12500, 0, std::nullopt, 2500, 1}, {12500}},
        {{13250, 0, std::nullopt, 3250, 3}, {13250}}};
    expect_prefixes(opening, {10000}, events, expected);
    // Hand-derived expected profit is zero: 13250 - 10000 - 3250.
    // This checks trace components, not the pending L-003 profit function.
}

TEST(ReplayEconomic, FixedHoldingsRemainPartOfOpeningAndEndingEquity) {
    const jacks::DepositState opening{10000, 2, 3000, 0, 0};
    const std::vector<jacks::Deposit> events{{5000, 1}, {2000, 2}};
    const std::vector<jacks::DepositApplied> expected{
        {{15000, 2, 3000, 5000, 1}, {21000}},
        {{17000, 2, 3000, 7000, 2}, {23000}}};
    expect_prefixes(opening, {16000}, events, expected);
    // Hand-derived expected profit is zero: 23000 - 16000 - 7000.
}

} // namespace
