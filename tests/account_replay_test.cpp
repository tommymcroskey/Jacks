#include "jacks/account_replay.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using AccountReplaySignature = jacks::AccountReplayResult (*)(
    const jacks::DepositState&, const std::vector<jacks::AccountEvent>&);
static_assert(std::is_same_v<decltype(&jacks::replay_account), AccountReplaySignature>);

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
