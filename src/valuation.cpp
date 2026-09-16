#include "jacks/valuation.hpp"

namespace jacks {

ValuationResult calculate_equity(
    std::int64_t cash_minor_units,
    std::int64_t quantity_units,
    std::optional<std::int64_t> mark_minor_units) {
		ValuationResult res;
		if (cash_minor_units < 0
			|| quantity_units < 0
			|| (mark_minor_units != std::nullopt && (*mark_minor_units) < 0)) {
				res = ErrorCode::invalid_input;
		} else if (quantity_units == 0) {
				res = Equity{cash_minor_units};
		} else if (mark_minor_units == std::nullopt) {
				res = ErrorCode::missing_mark;
		} else if ((INT64_MAX - cash_minor_units) / quantity_units < (*mark_minor_units)) {
				res = ErrorCode::overflow;
		} else {
				res = Equity{quantity_units * (*mark_minor_units) + cash_minor_units};
		}
		return res;
	}
} // namespace jacks
