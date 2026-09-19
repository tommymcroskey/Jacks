#include "jacks/mark.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using MarkSignature = jacks::MarkResult (*)(const jacks::DepositState&, const jacks::MarkUpdate&);
static_assert(std::is_same_v<decltype(&jacks::apply_mark), MarkSignature>);

// Error Precedence: invalid_state, invalid_mark, invalid_sequence, overflow

// invalid_state

// encompasses negative cash, negative quantity,
// missing mark, and overflow
TEST(Mark, MarkEquityInvalidState) {
	const jacks::DepositState state{
		/* cash_minor_units */		   -200,
		/* quantity_units */		   15,
		/* mark_minor_units */ 		   20,
		/* net_deposits_minor_units */ 89,
		/* last_sequence */ 		   18
	};
	const jacks::MarkUpdate mark_update{100, 56};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_state);
}

TEST(Mark, MarkEquityMissingMark) {
	const jacks::DepositState state{
		/* cash_minor_units */		   109,
		/* quantity_units */		   8,
		/* mark_minor_units */ 		   std::nullopt,
		/* net_deposits_minor_units */ 77,
		/* last_sequence */ 		   34
	};
	const jacks::MarkUpdate mark_update{98, 38};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_state);
}

TEST(Mark, MarkEquityOverflow) {
	const jacks::DepositState state{
		/* cash_minor_units */		   INT64_MAX,
		/* quantity_units */		   1,
		/* mark_minor_units */ 		   1,
		/* net_deposits_minor_units */ INT64_MAX,
		/* last_sequence */ 		   9999
	};
	const jacks::MarkUpdate mark_update{2, 10000};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_state);
}

TEST(Mark, MarkNegativeStateDeposits) {
	const jacks::DepositState state{
		/* cash_minor_units */		   655,
		/* quantity_units */		   6,
		/* mark_minor_units */ 		   9000,
		/* net_deposits_minor_units */ -1,
		/* last_sequence */ 		   4
	};
	const jacks::MarkUpdate mark_update{9002, 6};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_state);
}

TEST(Mark, MarkNegativeStateLastSequence) {
	const jacks::DepositState state{
		/* cash_minor_units */		   20,
		/* quantity_units */		   30,
		/* mark_minor_units */ 		   40,
		/* net_deposits_minor_units */ 50,
		/* last_sequence */ 		   -60
	};
	const jacks::MarkUpdate mark_update{70, 80};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_state);
}

// invalid_mark

TEST(Mark, MarkNegativeMark) {
	const jacks::DepositState state{
		/* cash_minor_units */		   97700,
		/* quantity_units */		   2,
		/* mark_minor_units */ 		   99,
		/* net_deposits_minor_units */ 188299,
		/* last_sequence */ 		   3000
	};
	const jacks::MarkUpdate mark_update{-100, 56};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_mark);
}

// invalid_sequence

TEST(Mark, MarkNegativeSequence) {
	const jacks::DepositState state{
		/* cash_minor_units */		   18900,
		/* quantity_units */		   113,
		/* mark_minor_units */ 		   177,
		/* net_deposits_minor_units */ 19030,
		/* last_sequence */ 		   13
	};
	const jacks::MarkUpdate mark_update{152, -14};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_sequence);
}

TEST(Mark, MarkLTSequence) {
	const jacks::DepositState state{
		/* cash_minor_units */		   298347,
		/* quantity_units */		   102938,
		/* mark_minor_units */ 		   342901,
		/* net_deposits_minor_units */ 580913,
		/* last_sequence */ 		   1000000
	};
	const jacks::MarkUpdate mark_update{518091, 999999};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_sequence);
}

TEST(Mark, MarkEqualSequence) {
	const jacks::DepositState state{
		/* cash_minor_units */		   298,
		/* quantity_units */		   102,
		/* mark_minor_units */ 		   342,
		/* net_deposits_minor_units */ 580,
		/* last_sequence */ 		   0
	};
	const jacks::MarkUpdate mark_update{518, 0};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_sequence);
}

// overflow

TEST(Mark, MarkBoundaryOverflow) {
	const int64_t x = INT64_MAX - (INT64_MAX / 5) * 5 + 1;
	const jacks::DepositState state{
		/* cash_minor_units */		   x,
		/* quantity_units */		   5,
		/* mark_minor_units */ 		   10,
		/* net_deposits_minor_units */ 0,
		/* last_sequence */ 		   2
	};
	const jacks::MarkUpdate mark_update{INT64_MAX / 5, 3};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::overflow);
}

