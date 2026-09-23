#pragma once

#include "jacks/deposit.hpp"
#include "jacks/fill.hpp"
#include "jacks/mark.hpp"

#include <cstddef>
#include <optional>
#include <variant>
#include <vector>

namespace jacks {

using AccountEvent = std::variant<Deposit, MarkUpdate, Fill>;
using AccountApplied = std::variant<DepositApplied, MarkApplied, FillApplied>;
using AccountEventError = std::variant<DepositError, MarkError, FillError>;

struct AccountReplayStep {
    std::size_t input_index;
    AccountApplied applied;
};

struct AccountReplayFailure {
    std::size_t input_index;
    AccountEventError error;
};

struct AccountReplayReport {
    DepositState opening_state;
    Equity opening_equity;
    DepositState final_state;
    Equity final_equity;
    std::vector<AccountReplayStep> accepted;
    std::optional<AccountReplayFailure> failure;
};

enum class AccountReplayError { invalid_initial_state };
using AccountReplayResult = std::variant<AccountReplayReport, AccountReplayError>;

// Contract: learning/assignments/L-006.md. New runs only: opening net deposits
// and last sequence must be zero, and opening calculate_equity must succeed.
// Validate even for an empty input. Apply each event using its accepted transition,
// in supplied order. Preserve the exact applied/error alternative and input index.
// Stop at the first rejection; retain only the accepted prefix and its final state.
// Inputs never change. No sorting, retry, execution inference or profit arithmetic.
[[nodiscard]] AccountReplayResult replay_account(
    const DepositState& opening,
    const std::vector<AccountEvent>& events);

} // namespace jacks
