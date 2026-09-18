#pragma once

#include "jacks/deposit.hpp"

namespace jacks {

struct MarkUpdate {
    std::int64_t mark_minor_units;
    std::int64_t sequence;
};

struct MarkApplied {
    DepositState state;
    Equity equity;
};

enum class MarkError { invalid_state, invalid_mark, invalid_sequence, overflow };
using MarkResult = std::variant<MarkApplied, MarkError>;

// Reuse DepositState as the existing account snapshot; its name is historical.
// Precedence: invalid current state (same structural domain as apply_deposit),
// negative new mark, sequence <= 0 or <= current.last_sequence, candidate overflow.
// Current valuation must succeed; this operation does not repair invalid state.
// Success replaces mark and last_sequence only, returning the new equity.
// Zero mark, unchanged mark, and zero holdings are valid observations.
// Inputs never change. Rejection consumes no sequence and returns no new state.
// The shared sequence orders supplied events; it makes no freshness/time claim.
[[nodiscard]] MarkResult apply_mark(const DepositState& current, const MarkUpdate& update);

} // namespace jacks
