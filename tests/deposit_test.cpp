#include "jacks/deposit.hpp"
#include <gtest/gtest.h>

#include <type_traits>

// Infrastructure/interface check only; it establishes no deposit behavior.
using DepositSignature = jacks::DepositResult (*)(
    const jacks::DepositState&, const jacks::Deposit&);
static_assert(std::is_same_v<decltype(&jacks::apply_deposit), DepositSignature>);

// TARGETED INVALID TESTS

// can compress out of range for cash, quantity, and mark down to
// invalid_input, missing_mark, overflow since calculate_equity owns
// these inputs' verification

// equity_calculation returns errorcode::invalid_input -> invalid_state
TEST(Deposit, DepositInvalidInputState) {
	const jacks::DepositState current{
		/* cash_minor_units */		   -1,
		/* quantity_units */		   100,
		/* mark_minor_units */		   10,
		/* net_deposits_minor_units */ 0,
		/* last_sequence */ 		   0
	};
	const jacks::Deposit deposit{100, 1};

	const auto res = jacks::apply_deposit({-1, 100, 10, 0, 0}, {100, 1});
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_state);
}

//  equity_calculation returns errorcode::missing_mark -> invalid_state
TEST(Deposit, DepositMissingMarkState) {
	const jacks::DepositState current{
		/* cash_minor_units */		   155,
		/* quantity_units */		   22000,
		/* mark_minor_units */		   std::nullopt,
		/* net_deposits_minor_units */ 1000,
		/* last_sequence */ 		   10
	};
	const jacks::Deposit deposit{33800, 45};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_state);
}

// equity_calculation returns errorcode::overflow -> invalid_state
TEST(Deposit, DepositOverflowState) {
	const jacks::DepositState current{
		/* cash_minor_units */		   155,
		/* quantity_units */		   INT64_MAX / 3,
		/* mark_minor_units */		   4,
		/* net_deposits_minor_units */ 1000,
		/* last_sequence */ 		   10
	};
	const jacks::Deposit deposit{320, 166};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_state);
}

// valid equity_calculation result, negative net_deposits -> invalid_state
TEST(Deposit, DepositNegativeNetDeposits) {
	const jacks::DepositState current{
		/* cash_minor_units */		   4020,
		/* quantity_units */		   8,
		/* mark_minor_units */		   99,
		/* net_deposits_minor_units */ -40,
		/* last_sequence */ 		   1190
	};
	const jacks::Deposit deposit{18, 55};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_state);
}

// valid equity_calculation result, negative sequence in state -> invalid_state
TEST(Deposit, DepositNegativeStateSequence) {
	const jacks::DepositState current{
		/* cash_minor_units */		   8851,
		/* quantity_units */		   42,
		/* mark_minor_units */		   5723,
		/* net_deposits_minor_units */ 1999,
		/* last_sequence */ 		   -99
	};
	const jacks::Deposit deposit{320, 166};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_state);
}

// valid state, zero deposit amount -> invalid_amount
TEST(Deposit, DepositValidStateZeroDeposit) {
	const jacks::DepositState current{
		/* cash_minor_units */		   624,
		/* quantity_units */		   301,
		/* mark_minor_units */		   42,
		/* net_deposits_minor_units */ 50,
		/* last_sequence */ 		   1
	};
	const jacks::Deposit deposit{0, 2};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_amount);
}

// valid state, negative deposit amount -> invalid_amount
TEST(Deposit, DepositValidStateNegativeDeposit) {
	const jacks::DepositState current{
		/* cash_minor_units */		   77,
		/* quantity_units */		   66,
		/* mark_minor_units */		   55,
		/* net_deposits_minor_units */ 44,
		/* last_sequence */ 		   33 
	};
	const jacks::Deposit deposit{-22, 111};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_amount);
}

// valid state, negative deposit sequence -> invalid_sequence
TEST(Deposit, DepositValidStateNegativeDepositSequence) {
	const jacks::DepositState current{
		/* cash_minor_units */		   50000,
		/* quantity_units */		   10,
		/* mark_minor_units */		   9800,
		/* net_deposits_minor_units */ 100000,
		/* last_sequence */ 		   199999
	};
	const jacks::Deposit deposit{8800, -200000};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_sequence);
}

// valid state, deposit sequence less than last sequence -> invalid_sequence
TEST(Deposit, DepositValidStateLTDepositSequence) {
	const jacks::DepositState current{
		/* cash_minor_units */		   220000,
		/* quantity_units */		   11,
		/* mark_minor_units */		   7700,
		/* net_deposits_minor_units */ 48000,
		/* last_sequence */ 		   26 
	};
	const jacks::Deposit deposit{1000000, 25};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_sequence);
}

