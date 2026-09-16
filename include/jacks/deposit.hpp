#pragma once

#include "jacks/valuation.hpp"

#include <cstdint>
#include <optional>
#include <variant>

namespace jacks {

// Ordinary value carriers: apply_deposit validates them at the boundary.
// Money is in minor CU (100 = 1 CU); quantity is in whole asset units.
struct DepositState {
    std::int64_t cash_minor_units;
    std::int64_t quantity_units;
    std::optional<std::int64_t> mark_minor_units;
    std::int64_t net_deposits_minor_units;
    std::int64_t last_sequence;
};

struct Deposit {
    std::int64_t amount_minor_units;
    std::int64_t sequence;
};

struct DepositApplied {
    DepositState state;
    Equity equity;
};

enum class DepositError { invalid_state, invalid_amount, invalid_sequence, overflow };
using DepositResult = std::variant<DepositApplied, DepositError>;

// Contract: learning/assignments/L-002.md.
// Initialize a valid snapshot with net deposits/sequence zero; retain opening
// equity outside this state. Validation checks structure, not historical provenance.
// Current cash/quantity/present mark/net deposits/sequence must be nonnegative,
// and calculate_equity must succeed for the current state.
// Precedence: invalid_state, amount <= 0, sequence <= 0 or <= last_sequence,
// then unrepresentable cash/net-deposit updates or candidate equity -> overflow.
// Unexpected candidate valuation invalid_input/missing_mark -> invalid_state.
// Success returns a new state/equity, keeping quantity and mark fixed.
// Inputs never change; rejection returns no new state and consumes no sequence.
// Use calculate_equity; no withdrawals, trades, profit calculation, or batch logic.
[[nodiscard]] DepositResult apply_deposit(
    const DepositState& current,
    const Deposit& deposit);

} // namespace jacks
