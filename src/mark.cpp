#include "jacks/mark.hpp"

#include <stdexcept>

namespace jacks {

MarkResult apply_mark(
	const DepositState& state,
	const MarkUpdate& mark_update) {

	const auto initial_equity = calculate_equity(
		state.cash_minor_units,
		state.quantity_units,
		state.mark_minor_units
	);

	if (std::holds_alternative<ErrorCode>(initial_equity)
		|| state.net_deposits_minor_units < 0
		|| state.last_sequence < 0) {
		return MarkError::invalid_state;
	}

	if (mark_update.mark_minor_units < 0) {
		return MarkError::invalid_mark;
	}

	if (mark_update.sequence <= state.last_sequence) {
		return MarkError::invalid_sequence;
	}

	if (mark_update.mark_minor_units
		> ((INT64_MAX - state.cash_minor_units)
		/ state.quantity_units)) {
		return MarkError::overflow;
	}

	const DepositState new_state{
		state.cash_minor_units,
		state.quantity_units,
		mark_update.mark_minor_units,
		state.net_deposits_minor_units,
		mark_update.sequence
	};

	const auto equity = calculate_equity(
		state.cash_minor_units,
		state.quantity_units,
		mark_update.mark_minor_units
	);

	return MarkApplied{new_state, std::get<Equity>(equity)};
}

} // namespace jacks
