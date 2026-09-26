#include "jacks/account_replay.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using AccountReplaySignature = jacks::AccountReplayResult (*)(
    const jacks::DepositState&, const std::vector<jacks::AccountEvent>&);
static_assert(std::is_same_v<decltype(&jacks::replay_account), AccountReplaySignature>);

namespace {

void expect_state(const jacks::DepositState& actual, const jacks::DepositState& expected) {
    EXPECT_EQ(actual.cash_minor_units, expected.cash_minor_units);
    EXPECT_EQ(actual.quantity_units, expected.quantity_units);
    EXPECT_EQ(actual.mark_minor_units, expected.mark_minor_units);
    EXPECT_EQ(actual.net_deposits_minor_units, expected.net_deposits_minor_units);
    EXPECT_EQ(actual.last_sequence, expected.last_sequence);
}

template<typename Applied>
void expect_step(const jacks::AccountReplayStep& step, std::size_t index,
                 const jacks::DepositState& state, std::int64_t equity) {
    SCOPED_TRACE(index);
    EXPECT_EQ(step.input_index, index);
    ASSERT_TRUE(std::holds_alternative<Applied>(step.applied));
    const auto& applied = std::get<Applied>(step.applied);
    expect_state(applied.state, state);
    EXPECT_EQ(applied.equity.minor_units, equity);
}

void expect_events(const std::vector<jacks::AccountEvent>& actual,
                   const std::vector<jacks::AccountEvent>& expected) {
    ASSERT_EQ(actual.size(), expected.size());
    for (std::size_t i = 0; i < expected.size(); ++i) {
        SCOPED_TRACE(i);
        std::visit([&](const auto& event) {
            using T = std::decay_t<decltype(event)>;
            const auto* observed = std::get_if<T>(&actual[i]);
            ASSERT_NE(observed, nullptr);
            EXPECT_EQ(observed->sequence, event.sequence);
            if constexpr (std::is_same_v<T, jacks::Deposit>) {
                EXPECT_EQ(observed->amount_minor_units, event.amount_minor_units);
            } else if constexpr (std::is_same_v<T, jacks::MarkUpdate>) {
                EXPECT_EQ(observed->mark_minor_units, event.mark_minor_units);
            } else {
                EXPECT_EQ(observed->side, event.side);
                EXPECT_EQ(observed->quantity_units, event.quantity_units);
                EXPECT_EQ(observed->price_minor_units, event.price_minor_units);
                EXPECT_EQ(observed->fee_minor_units, event.fee_minor_units);
            }
        }, expected[i]);
    }
}

std::vector<jacks::AccountEvent> lifecycle_events() {
    return {
        jacks::Deposit{5000, 1},
        jacks::Fill{jacks::FillSide::buy, 4, 1100, 20, 2},
        jacks::MarkUpdate{1200, 3},
        jacks::Fill{jacks::FillSide::sell, 4, 1150, 20, 4}
    };
}

} // namespace

class AccountReplayTest : public ::testing::Test {
protected:
	jacks::DepositState state{
		/* cash_minor_units */		   1000,
		/* quantity_units */		   0,
		/* mark_minor_units */		   10,
		/* net_deposits_minor_units */ 0,
		/* last_sequence */			   0
	};

	std::vector<jacks::AccountEvent> events;
};

TEST_F(AccountReplayTest, InvalidInitialState) {
	state.last_sequence = 1;

	const auto res = replay_account(state, events);

	ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayError>(res));
	ASSERT_EQ(std::get<jacks::AccountReplayError>(res),
				jacks::AccountReplayError::invalid_initial_state);
}

TEST_F(AccountReplayTest, InvalidMarkUpdate) {
	events.push_back(jacks::MarkUpdate{-1, 5});

	const auto res = jacks::replay_account(state, events);

	ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayReport>(res));
	const auto report = std::get<jacks::AccountReplayReport>(res);
	ASSERT_NE(report.failure, std::nullopt);
	const auto failure = *report.failure;
	const auto error = failure.error;
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(error));
	ASSERT_EQ(std::get<jacks::MarkError>(error), jacks::MarkError::invalid_mark);
}

