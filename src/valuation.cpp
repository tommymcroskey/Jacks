#include "jacks/valuation.hpp"

namespace jacks {

ValuationResult calculate_equity(
    std::int64_t cash_minor_units,
    std::int64_t quantity_units,
    std::optional<std::int64_t> mark_minor_units) {

	if (cash_minor_units < 0
		|| quantity_units < 0
		|| (mark_minor_units != std::nullopt && (*mark_minor_units) < 0)) {
		return ErrorCode::invalid_input;
	}
	if (quantity_units == 0) {
		return Equity{cash_minor_units};
	}
	if (mark_minor_units == std::nullopt) {
		return ErrorCode::missing_mark;
	}
	if ((INT64_MAX - cash_minor_units) / quantity_units < (*mark_minor_units)) {
		return ErrorCode::overflow;
	}
	return Equity{quantity_units * (*mark_minor_units) + cash_minor_units};
}

} // namespace jacks
