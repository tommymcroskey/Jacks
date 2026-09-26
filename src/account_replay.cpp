#include "jacks/account_replay.hpp"

#include <type_traits>

namespace jacks {

template <
	typename Event_T,
	typename Event_Result_T,
	typename Event_Applied_T,
	typename Event_Err_T
>
constexpr bool is_valid_event_type_combination() {
	return (std::is_same_v<Event_T, MarkUpdate>
			&& std::is_same_v<Event_Result_T, MarkResult>
			&& std::is_same_v<Event_Applied_T, MarkApplied>
			&& std::is_same_v<Event_Err_T, MarkError>)
			|| (std::is_same_v<Event_T, Fill>
			&& std::is_same_v<Event_Result_T, FillResult>
			&& std::is_same_v<Event_Applied_T, FillApplied>
			&& std::is_same_v<Event_Err_T, FillError>)
			|| (std::is_same_v<Event_T, Deposit>
			&& std::is_same_v<Event_Result_T, DepositResult>
			&& std::is_same_v<Event_Applied_T, DepositApplied>
			&& std::is_same_v<Event_Err_T, DepositError>);
}


template <
	typename Event_T,
	typename Event_Result_T,
	typename Event_Applied_T,
	typename Event_Err_T
>
void apply_record_event(
	AccountReplayReport& report,
	DepositState& last_state,
	const Event_T& event,
	const std::size_t idx) {

	static_assert(
		is_valid_event_type_combination<
			Event_T,
			Event_Result_T,
			Event_Applied_T,
			Event_Err_T
		>(),
		"Invalid type combination"
	);

	Event_Result_T event_result;

	if constexpr (std::is_same_v<Event_T, MarkUpdate>) {
		event_result = apply_mark(last_state, event); 
	} else if constexpr (std::is_same_v<Event_T, Deposit>) {
		event_result = apply_deposit(last_state,event); 
	} else if constexpr (std::is_same_v<Event_T, Fill>) {
		event_result = apply_fill(last_state, event);
	}

	if (std::holds_alternative<Event_Err_T>(event_result)) {
		report.failure = AccountReplayFailure{
			idx,
			std::get<Event_Err_T>(event_result)
		};
		return;
	}

	auto event_applied = std::get<Event_Applied_T>(event_result);
	last_state = event_applied.state;

	report.accepted.push_back(AccountReplayStep{idx, event_applied});
}

void record_final_state_equity(AccountReplayReport& report) {

	if (report.accepted.empty()) {
		report.final_state = report.opening_state;
		report.final_equity = report.opening_equity;
		return;
	}

	const auto last_step_applied = report.accepted.back().applied;
	std::visit([&report](const auto& applied) {
		report.final_state = applied.state;
		report.final_equity = applied.equity;
	}, last_step_applied);
}

AccountReplayResult replay_account(
	const DepositState& opening_state,
	const std::vector<AccountEvent>& events) {

	const auto opening_valuation_result = calculate_equity(
		opening_state.cash_minor_units,
		opening_state.quantity_units,
		opening_state.mark_minor_units
	);

	if (std::holds_alternative<ErrorCode>(opening_valuation_result)
		|| opening_state.net_deposits_minor_units != 0
		|| opening_state.last_sequence != 0) {
		return AccountReplayError::invalid_initial_state;
	}

	AccountReplayReport res;
	res.opening_state = opening_state;
	res.opening_equity = std::get<Equity>(opening_valuation_result);
	if (events.empty()) {
		res.final_state = res.opening_state;
		res.final_equity = res.opening_equity;
		return res;
	}

	size_t idx{0};
	DepositState last_state{opening_state};
	for (auto& event : events) {
		std::visit([&](const auto& concrete_event) {
			using T = std::decay_t<decltype(concrete_event)>;

			if constexpr (std::is_same_v<T, MarkUpdate>) {
				apply_record_event<
					MarkUpdate, MarkResult, MarkApplied, MarkError>(
						res, last_state, concrete_event, idx);

			} else if constexpr (std::is_same_v<T, Fill>) {
				apply_record_event<
					Fill, FillResult, FillApplied, FillError>(
						res, last_state, concrete_event, idx);

			} else if constexpr (std::is_same_v<T, Deposit>) {
				apply_record_event<
					Deposit, DepositResult, DepositApplied, DepositError>(
						res, last_state, concrete_event, idx);
			}
		}, event);

		if (res.failure) {
			record_final_state_equity(res);
			return res;
		}

		idx++;
	}

	record_final_state_equity(res);

	return res;
}

} // namespace jacks