TEST_F(AccountReplayTest, InvalidDeposit) {
	events.push_back(jacks::Deposit{INT64_MAX, 5});

	const auto res = jacks::replay_account(state, events);

	ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayReport>(res));
	const auto report = std::get<jacks::AccountReplayReport>(res);
	ASSERT_NE(report.failure, std::nullopt);
	const auto failure = *report.failure;
	const auto error = failure.error;
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(error));
	ASSERT_EQ(std::get<jacks::DepositError>(error), jacks::DepositError::overflow);
}

TEST_F(AccountReplayTest, InvalidFill) {
	events.push_back(jacks::Fill{jacks::FillSide::buy, 100, 20, 0, 5});

	const auto res = jacks::replay_account(state, events);

	ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayReport>(res));
	const auto report = std::get<jacks::AccountReplayReport>(res);
	ASSERT_NE(report.failure, std::nullopt);
	const auto failure = *report.failure;
	const auto error = failure.error;
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(error));
	ASSERT_EQ(std::get<jacks::FillError>(error), jacks::FillError::insufficient_cash);
}

TEST_F(AccountReplayTest, InvalidPrefixValidSuffix) {
	state.quantity_units = 1;
	events.push_back(jacks::MarkUpdate{INT64_MAX, 5});
	events.push_back(jacks::MarkUpdate{56, 6});
	const auto opening = state;
	const auto original_events = events;

	const auto res = jacks::replay_account(state, events);

	ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayReport>(res));
	const auto report = std::get<jacks::AccountReplayReport>(res);
	ASSERT_NE(report.failure, std::nullopt);
	const auto failure = *report.failure;
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(failure.error));
	ASSERT_EQ(std::get<jacks::MarkError>(failure.error), jacks::MarkError::overflow);
	EXPECT_EQ(failure.input_index, 0u);
	EXPECT_TRUE(report.accepted.empty());
	expect_state(report.opening_state, opening);
	EXPECT_EQ(report.opening_equity.minor_units, 1010);
	expect_state(report.final_state, opening);
	EXPECT_EQ(report.final_equity.minor_units, 1010);
	expect_state(state, opening);
	expect_events(events, original_events);
}

TEST_F(AccountReplayTest, BasicValid) {
	events.push_back(jacks::Fill{
		jacks::FillSide::buy,
		100,
		10,
		0,
		1
	});
	events.push_back(jacks::MarkUpdate{11, 2});
	events.push_back(jacks::MarkUpdate{12, 3});
	events.push_back(jacks::MarkUpdate{13, 4});
	events.push_back(jacks::MarkUpdate{14, 5});
	events.push_back(jacks::MarkUpdate{15, 6});

	const jacks::DepositState state_cpy(state);
	const std::vector<jacks::AccountEvent> events_cpy(events);

	const auto res = jacks::replay_account(state, events);

	ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayReport>(res));
	const auto report = std::get<jacks::AccountReplayReport>(res);
	ASSERT_EQ(report.opening_state.cash_minor_units, 1000);
	ASSERT_EQ(report.opening_state.quantity_units, 0);
	ASSERT_NE(report.opening_state.mark_minor_units, std::nullopt);
	ASSERT_EQ(*report.opening_state.mark_minor_units, 10);
	ASSERT_EQ(report.opening_state.net_deposits_minor_units, 0);
	ASSERT_EQ(report.opening_state.last_sequence, 0);
	ASSERT_EQ(report.opening_equity.minor_units, 1000);

	ASSERT_EQ(report.final_state.cash_minor_units, 0);
	ASSERT_EQ(report.final_state.quantity_units, 100);
	ASSERT_NE(report.final_state.mark_minor_units, std::nullopt);
	ASSERT_EQ(*report.final_state.mark_minor_units, 15);
	ASSERT_EQ(report.final_state.net_deposits_minor_units, 0);
	ASSERT_EQ(report.final_state.last_sequence, 6);
	ASSERT_EQ(report.final_equity.minor_units, 1500);

	ASSERT_EQ(state_cpy.cash_minor_units, state.cash_minor_units);
	ASSERT_EQ(state_cpy.quantity_units, state.quantity_units);
	ASSERT_NE(state_cpy.mark_minor_units, std::nullopt);
	ASSERT_NE(state.mark_minor_units, std::nullopt);
	ASSERT_EQ(*state_cpy.mark_minor_units, *state.mark_minor_units);
	ASSERT_EQ(state_cpy.net_deposits_minor_units, state.net_deposits_minor_units);
	ASSERT_EQ(state_cpy.last_sequence, state.last_sequence);

	expect_events(events, events_cpy);
}