// valid state, deposit sequence equal to last sequence -> invalid_sequence
TEST(Deposit, DepositValidStateEqualDepositSequence) {
	const jacks::DepositState current{
		/* cash_minor_units */		   104003,
		/* quantity_units */		   24,
		/* mark_minor_units */		   10000,
		/* net_deposits_minor_units */ 6600,
		/* last_sequence */ 		   18
	};
	const jacks::Deposit deposit{10000, 18};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_sequence);
}

// valid state and deposit, cash overflow -> overflow
TEST(Deposit, DepositCashOverflow) {
	const jacks::DepositState current{
		/* cash_minor_units */		   199,
		/* quantity_units */		   5839,
		/* mark_minor_units */		   1124,
		/* net_deposits_minor_units */ 123481,
		/* last_sequence */ 		   99
	};
	const jacks::Deposit deposit{INT64_MAX, 100};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::overflow);
}

// valid state and deposit, net deposits overflow -> overflow
TEST(Deposit, DepositNetDepositsOverflow) {
	const jacks::DepositState current{
		/* cash_minor_units */		   245,
		/* quantity_units */		   2,
		/* mark_minor_units */		   1199,
		/* net_deposits_minor_units */ 306,
		/* last_sequence */ 		   12
	};
	const jacks::Deposit deposit{INT64_MAX - 245, 13};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::overflow);
}

// valid state and deposit, resulting equity overflow -> overflow
TEST(Deposit, DepositEquityOverflow) {
	const jacks::DepositState current{
		/* cash_minor_units */		   INT64_MAX / 5,
		/* quantity_units */		   4,
		/* mark_minor_units */		   INT64_MAX / 5,
		/* net_deposits_minor_units */ 100000,
		/* last_sequence */ 		   10
	};
	const jacks::Deposit deposit{88, 166};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::overflow);
}

// ERROR PRECEDENCE TESTS

// order of precedence:
// invalid_state, invalid_amount, invalid_sequence, overflow 

TEST(Deposit, DepositInvalidStateInvalidAmount) {
	const jacks::DepositState current{
		/* cash_minor_units */		   -1,
		/* quantity_units */		   44,
		/* mark_minor_units */		   4,
		/* net_deposits_minor_units */ 4444,
		/* last_sequence */ 		   44
	};
	const jacks::Deposit deposit{-1, 444};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_state);
}

TEST(Deposit, DepositInvalidStateInvalidSequence) {
	const jacks::DepositState current{
		/* cash_minor_units */		   100,
		/* quantity_units */		   100000,
		/* mark_minor_units */		   10000,
		/* net_deposits_minor_units */ -9999,
		/* last_sequence */ 		   10
	};
	const jacks::Deposit deposit{1000, 9};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_state);
}

TEST(Deposit, DepositInvalidStateOverflow) {
	const jacks::DepositState current{
		/* cash_minor_units */		   8577,
		/* quantity_units */		   99,
		/* mark_minor_units */		   std::nullopt,
		/* net_deposits_minor_units */ 20000,
		/* last_sequence */ 		   8699
	};
	const jacks::Deposit deposit{INT64_MAX - 5000, 166};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_state);
}

TEST(Deposit, DepositInvalidAmountInvalidSequence) {
	const jacks::DepositState current{
		/* cash_minor_units */		   350,
		/* quantity_units */		   1,
		/* mark_minor_units */		   5831,
		/* net_deposits_minor_units */ 6047,
		/* last_sequence */ 		   100
	};
	const jacks::Deposit deposit{0, INT64_MIN};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_amount);
}

TEST(Deposit, DepositInvalidSequenceOverflow) {
	const jacks::DepositState current{
		/* cash_minor_units */		   12,
		/* quantity_units */		   0,
		/* mark_minor_units */		   std::nullopt,
		/* net_deposits_minor_units */ 18,
		/* last_sequence */ 		   1
	};
	const jacks::Deposit deposit{INT_MAX, -1};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositError>(res));
	ASSERT_EQ(std::get<jacks::DepositError>(res), jacks::DepositError::invalid_sequence);
}

// VALID RESULT TESTS

TEST(Deposit, DepositValid) {
	const jacks::DepositState current{
		/* cash_minor_units */		   6500,
		/* quantity_units */		   6,
		/* mark_minor_units */		   10000,
		/* net_deposits_minor_units */ 6300,
		/* last_sequence */ 		   1
	};
	const jacks::Deposit deposit{12000, 2};

	const auto res = jacks::apply_deposit(current, deposit);
	ASSERT_TRUE(std::holds_alternative<jacks::DepositApplied>(res));
	const auto dp = std::get<jacks::DepositApplied>(res);
	ASSERT_EQ(dp.equity.minor_units, 78500);
	ASSERT_EQ(dp.state.cash_minor_units, 18500);
	ASSERT_EQ(dp.state.net_deposits_minor_units, 18300);
	ASSERT_EQ(dp.state.last_sequence, 2);
	ASSERT_EQ(dp.state.quantity_units, 6);
	ASSERT_EQ(*dp.state.mark_minor_units, 10000);
}
