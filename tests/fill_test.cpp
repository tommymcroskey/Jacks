#include "jacks/fill.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using FillSignature = jacks::FillResult (*)(const jacks::DepositState&, const jacks::Fill&);
static_assert(std::is_same_v<decltype(&jacks::apply_fill), FillSignature>);

class FillTest : public ::testing::Test {
protected:
	jacks::DepositState state{
		/* cash_minor_units */		   4000,
		/* quantity_units */		   20,
		/* mark_minor_units */		   250,
		/* net_deposits_minor_units */ 7000,
		/* last_sequence */			   6
	};
	
	jacks::Fill fill_buy{
		/* side */ jacks::FillSide::buy,
		/* quantity_units */	8,
		/* price_minor_units */ 255,
		/* fee_minor_units */	16,
		/* sequence */			8
	};

	jacks::Fill fill_sell{
		/* side */ jacks::FillSide::sell,
		/* quantity_units */	12,
		/* price_minor_units */ 256,
		/* fee_minor_units */	18,
		/* sequence */			9
	};
};

// precedence: invalid_state, invalid_fill, invalid_sequence,
// missing_mark, insufficient_quantity, overflow, insufficient_cash

// invalid_state

TEST_F(FillTest, FillStateInvalidState) {
	state.quantity_units = -10;
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_state);
}

TEST_F(FillTest, FillStateMissingMark) {
	state.mark_minor_units = std::nullopt;
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_state);
}

TEST_F(FillTest, FillStateOverflow) {
	// INT64_MAX / 15 * 20 (quantity_units) > INT64_MAX -> overflow
	state.mark_minor_units = INT64_MAX / 15;
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_state);
}

TEST_F(FillTest, FillStateNegativeDeposits) {
	state.net_deposits_minor_units = -1;
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_state);
}

TEST_F(FillTest, FillStateNegativeSequence) {
	state.last_sequence = -1;
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_state);
}

// invalid_fill

TEST_F(FillTest, FillInvalidSide) {
	fill_buy.side = static_cast<jacks::FillSide>(42);
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_fill);
}

TEST_F(FillTest, FillNegativeQuantity) {
	fill_sell.quantity_units = -1;
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_fill);
}

TEST_F(FillTest, FillZeroQuantity) {
	fill_buy.quantity_units = 0;
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_fill);
}

TEST_F(FillTest, FillNegativePrice) {
	fill_sell.price_minor_units = -1;
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_fill);
}

TEST_F(FillTest, FillZeroPrice) {
	fill_buy.price_minor_units = 0;
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_fill);
}

TEST_F(FillTest, FillNegativeFee) {
	fill_sell.fee_minor_units = -1;
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_fill);
}

// invalid_sequence

TEST_F(FillTest, FillNegativeSequence) {
	fill_buy.sequence = -1;
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_sequence);
}

TEST_F(FillTest, FillLTSequence) {
	fill_sell.sequence = state.last_sequence - 1; // 5
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_sequence);
}

TEST_F(FillTest, FillEqualSequence) {
	fill_buy.sequence = state.last_sequence; // 6
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::invalid_sequence);
}

// missing_mark

TEST_F(FillTest, FillBuyAbsentValuation) {
	state.quantity_units = 0;
	state.mark_minor_units = std::nullopt;
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::missing_mark);
}

TEST_F(FillTest, FillSellAbsentValuation) {
	state.quantity_units = 0;
	state.mark_minor_units = std::nullopt;
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::missing_mark);
}

// insufficient_quantity

TEST_F(FillTest, FillSellBoundaryGreaterThanQuantity) {
	fill_sell.quantity_units = state.quantity_units + 1; // 21
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::insufficient_quantity);
}

// include sell inside boundary in valid

// overflow

TEST_F(FillTest, FillGrossOverflow) {
	state.cash_minor_units = 0;
	state.mark_minor_units = 1;
	fill_sell.quantity_units = 20;
	fill_sell.price_minor_units = INT64_MAX / 10;
	fill_sell.fee_minor_units = INT64_MAX;
	// final quantity 0, final cash INT64_MAX (0 + 2 * INT64_MAX - INT64_MAX),
	// final equity: INT64_MAX (INT64_MAX + 0 * 1)
	// therefore, gross overflow isolated in this case
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::overflow);
}

TEST_F(FillTest, FillFinalQuantityOverflow) {
	// final quantity = (buy) fill_quantity + state_quantity
	// 					(sell) fill_quantity - state_quantity
	// higher precedent test FillSellBoundaryGreater than quantity
	// already establishes sell operation clamped to zero, so buy
	// is only concern
	state.quantity_units = INT64_MAX;
	state.mark_minor_units = 0;
	fill_buy.quantity_units = 1;
	fill_buy.price_minor_units = 1;
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::overflow);
}

