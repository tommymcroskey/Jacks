#include "jacks/fill.hpp"

#include <stdexcept>

namespace jacks {

bool is_valid_fill_side(FillSide);

FillResult apply_fill(
	const DepositState& state,
	const Fill& fill) {

	const auto initial_equity = calculate_equity(
		state.cash_minor_units,
		state.quantity_units,
		state.mark_minor_units
	);

	if (state.net_deposits_minor_units < 0
		|| state.last_sequence < 0
		|| std::holds_alternative<ErrorCode>(initial_equity)) {
		return FillError::invalid_state;
	}

	if ((fill.side != FillSide::sell
		&& fill.side != FillSide::buy)
		|| fill.quantity_units <= 0
		|| fill.price_minor_units <= 0
		|| fill.fee_minor_units < 0) {
		return FillError::invalid_fill;
	}

	if (fill.sequence <= state.last_sequence) {
		return FillError::invalid_sequence;
	}

	if (state.mark_minor_units == std::nullopt) {
		return FillError::missing_mark;
	}

	if (fill.side == FillSide::sell) {
		if (fill.quantity_units > state.quantity_units) {
			return FillError::insufficient_quantity;
		}

		// gross overflow
		if (INT64_MAX / fill.quantity_units < fill.price_minor_units) {
			return FillError::overflow;
		}
	}

	if (fill.side == FillSide::buy) {
		// quantity overflow
		if (INT64_MAX - fill.quantity_units < state.quantity_units) { 
			return FillError::overflow;
		}

		// gross overflow
		if (INT64_MAX / fill.quantity_units < fill.price_minor_units) {
			return FillError::overflow;
		}
	}

	const auto gross = fill.quantity_units * fill.price_minor_units;

	if (fill.side == FillSide::sell) {
		// insufficient cash
		if (state.cash_minor_units - fill.fee_minor_units < -gross) {
			return FillError::insufficient_cash;
		}
		// final cash overflow
		if (INT64_MAX - state.cash_minor_units - fill.fee_minor_units
			< gross) {
			return FillError::overflow;
		}
	}

	if (fill.side == FillSide::buy) {
		// insufficient cash
		if (state.cash_minor_units - fill.fee_minor_units < gross) {
			return FillError::insufficient_cash;
		}
	}

	const DepositState resulting_state{
		state.cash_minor_units + (fill.side == FillSide::buy ? -gross : gross)
		- fill.fee_minor_units,
		state.quantity_units + (fill.side == FillSide::buy
		? fill.quantity_units : -fill.quantity_units),
		state.mark_minor_units,
		state.net_deposits_minor_units,
		fill.sequence
	};

	const auto resulting_equity = calculate_equity(
		resulting_state.cash_minor_units,
		resulting_state.quantity_units,
		resulting_state.mark_minor_units
	);

	if (std::holds_alternative<ErrorCode>(resulting_equity)) {
		return FillError::overflow;
	}

	return FillApplied{resulting_state, std::get<Equity>(resulting_equity)};
}

} // namespace jacks
