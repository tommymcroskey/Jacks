#include "jacks/valuation.hpp"
#include <gtest/gtest.h>
#include <optional>
#include <variant>

void assume_valuation_err(jacks::ValuationResult& v, jacks::ErrorCode& dest) {
	dest = std::get<jacks::ErrorCode>(v);
}

void assume_valuation_equity(jacks::ValuationResult& v, jacks::Equity& dest) {
	dest = std::get<jacks::Equity>(v);
}

// Cash: 10000; Quantity: 40; Mark: std::nullopt;
// Should return ValuationResult evaluating to ErrorCode::missing_mark
TEST(Valuation, CalculateEquityMissingMark) {
	std::int64_t cash_minor_units{10000};
	std::int64_t quantity_units{40};
	std::optional<std::int64_t> mark_minor_units{std::nullopt};
	auto res = jacks::calculate_equity(cash_minor_units, quantity_units, mark_minor_units);
	jacks::ErrorCode err;
	ASSERT_NO_THROW(assume_valuation_err(res, err));
	ASSERT_EQ(err, jacks::ErrorCode::missing_mark);
};
 
// Cash: -1000; Quantity: 50; Mark: 40;
// Should return ValuationResult evaluating to ErrorCode::invalid_input
TEST(Valuation, CalculateEquityInvalidCash) {
	std::int64_t cash_minor_units(-1000);
	std::int64_t quantity_units{50};
	std::optional<std::int64_t> mark_minor_units{40};
	auto res = jacks::calculate_equity(cash_minor_units, quantity_units, mark_minor_units);
	jacks::ErrorCode err;
	ASSERT_NO_THROW(assume_valuation_err(res, err));
	ASSERT_EQ(err, jacks::ErrorCode::invalid_input);
}

// Cash: 15000; Quantity: -1; Mark: 240;
// Should return ValuationResult evaluating to ErrorCode::invalid_input
TEST(Valuation, CalculateEquityInvalidQuantity) {
	std::int64_t cash_minor_units{15000};
	std::int64_t quantity_units{-1};
	std::optional<std::int64_t> mark_minor_units{240};
	auto res = jacks::calculate_equity(cash_minor_units, quantity_units, mark_minor_units);
	jacks::ErrorCode err;
	ASSERT_NO_THROW(assume_valuation_err(res, err));
	ASSERT_EQ(err, jacks::ErrorCode::invalid_input);
}

// Cash: 1000; Quantity: 100; Mark: 24;
// Should return ValuationResult evaluating to Equity->3400
TEST(Valuation, CalculateEquityValidCalculation) {
	std::int64_t cash_minor_units{1000};
	std::int64_t quantity_units{100};
	std::optional<std::int64_t> mark_minor_units{24};
	jacks::Equity expected{3400};
	auto res = jacks::calculate_equity(cash_minor_units, quantity_units, mark_minor_units);
	jacks::Equity eq{};
	ASSERT_NO_THROW(assume_valuation_equity(res, eq));
	ASSERT_EQ(eq.minor_units, expected.minor_units);
}

// Cash: 100; Quantity: 1; Mark: -17;
// Should return ValuationResult evaluating to ErrorCode::invalid_input
TEST(Valuation, CalculateEquityInvalidMark) {
	std::int64_t cash_minor_units{100};
	std::int64_t quantity_units{100};
	std::optional<std::int64_t> mark_minor_units{-1};
	auto res = jacks::calculate_equity(cash_minor_units, quantity_units, mark_minor_units);
	jacks::ErrorCode err;
	ASSERT_NO_THROW(assume_valuation_err(res, err));
	ASSERT_EQ(err, jacks::ErrorCode::invalid_input);
}

// Cash: 3; Quantity: INT64_MAX / 3; Mark: 3
// Should return ValuationResult evaluating to ErrorCode::overflow
TEST(Valuation, CalculateEquityOverflow) {
	std::int64_t cash_minor_units{3};
	std::int64_t quantity_units{INT64_MAX / 3};
	std::optional<std::int64_t> mark_minor_units{3};
	auto res = jacks::calculate_equity(cash_minor_units, quantity_units, mark_minor_units);
	jacks::ErrorCode err;
	ASSERT_NO_THROW(assume_valuation_err(res, err));
	ASSERT_EQ(err, jacks::ErrorCode::overflow);
}

// Cash: 1; Quantity: INT64_MAX / 3; Mark: 3
// Should return ValuationResult evaluating to Equity->INT64_MAX
TEST(Valuation, CalculateEquityAlmostOverflow) {
	std::int64_t cash_minor_units{1};
	std::int64_t quantity_units{INT64_MAX / 3};
	std::optional<std::int64_t> mark_minor_units{3};
	auto res = jacks::calculate_equity(cash_minor_units, quantity_units, mark_minor_units);
	jacks::Equity eq{};
	ASSERT_NO_THROW(assume_valuation_equity(res, eq));
	ASSERT_EQ(eq.minor_units, INT64_MAX);
}
