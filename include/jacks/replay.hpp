#pragma once

#include "jacks/deposit.hpp"

#include <cstddef>
#include <optional>
#include <variant>
#include <vector>

namespace jacks {

struct ReplayStep {
    std::size_t input_index;
    DepositApplied applied;
};

struct ReplayFailure {
    std::size_t input_index;
    DepositError error;
};

struct ReplayReport {
    DepositState opening_state;
    Equity opening_equity;
    DepositState final_state;
    Equity final_equity;
    std::vector<ReplayStep> accepted;
    std::optional<ReplayFailure> failure;
};

enum class ReplayError { invalid_initial_state };
using ReplayResult = std::variant<ReplayReport, ReplayError>;

// Validate the opening snapshot even for an empty input. A new run requires
// initial net deposits and last_sequence both zero; resumed runs are unsupported.
// Apply deposits in supplied order, stopping at the first rejected event.
// A report retains only accepted steps and the last accepted state/equity;
// failure identifies the zero-based input index and original DepositError.
// Invalid initialization returns ReplayError instead of a report/event failure.
// Inputs are never changed; events are not sorted or retried.
// With the required zero opening counter, final net deposits are interval funding.
[[nodiscard]] ReplayResult replay_deposits(
    const DepositState& opening,
    const std::vector<Deposit>& deposits);

} // namespace jacks
