#include "jacks/replay.hpp"

namespace jacks {

ReplayResult replay_deposits(
    const DepositState& opening,
    const std::vector<Deposit>& deposits) {
    if (opening.net_deposits_minor_units != 0 || opening.last_sequence != 0) {
        return ReplayError::invalid_initial_state;
    }

    const auto opening_result = calculate_equity(
        opening.cash_minor_units, opening.quantity_units, opening.mark_minor_units);
    const auto* opening_equity = std::get_if<Equity>(&opening_result);
    if (!opening_equity) {
        return ReplayError::invalid_initial_state;
    }

    ReplayReport report{opening, *opening_equity, opening, *opening_equity, {}, std::nullopt};
    for (std::size_t index = 0; index < deposits.size(); ++index) {
        const auto result = apply_deposit(report.final_state, deposits[index]);
        if (const auto* error = std::get_if<DepositError>(&result)) {
            report.failure = ReplayFailure{index, *error};
            return report;
        }

        const auto& applied = std::get<DepositApplied>(result);
        report.accepted.push_back(ReplayStep{index, applied});
        report.final_state = applied.state;
        report.final_equity = applied.equity;
    }
    return report;
}

} // namespace jacks
