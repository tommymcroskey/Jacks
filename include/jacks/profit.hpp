#pragma once

#include "jacks/valuation.hpp"

#include <cstdint>
#include <variant>

namespace jacks {

struct Profit {
    std::int64_t minor_units;
};

enum class ProfitError { invalid_input, overflow };
using ProfitResult = std::variant<Profit, ProfitError>;

// Funding-adjusted change over one interval, in minor currency units:
// current equity - opening equity - deposits made during that interval.
// All inputs must be nonnegative; invalid_input takes precedence over overflow.
// Profit may be negative. Return overflow only if the mathematical result cannot
// fit int64_t; an avoidable intermediate overflow must not reject a valid result.
// This checks arithmetic, not the provenance or consistency of account history.
[[nodiscard]] ProfitResult calculate_profit(
    Equity opening_equity,
    Equity current_equity,
    std::int64_t net_deposits_minor_units);

} // namespace jacks