TEST_F(AccountReplayTest, BasicValidFillMarkDeposit) {
	events.push_back(jacks::Fill{
		jacks::FillSide::buy,
		100,
		10,
		0,
		1
	});
	events.push_back(jacks::MarkUpdate{11, 2});
	events.push_back(jacks::MarkUpdate{12, 3});
	events.push_back(jacks::MarkUpdate{13, 4});
	events.push_back(jacks::Deposit{1000, 5});
	events.push_back(jacks::MarkUpdate{14, 6});
	events.push_back(jacks::MarkUpdate{15, 7});

	const auto res = jacks::replay_account(state, events);

	ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayReport>(res));
	const auto report = std::get<jacks::AccountReplayReport>(res);
	ASSERT_EQ(report.opening_state.cash_minor_units, 1000);
	ASSERT_EQ(report.opening_state.quantity_units, 0);
	ASSERT_NE(report.opening_state.mark_minor_units, std::nullopt);
	ASSERT_EQ(*report.opening_state.mark_minor_units, 10);
	ASSERT_EQ(report.opening_state.net_deposits_minor_units, 0);
	ASSERT_EQ(report.opening_state.last_sequence, 0);
	ASSERT_EQ(report.opening_equity.minor_units, 1000);

	ASSERT_EQ(report.final_state.cash_minor_units, 1000);
	ASSERT_EQ(report.final_state.quantity_units, 100);
	ASSERT_NE(report.final_state.mark_minor_units, std::nullopt);
	ASSERT_EQ(*report.final_state.mark_minor_units, 15);
	ASSERT_EQ(report.final_state.net_deposits_minor_units, 1000);
	ASSERT_EQ(report.final_state.last_sequence, 7);
	ASSERT_EQ(report.final_equity.minor_units, 2500);
}

TEST_F(AccountReplayTest, ValidEmptyEvents) {
	const auto res = jacks::replay_account(state, events);

	ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayReport>(res));
	const auto report = std::get<jacks::AccountReplayReport>(res);
	EXPECT_TRUE(report.accepted.empty());
	EXPECT_FALSE(report.failure.has_value());
	expect_state(report.opening_state, state);
	EXPECT_EQ(report.opening_equity.minor_units, 1000);
	ASSERT_EQ(report.opening_state.cash_minor_units, report.final_state.cash_minor_units);
	ASSERT_EQ(report.opening_state.quantity_units, report.final_state.quantity_units);
	ASSERT_NE(report.opening_state.mark_minor_units, std::nullopt);
	ASSERT_NE(report.final_state.mark_minor_units, std::nullopt);
	ASSERT_EQ(*report.opening_state.mark_minor_units, *report.final_state.mark_minor_units);
	ASSERT_EQ(report.opening_state.net_deposits_minor_units,
			  report.final_state.net_deposits_minor_units);
	ASSERT_EQ(report.opening_state.net_deposits_minor_units, 0);
	ASSERT_EQ(report.opening_state.last_sequence, report.final_state.last_sequence);
	ASSERT_EQ(report.opening_state.last_sequence, 0);
	ASSERT_EQ(report.opening_equity.minor_units, report.final_equity.minor_units);
}