// precedence tests

TEST(Mark, MarkInvalidStateInvalidMark) {
	const jacks::DepositState state{
		/* cash_minor_units */		   -1,
		/* quantity_units */		   99,
		/* mark_minor_units */ 		   22,
		/* net_deposits_minor_units */ 35,
		/* last_sequence */ 		   7
	};
	const jacks::MarkUpdate mark_update{-45, 11};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_state);
}

TEST(Mark, MarkInvalidStateInvalidSequence) {
	const jacks::DepositState state{
		/* cash_minor_units */		   26,
		/* quantity_units */		   -11,
		/* mark_minor_units */ 		   0,
		/* net_deposits_minor_units */ 1248,
		/* last_sequence */ 		   1324
	};
	const jacks::MarkUpdate mark_update{100, 1322};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_state);
}

TEST(Mark, MarkInvalidStateOverflow) {
	const jacks::DepositState state{
		/* cash_minor_units */		   599,
		/* quantity_units */		   15,
		/* mark_minor_units */ 		   2350,
		/* net_deposits_minor_units */ -47,
		/* last_sequence */ 		   34019
	};
	const jacks::MarkUpdate mark_update{INT64_MAX / 15, 99999};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_state);
}

TEST(Mark, MarkInvalidMarkInvalidSequence) {
	const jacks::DepositState state{
		/* cash_minor_units */		   683,
		/* quantity_units */		   3112,
		/* mark_minor_units */ 		   18962,
		/* net_deposits_minor_units */ 566,
		/* last_sequence */ 		   20
	};
	const jacks::MarkUpdate mark_update{-66, -40};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_mark);
}

TEST(Mark, MarkInvalidMarkOverflow) {
	const jacks::DepositState state{
		/* cash_minor_units */		   801,
		/* quantity_units */		   2201,
		/* mark_minor_units */ 		   899,
		/* net_deposits_minor_units */ 1022,
		/* last_sequence */ 		   44
	};
	const jacks::MarkUpdate mark_update{-INT64_MAX, 56};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_mark);
}

TEST(Mark, MarkInvalidSequenceOverflow) {
	const jacks::DepositState state{
		/* cash_minor_units */		   1034785,
		/* quantity_units */		   1091385,
		/* mark_minor_units */ 		   2845780,
		/* net_deposits_minor_units */ 6792948,
		/* last_sequence */ 		   7937480
	};
	const jacks::MarkUpdate mark_update{INT64_MAX / 100, -2};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_sequence);
}

// valid tests

TEST(Mark, MarkValidBoundary) {
	const int64_t x = INT64_MAX - (INT64_MAX / 100) * 100;
	const jacks::DepositState state{
		/* cash_minor_units */		   x,
		/* quantity_units */		   100,
		/* mark_minor_units */ 		   50,
		/* net_deposits_minor_units */ 10000,
		/* last_sequence */ 		   45
	};
	const jacks::MarkUpdate mark_update{INT64_MAX / 100, 46};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkApplied>(res));

	const auto ma = std::get<jacks::MarkApplied>(res);
	ASSERT_EQ(ma.equity.minor_units, INT64_MAX);
	ASSERT_EQ(ma.state.cash_minor_units, x);
	ASSERT_EQ(ma.state.quantity_units, 100);
	ASSERT_EQ(ma.state.mark_minor_units, INT64_MAX / 100);
	ASSERT_EQ(ma.state.net_deposits_minor_units, 10000);
	ASSERT_EQ(ma.state.last_sequence, 46);
}

TEST(Mark, MarkValidZeroMark) {
	const jacks::DepositState state{
		/* cash_minor_units */		   100,
		/* quantity_units */		   1000,
		/* mark_minor_units */ 		   101,
		/* net_deposits_minor_units */ 150,
		/* last_sequence */ 		   93
	};
	const jacks::MarkUpdate mark_update{0, 95};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkApplied>(res));

	const auto ma = std::get<jacks::MarkApplied>(res);
	ASSERT_EQ(ma.equity.minor_units, 100);
	ASSERT_EQ(ma.state.cash_minor_units, 100);
	ASSERT_EQ(ma.state.quantity_units, 1000);
	ASSERT_EQ(ma.state.mark_minor_units, 0);
	ASSERT_EQ(ma.state.net_deposits_minor_units, 150);
	ASSERT_EQ(ma.state.last_sequence, 95);
}

