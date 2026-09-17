#include "jacks/deposit.hpp"

namespace jacks {

DepositResult apply_deposit(
    const DepositState& current, 
    const Deposit& deposit) {

	const auto initial_equity_res = calculate_equity(
		current.cash_minor_units,
		current.quantity_units,
		current.mark_minor_units);

	if (std::holds_alternative<ErrorCode>(initial_equity_res)
		|| current.net_deposits_minor_units < 0
		|| current.last_sequence < 0) {
		return DepositError::invalid_state;
	}

	if (deposit.amount_minor_units <= 0) {
		return DepositError::invalid_amount;
	}

	// <0 is a subset of <last_sequence since last sequence
	// is clamped to >=0 
	if (deposit.sequence <= current.last_sequence) {
		return DepositError::invalid_sequence;
	}

	// additive overflow check
	if (INT64_MAX - deposit.amount_minor_units < current.cash_minor_units
		|| INT64_MAX - deposit.amount_minor_units < current.net_deposits_minor_units) {
		return DepositError::overflow;
	}

	DepositState newState{
		/* new cash_minor_units */ 		   current.cash_minor_units + deposit.amount_minor_units,
		/* new quantity_units */ 		   current.quantity_units,
		/* new mark_minor_units */ 		   current.mark_minor_units,
		/* new net_deposits_minor_units */ current.net_deposits_minor_units + deposit.amount_minor_units,
		/* new last_sequence */ 		   deposit.sequence
	};

	const auto equity_res = calculate_equity(
		newState.cash_minor_units,
		newState.quantity_units,
		newState.mark_minor_units
	);

	if (std::holds_alternative<ErrorCode>(equity_res)) {
		return DepositError::overflow;
	}

	return DepositApplied{newState, std::get<Equity>(equity_res)};
}

} // namespace jacks
