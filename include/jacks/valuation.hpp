#pragma once

#include <cstdint>
#include <optional>
#include <variant>

namespace jacks {

// One fictional currency: 100 minor units = 1 CU.
// These aggregates carry values; they do not enforce validity at construction.
struct Equity {
    std::int64_t minor_units;
};

enum class ErrorCode { invalid_input, missing_mark, overflow };
using ValuationResult = std::variant<Equity, ErrorCode>;

// Cash: minor units; quantity: whole units; mark: minor units per whole unit.
// Contract (L-001): inputs nonnegative, including any supplied mark.
// Zero mark is valid. Absent mark is allowed only with zero quantity.
// Error precedence: invalid_input, missing_mark, overflow.
// Return a value or error, never a numeric error sentinel; do not mutate inputs.
// Check representability before arithmetic. No event processing or profit here.
[[nodiscard]] ValuationResult calculate_equity(
    std::int64_t cash_minor_units,
    std::int64_t quantity_units,
    std::optional<std::int64_t> mark_minor_units);

} // namespace jacks