// insufficient_cash

TEST_F(FillTest, FillFinalCashNegativeBuy) {
	// final cash = (buy) cash - gross - fee
	state.cash_minor_units = INT64_MAX;
	state.quantity_units = 0;
	state.mark_minor_units = 0;
	fill_buy.quantity_units = 15;
	fill_buy.price_minor_units = INT64_MAX / 15;
	fill_buy.fee_minor_units = INT64_MAX - 15 * (INT64_MAX / 15) + 1;
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::insufficient_cash);
}

TEST_F(FillTest, FillFinalCashNegativeSell) {
	// final cash = (sell) cash + gross - fee
	state.cash_minor_units = 0;
	state.quantity_units = 1;
	fill_sell.quantity_units = 1;
	fill_sell.price_minor_units = 1;
	fill_sell.fee_minor_units = 2;
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::insufficient_cash);
}

// overflow

TEST_F(FillTest, FillFinalCashOverflow) {
	// final cash = (sell) cash + gross - fee
	state.cash_minor_units = INT64_MAX;
	state.quantity_units = 1;
	state.mark_minor_units = 0;
	fill_sell.quantity_units = 1;
	fill_sell.price_minor_units = 1;
	fill_sell.fee_minor_units = 0;
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::overflow);
}

TEST_F(FillTest, FillCandidateEquityOverflow) {
	// candidate equity = quantity * mark + cash
	state.cash_minor_units = INT64_MAX - (INT64_MAX / 10) + 11;
	state.quantity_units = 0;
	state.mark_minor_units = INT64_MAX / 10;
	fill_buy.quantity_units = 10;
	fill_buy.price_minor_units = 10;
	fill_buy.fee_minor_units = 0;
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillError>(res));
	ASSERT_EQ(std::get<jacks::FillError>(res), jacks::FillError::overflow);
}

// valid

TEST_F(FillTest, FillValidBuy) {
	const auto res = jacks::apply_fill(state, fill_buy);
	ASSERT_TRUE(std::holds_alternative<jacks::FillApplied>(res));
	const auto ds = std::get<jacks::FillApplied>(res).state;
	const auto eq = std::get<jacks::FillApplied>(res).equity;
	ASSERT_EQ(ds.cash_minor_units, 1944);
	ASSERT_EQ(ds.quantity_units, 28);
	ASSERT_NE(ds.mark_minor_units, std::nullopt);
	ASSERT_EQ(*ds.mark_minor_units, 250);
	ASSERT_EQ(ds.net_deposits_minor_units, 7000);
	ASSERT_EQ(ds.last_sequence, 8);
	ASSERT_EQ(eq.minor_units, 8944);
}

TEST_F(FillTest, FillValidSell) {
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillApplied>(res));
	const auto ds = std::get<jacks::FillApplied>(res).state;
	const auto eq = std::get<jacks::FillApplied>(res).equity;
	ASSERT_EQ(ds.cash_minor_units, 7054);
	ASSERT_EQ(ds.quantity_units, 8);
	ASSERT_NE(ds.mark_minor_units, std::nullopt);
	ASSERT_EQ(*ds.mark_minor_units, 250);
	ASSERT_EQ(ds.net_deposits_minor_units, 7000);
	ASSERT_EQ(ds.last_sequence, 9);
	ASSERT_EQ(eq.minor_units, 9054);
}

TEST_F(FillTest, FillValidCashPlusGrossExceedsMax) {
	state.cash_minor_units = INT64_MAX;
	state.quantity_units = 1;
	state.mark_minor_units = 0;
	fill_sell.quantity_units = 1;
	fill_sell.price_minor_units = 1;
	fill_sell.fee_minor_units = 1;
	const auto res = jacks::apply_fill(state, fill_sell);
	ASSERT_TRUE(std::holds_alternative<jacks::FillApplied>(res));
	const auto ds = std::get<jacks::FillApplied>(res).state;
	const auto eq = std::get<jacks::FillApplied>(res).equity;
	ASSERT_EQ(ds.cash_minor_units, INT64_MAX);
	ASSERT_EQ(ds.quantity_units, 0);
	ASSERT_NE(ds.mark_minor_units, std::nullopt);
	ASSERT_EQ(*ds.mark_minor_units, 0);
	ASSERT_EQ(ds.net_deposits_minor_units, 7000);
	ASSERT_EQ(ds.last_sequence, 9);
	ASSERT_EQ(eq.minor_units, INT64_MAX);
}