TEST(Mark, MarkValidZeroQuantity) {
	const jacks::DepositState state{
		/* cash_minor_units */		   44,
		/* quantity_units */		   0,
		/* mark_minor_units */ 		   std::nullopt,
		/* net_deposits_minor_units */ 50,
		/* last_sequence */ 		   5
	};
	const jacks::MarkUpdate mark_update{86, 9};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkApplied>(res));

	const auto ma = std::get<jacks::MarkApplied>(res);
	ASSERT_EQ(ma.equity.minor_units, 44);
	ASSERT_EQ(ma.state.cash_minor_units, 44);
	ASSERT_EQ(ma.state.quantity_units, 0);
	ASSERT_EQ(ma.state.mark_minor_units, 86);
	ASSERT_EQ(ma.state.net_deposits_minor_units, 50);
	ASSERT_EQ(ma.state.last_sequence, 9);
}

TEST(Mark, MarkUnchangedMark) {
	const jacks::DepositState state{
		/* cash_minor_units */		   50,
		/* quantity_units */		   1,
		/* mark_minor_units */ 		   109,
		/* net_deposits_minor_units */ 49,
		/* last_sequence */ 		   6
	};
	const jacks::MarkUpdate mark_update{109, 7};
	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkApplied>(res));

	const auto ma = std::get<jacks::MarkApplied>(res);
	ASSERT_EQ(ma.equity.minor_units, 159);
	ASSERT_EQ(ma.state.cash_minor_units, 50);
	ASSERT_EQ(ma.state.quantity_units, 1);
	ASSERT_EQ(ma.state.mark_minor_units, 109);
	ASSERT_EQ(ma.state.net_deposits_minor_units, 49);
	ASSERT_EQ(ma.state.last_sequence, 7);
}

// unchanged state verification

TEST(Mark, MarkValidInitialStateUnchanged) {
	const jacks::DepositState state{
		/* cash_minor_units */		   26,
		/* quantity_units */		   19,
		/* mark_minor_units */ 		   69,
		/* net_deposits_minor_units */ 30,
		/* last_sequence */ 		   1
	};
	const jacks::MarkUpdate mark_update{20, 2};

	const auto state_cpy = state;
	const auto mark_update_cpy = mark_update;

	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkApplied>(res));
	ASSERT_EQ(std::get<jacks::MarkApplied>(res).equity.minor_units, 406);

	ASSERT_EQ(state.cash_minor_units, state_cpy.cash_minor_units);
	ASSERT_EQ(state.quantity_units, state_cpy.quantity_units);
	ASSERT_EQ(state.mark_minor_units, state_cpy.mark_minor_units);
	ASSERT_EQ(state.net_deposits_minor_units, state_cpy.net_deposits_minor_units);
	ASSERT_EQ(state.last_sequence, state_cpy.last_sequence);
	ASSERT_EQ(mark_update.mark_minor_units, mark_update_cpy.mark_minor_units);
	ASSERT_EQ(mark_update.sequence, mark_update_cpy.sequence);
}

TEST(Mark, MarkInvalidInitialStateUnchanged) {
	const jacks::DepositState state{
		/* cash_minor_units */		   8023,
		/* quantity_units */		   2359,
		/* mark_minor_units */ 		   1348,
		/* net_deposits_minor_units */ 1202,
		/* last_sequence */ 		   -1438
	};
	const jacks::MarkUpdate mark_update{5409, 1};

	const auto state_cpy = state;
	const auto mark_update_cpy = mark_update;

	const auto res = jacks::apply_mark(state, mark_update);
	ASSERT_TRUE(std::holds_alternative<jacks::MarkError>(res));
	ASSERT_EQ(std::get<jacks::MarkError>(res), jacks::MarkError::invalid_state);

	ASSERT_EQ(state.cash_minor_units, state_cpy.cash_minor_units);
	ASSERT_EQ(state.quantity_units, state_cpy.quantity_units);
	ASSERT_EQ(state.mark_minor_units, state_cpy.mark_minor_units);
	ASSERT_EQ(state.net_deposits_minor_units, state_cpy.net_deposits_minor_units);
	ASSERT_EQ(state.last_sequence, state_cpy.last_sequence);
	ASSERT_EQ(mark_update.mark_minor_units, mark_update_cpy.mark_minor_units);
	ASSERT_EQ(mark_update.sequence, mark_update_cpy.sequence);
}
