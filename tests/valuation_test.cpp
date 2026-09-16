#include "jacks/valuation.hpp"
#include <gtest/gtest.h>
#include <optional>
#include <variant>

// Cash: 10000; Quantity: 40; Mark: std::nullopt;
// Should return ValuationResult evaluating to ErrorCode::missing_mark
TEST(Valuation, CalculateEquityMissingMark) {
	const auto res = jacks::calculate_equity(10000, 40, std::nullopt);
	ASSERT_TRUE(std::holds_alternative<jacks::ErrorCode>(res));
	ASSERT_EQ(std::get<jacks::ErrorCode>(res), jacks::ErrorCode::missing_mark);
}
 
// Cash: -1000; Quantity: 50; Mark: 40;
// Should return ValuationResult evaluating to ErrorCode::invalid_input
TEST(Valuation, CalculateEquityInvalidCash) {
	const auto res = jacks::calculate_equity(-1000, 50, 40);
	ASSERT_TRUE(std::holds_alternative<jacks::ErrorCode>(res));
	ASSERT_EQ(std::get<jacks::ErrorCode>(res), jacks::ErrorCode::invalid_input);
}

// Cash: 15000; Quantity: -1; Mark: 240;
// Should return ValuationResult evaluating to ErrorCode::invalid_input
TEST(Valuation, CalculateEquityInvalidQuantity) {
	const auto res = jacks::calculate_equity(15000, -1, 240);
	ASSERT_TRUE(std::holds_alternative<jacks::ErrorCode>(res));
	ASSERT_EQ(std::get<jacks::ErrorCode>(res), jacks::ErrorCode::invalid_input);
}

// Cash: 1000; Quantity: 100; Mark: 24;
// Should return ValuationResult evaluating to Equity->3400
TEST(Valuation, CalculateEquityValidCalculation) {
	const auto res = jacks::calculate_equity(1000, 100, 24);
	ASSERT_TRUE(std::holds_alternative<jacks::Equity>(res));
	ASSERT_EQ(std::get<jacks::Equity>(res).minor_units, 3400);
}

// Cash: 100; Quantity: 1; Mark: -17;
// Should return ValuationResult evaluating to ErrorCode::invalid_input
TEST(Valuation, CalculateEquityInvalidMark) {
	const auto res = jacks::calculate_equity(100, 1, -17);
	ASSERT_TRUE(std::holds_alternative<jacks::ErrorCode>(res));
	ASSERT_EQ(std::get<jacks::ErrorCode>(res), jacks::ErrorCode::invalid_input);
}

// Cash: 3; Quantity: INT64_MAX / 3; Mark: 3
// Should return ValuationResult evaluating to ErrorCode::overflow
TEST(Valuation, CalculateEquityOverflow) {
	const auto res = jacks::calculate_equity(3, INT64_MAX / 3, 3);
	ASSERT_TRUE(std::holds_alternative<jacks::ErrorCode>(res));
	ASSERT_EQ(std::get<jacks::ErrorCode>(res), jacks::ErrorCode::overflow);
}

// Cash: 1; Quantity: INT64_MAX / 3; Mark: 3
// Should return ValuationResult evaluating to Equity->INT64_MAX
TEST(Valuation, CalculateEquityAlmostOverflow) {
	const auto res = jacks::calculate_equity(1, INT64_MAX / 3, 3);
	ASSERT_TRUE(std::holds_alternative<jacks::Equity>(res));
	ASSERT_EQ(std::get<jacks::Equity>(res).minor_units, INT64_MAX);
}

// Cash: 100; Quantity: 0; Mark: std::nullopt;
// Should return ValuationResult evaluating to Equity->100
TEST(Valuation, CalculateEquityOnlyCash) {
	const auto res = jacks::calculate_equity(100, 0, std::nullopt);
	ASSERT_TRUE(std::holds_alternative<jacks::Equity>(res));
	ASSERT_EQ(std::get<jacks::Equity>(res).minor_units, 100);
}

// Cash: -1; Quantity: 1; Mark: std::nullopt;
// Should return ValuationResult evaluating to ErrorCode::invalid_input
TEST(Valuation, CalculateEquityInvalidCashMissingMark) {
	const auto res = jacks::calculate_equity(-1, 1, std::nullopt);
	ASSERT_TRUE(std::holds_alternative<jacks::ErrorCode>(res));
	ASSERT_EQ(std::get<jacks::ErrorCode>(res), jacks::ErrorCode::invalid_input);
}

// Cash: 1; Quantity: INT64_MAX / 2; Mark: 3;
// Should return ValuationResult evaluating to ErrorCode::overflow
TEST(Valuation, CalculateEquityMultiplicationOverflow) {
	const auto res = jacks::calculate_equity(1, INT64_MAX / 2, 3);
	ASSERT_TRUE(std::holds_alternative<jacks::ErrorCode>(res));
	ASSERT_EQ(std::get<jacks::ErrorCode>(res), jacks::ErrorCode::overflow);
}