TEST(AccountReplay, CompleteResearchLifecycle) {
    const jacks::DepositState opening{10000, 0, 1000, 0, 0};
    const auto events = lifecycle_events();
    const auto original_events = events;
    const auto result = jacks::replay_account(opening, events);
    ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayReport>(result));
    const auto& report = std::get<jacks::AccountReplayReport>(result);
    EXPECT_FALSE(report.failure.has_value());
    expect_state(report.opening_state, {10000, 0, 1000, 0, 0});
    EXPECT_EQ(report.opening_equity.minor_units, 10000);
    ASSERT_EQ(report.accepted.size(), 4u);
    // Independent literal balances from RES-005, not calculated by the code under test.
    expect_step<jacks::DepositApplied>(report.accepted[0], 0, {15000, 0, 1000, 5000, 1}, 15000);
    expect_step<jacks::FillApplied>(report.accepted[1], 1, {10580, 4, 1000, 5000, 2}, 14580);
    expect_step<jacks::MarkApplied>(report.accepted[2], 2, {10580, 4, 1200, 5000, 3}, 15380);
    expect_step<jacks::FillApplied>(report.accepted[3], 3, {15160, 0, 1200, 5000, 4}, 15160);
    expect_state(report.final_state, {15160, 0, 1200, 5000, 4});
    EXPECT_EQ(report.final_equity.minor_units, 15160);
    expect_state(opening, {10000, 0, 1000, 0, 0});
    expect_events(events, original_events);
}

TEST(AccountReplay, RejectionRetainsAcceptedPrefixAndIgnoresValidSuffix) {
    const jacks::DepositState opening{10000, 0, 1000, 0, 0};
    auto events = lifecycle_events();
    events[2] = jacks::MarkUpdate{1200, 2}; // Duplicates the accepted fill's sequence.
    const auto original_events = events;
    const auto result = jacks::replay_account(opening, events);
    ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayReport>(result));
    const auto& report = std::get<jacks::AccountReplayReport>(result);
    ASSERT_TRUE(report.failure.has_value());
    EXPECT_EQ(report.failure->input_index, 2u);
    ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(report.failure->error));
    EXPECT_EQ(std::get<jacks::MarkError>(report.failure->error), jacks::MarkError::invalid_sequence);
    ASSERT_EQ(report.accepted.size(), 2u);
    expect_step<jacks::DepositApplied>(report.accepted[0], 0, {15000, 0, 1000, 5000, 1}, 15000);
    expect_step<jacks::FillApplied>(report.accepted[1], 1, {10580, 4, 1000, 5000, 2}, 14580);
    expect_state(report.opening_state, opening);
    EXPECT_EQ(report.opening_equity.minor_units, 10000);
    expect_state(report.final_state, {10580, 4, 1000, 5000, 2});
    EXPECT_EQ(report.final_equity.minor_units, 14580);
    expect_state(opening, {10000, 0, 1000, 0, 0});
    expect_events(events, original_events);
}

TEST(AccountReplay, RejectsInvalidOpeningFundingOrValuationEvenWithoutEvents) {
    const jacks::DepositState invalid_openings[] = {
        {1000, 0, 10, 1, 0},
        {1000, 1, std::nullopt, 0, 0}
    };
    for (const auto& opening : invalid_openings) {
        SCOPED_TRACE(opening.net_deposits_minor_units);
        const auto result = jacks::replay_account(opening, {});
        ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayError>(result));
        EXPECT_EQ(std::get<jacks::AccountReplayError>(result),
                  jacks::AccountReplayError::invalid_initial_state);
    }
}

TEST(AccountReplay, EmptyCashOnlyRunAllowsAbsentMark) {
    const jacks::DepositState opening{1000, 0, std::nullopt, 0, 0};
    const auto result = jacks::replay_account(opening, {});
    ASSERT_TRUE(std::holds_alternative<jacks::AccountReplayReport>(result));
    const auto& report = std::get<jacks::AccountReplayReport>(result);
    EXPECT_FALSE(report.failure.has_value());
    EXPECT_TRUE(report.accepted.empty());
    expect_state(report.opening_state, opening);
    expect_state(report.final_state, opening);
    EXPECT_EQ(report.opening_equity.minor_units, 1000);
    EXPECT_EQ(report.final_equity.minor_units, 1000);
}
