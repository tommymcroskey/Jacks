#include "jacks/replay.hpp"

#include <gtest/gtest.h>
#include <limits>

// Independent literal expectations from RISK-006, not a second replay algorithm.
namespace {
using namespace jacks;
constexpr auto maximum = std::numeric_limits<std::int64_t>::max();

void expect_state(const DepositState& actual, const DepositState& expected) {
    EXPECT_EQ(actual.cash_minor_units, expected.cash_minor_units);
    EXPECT_EQ(actual.quantity_units, expected.quantity_units);
    EXPECT_EQ(actual.mark_minor_units, expected.mark_minor_units);
    EXPECT_EQ(actual.net_deposits_minor_units, expected.net_deposits_minor_units);
    EXPECT_EQ(actual.last_sequence, expected.last_sequence);
}

struct Fixture {
    const char* id;
    DepositState opening;
    std::vector<Deposit> events;
    bool invalid_initial;
    Equity opening_equity;
    DepositApplied final;
    std::vector<ReplayStep> accepted;
    std::optional<ReplayFailure> failure;
};

void verify(const Fixture& fixture) {
    auto opening = fixture.opening;
    auto events = fixture.events;
    const auto result = replay_deposits(opening, events);
    expect_state(opening, fixture.opening);
    ASSERT_EQ(events.size(), fixture.events.size());
    for (std::size_t index = 0; index < events.size(); ++index) {
        EXPECT_EQ(events[index].amount_minor_units, fixture.events[index].amount_minor_units);
        EXPECT_EQ(events[index].sequence, fixture.events[index].sequence);
    }
    if (fixture.invalid_initial) {
        ASSERT_TRUE(std::holds_alternative<ReplayError>(result));
        EXPECT_EQ(std::get<ReplayError>(result), ReplayError::invalid_initial_state);
        return;
    }
    ASSERT_TRUE(std::holds_alternative<ReplayReport>(result));
    const auto& report = std::get<ReplayReport>(result);
    expect_state(report.opening_state, fixture.opening);
    EXPECT_EQ(report.opening_equity.minor_units, fixture.opening_equity.minor_units);
    expect_state(report.final_state, fixture.final.state);
    EXPECT_EQ(report.final_equity.minor_units, fixture.final.equity.minor_units);
    ASSERT_EQ(report.failure.has_value(), fixture.failure.has_value());
    if (fixture.failure) {
        EXPECT_EQ(report.failure->input_index, fixture.failure->input_index);
        EXPECT_EQ(report.failure->error, fixture.failure->error);
    }
    ASSERT_EQ(report.accepted.size(), fixture.accepted.size());
    for (std::size_t index = 0; index < fixture.accepted.size(); ++index) {
        SCOPED_TRACE(index);
        EXPECT_EQ(report.accepted[index].input_index, fixture.accepted[index].input_index);
        expect_state(report.accepted[index].applied.state, fixture.accepted[index].applied.state);
        EXPECT_EQ(report.accepted[index].applied.equity.minor_units,
                  fixture.accepted[index].applied.equity.minor_units);
    }
}

std::vector<Fixture> fixtures() {
    // Unused numeric expectations on invalid initialization are never read.
    return {
        {"R01", {100,0,std::nullopt,0,0}, {}, false, {100},
            {{100,0,std::nullopt,0,0},{100}}, {}, std::nullopt},
        {"R02", {0,1,std::nullopt,0,0}, {}, true, {}, {}, {}, std::nullopt},
        {"R03", {100,0,std::nullopt,1,0}, {{1,1}}, true, {}, {}, {}, std::nullopt},
        {"R04", {100,0,std::nullopt,0,1}, {}, true, {}, {}, {}, std::nullopt},
        {"R05", {100,2,10,0,0}, {{25,1},{7,1},{50,2}}, false, {120},
            {{125,2,10,25,1},{145}}, {{0,{{125,2,10,25,1},{145}}}},
            ReplayFailure{1,DepositError::invalid_sequence}},
        {"R06", {100,0,std::nullopt,0,0}, {{5,3},{7,2},{11,4}}, false, {100},
            {{105,0,std::nullopt,5,3},{105}}, {{0,{{105,0,std::nullopt,5,3},{105}}}},
            ReplayFailure{1,DepositError::invalid_sequence}},
        {"R07", {100,0,std::nullopt,0,0}, {{0,0},{5,1}}, false, {100},
            {{100,0,std::nullopt,0,0},{100}}, {}, ReplayFailure{0,DepositError::invalid_amount}},
        {"R08", {12,0,std::nullopt,0,0}, {{maximum,0}}, false, {12},
            {{12,0,std::nullopt,0,0},{12}}, {}, ReplayFailure{0,DepositError::invalid_sequence}},
        {"R09", {0,0,std::nullopt,0,0}, {{maximum,1},{1,2},{1,3}}, false, {0},
            {{maximum,0,std::nullopt,maximum,1},{maximum}},
            {{0,{{maximum,0,std::nullopt,maximum,1},{maximum}}}}, ReplayFailure{1,DepositError::overflow}},
        {"R10", {maximum-2,1,1,0,0}, {{1,1},{1,2}}, false, {maximum-1},
            {{maximum-1,1,1,1,1},{maximum}}, {{0,{{maximum-1,1,1,1,1},{maximum}}}},
            ReplayFailure{1,DepositError::overflow}},
        {"R11", {0,0,-1,0,0}, {{0,0}}, true, {}, {}, {}, std::nullopt},
        {"R12", {maximum,1,1,0,0}, {}, true, {}, {}, {}, std::nullopt}
    };
}

TEST(ReplayRisk, LiteralRejectionAndBoundaryFixtures) {
    for (const auto& fixture : fixtures()) {
        SCOPED_TRACE(fixture.id);
        verify(fixture);
    }
}

TEST(ReplayRisk, AcceptedPrefixMatchesBeforeRejectedSuffixIsAppended) {
    for (const auto& fixture : fixtures()) {
        if (fixture.invalid_initial || fixture.accepted.empty()) {
            continue;
        }
        SCOPED_TRACE(fixture.id);
        auto prefix = fixture;
        prefix.events.resize(prefix.accepted.size());
        prefix.failure.reset();
        verify(prefix);
        // Both runs are checked against the same independent literal rows.
        verify(fixture);
    }
}
} // namespace
