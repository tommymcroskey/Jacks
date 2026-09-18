#include "jacks/profit.hpp"

namespace jacks {

ProfitResult calculate_profit(
	Equity opening_equity,
	Equity current_equity,
	std::int64_t net_deposits_minor_units) {

	if (opening_equity.minor_units < 0
		|| current_equity.minor_units < 0
		|| net_deposits_minor_units < 0) {
		return ProfitError::invalid_input;
	}

	// since inputs are in range [0, INT64_MAX]
	// opening_equity + net_deposits_minor_units <= INT64_MAX - INT64_MIN
	// Therefore adding these two to INT64_MIN, where first
	// result calculated is with INT64_MIN, is safe
	if ((INT64_MIN + opening_equity.minor_units)
		+ net_deposits_minor_units
		> current_equity.minor_units) {
		return ProfitError::overflow;
	}

	return Profit{current_equity.minor_units
		- opening_equity.minor_units
		- net_deposits_minor_units};
}

} // namespace jacks
