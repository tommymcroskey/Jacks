#include "jacks/profit.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using ProfitSignature = jacks::ProfitResult (*)(
    jacks::Equity, jacks::Equity, std::int64_t);
static_assert(std::is_same_v<decltype(&jacks::calculate_profit), ProfitSignature>);

TEST(Profit, ProfitNegativeOpeningEquity) {
	jacks::Equity opening{-10000};
	jacks::Equity current{1000};
	int64_t net_deposits_minor_units{11000};
	const auto res = jacks::calculate_profit(opening, current, net_deposits_minor_units);
	ASSERT_TRUE(std::holds_alternative<jacks::ProfitError>(res));
	ASSERT_EQ(std::get<jacks::ProfitError>(res), jacks::ProfitError::invalid_input);
}

TEST(Profit, ProfitNegativeCurrentEquity) {
	jacks::Equity opening{25000};
	jacks::Equity current{-1};
	int64_t net_deposits_minor_units{56};
	const auto res = jacks::calculate_profit(opening, current, net_deposits_minor_units);
	ASSERT_TRUE(std::holds_alternative<jacks::ProfitError>(res));
	ASSERT_EQ(std::get<jacks::ProfitError>(res), jacks::ProfitError::invalid_input);
}

TEST(Profit, ProfitNegativeNetDeposits) {
	jacks::Equity opening{426788};
	jacks::Equity current{12239};
	int64_t net_deposits_minor_units{-30};
	const auto res = jacks::calculate_profit(opening, current, net_deposits_minor_units);
	ASSERT_TRUE(std::holds_alternative<jacks::ProfitError>(res));
	ASSERT_EQ(std::get<jacks::ProfitError>(res), jacks::ProfitError::invalid_input);
}

TEST(Profit, ProfitUnderflow) {
	jacks::Equity opening{INT64_MAX};
	jacks::Equity current{129};
	int64_t net_deposits_minor_units{9000};
	const auto res = jacks::calculate_profit(opening, current, net_deposits_minor_units);
	ASSERT_TRUE(std::holds_alternative<jacks::ProfitError>(res));
	ASSERT_EQ(std::get<jacks::ProfitError>(res), jacks::ProfitError::overflow);
}

TEST(Profit, ProfitZeroEquitiesZeroDeposits) {
	jacks::Equity opening{0};
	jacks::Equity current{0};
	int64_t net_deposits_minor_units{0};
	const auto res = jacks::calculate_profit(opening, current, net_deposits_minor_units);
	ASSERT_TRUE(std::holds_alternative<jacks::Profit>(res));
	ASSERT_EQ(std::get<jacks::Profit>(res).minor_units, 0);
}

TEST(Profit, ProfitNonzeroEquitiesZeroDeposits) {
	jacks::Equity opening{100};
	jacks::Equity current{400285};
	int64_t net_deposits_minor_units{0};
	const auto res = jacks::calculate_profit(opening, current, net_deposits_minor_units);
	ASSERT_TRUE(std::holds_alternative<jacks::Profit>(res));
	ASSERT_EQ(std::get<jacks::Profit>(res).minor_units, 400185);
}

TEST(Profit, ProfitRealisticExample) {
	jacks::Equity opening{0};
	jacks::Equity current{3080148};
	int64_t net_deposits_minor_units{3000000};
	const auto res = jacks::calculate_profit(opening, current, net_deposits_minor_units);
	ASSERT_TRUE(std::holds_alternative<jacks::Profit>(res));
	ASSERT_EQ(std::get<jacks::Profit>(res).minor_units, 80148);
}

TEST(Profit, ProfitMaxLoss) {
	jacks::Equity opening{INT64_MAX};
	jacks::Equity current{0};
	int64_t net_deposits_minor_units{1};
	const auto res = jacks::calculate_profit(opening, current, net_deposits_minor_units);
	ASSERT_TRUE(std::holds_alternative<jacks::Profit>(res));
	ASSERT_EQ(std::get<jacks::Profit>(res).minor_units, INT64_MIN);
}

TEST(Profit, ProfitMaxGain) {
	jacks::Equity opening{0};
	jacks::Equity current{INT64_MAX};
	int64_t net_deposits_minor_units{0};
	const auto res = jacks::calculate_profit(opening, current, net_deposits_minor_units);
	ASSERT_TRUE(std::holds_alternative<jacks::Profit>(res));
	ASSERT_EQ(std::get<jacks::Profit>(res).minor_units, INT64_MAX);
}
