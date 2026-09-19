#pragma once

#include "jacks/deposit.hpp"

namespace jacks {

enum class FillSide { buy, sell };

// An externally supplied complete execution, not an order or a price forecast.
// Money is in minor CU; quantity is in whole asset units.
struct Fill {
    FillSide side;
    std::int64_t quantity_units;
    std::int64_t price_minor_units;
    std::int64_t fee_minor_units;
    std::int64_t sequence;
};

struct FillApplied {
    DepositState state;
    Equity equity;
};

enum class FillError {
    invalid_state,
    invalid_fill,
    invalid_sequence,
    missing_mark,
    insufficient_quantity,
    insufficient_cash,
    overflow
};
using FillResult = std::variant<FillApplied, FillError>;

// Contract and ordered validation stages: learning/assignments/L-005.md.
// Validate the current snapshot as in apply_mark; reject unknown side,
// quantity <= 0, execution price <= 0, fee < 0, then invalid sequence.
// A present valuation mark is required; selling cannot exceed holdings.
// Gross quantity * execution price and final quantity must fit int64_t.
// Then check exact final cash for insufficiency or overflow, and equity.
// Intermediate cash + proceeds or cost + fee need not fit when the final result
// is valid.
// Success updates cash, quantity and sequence only; preserve mark and funding.
// Reuse calculate_equity. Inputs stay unchanged; rejection consumes no sequence.
// No matching, execution-price inference, partial fills, borrowing or shorting.
[[nodiscard]] FillResult apply_fill(const DepositState& current, const Fill& fill);

} // namespace jacks
