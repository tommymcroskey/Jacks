#include "jacks/valuation.hpp"

#include <stdexcept>

namespace jacks {

ValuationResult calculate_equity(
    std::int64_t /*cash_minor_units*/,
    std::int64_t /*quantity_units*/,
    std::optional<std::int64_t> /*mark_minor_units*/) {
    // ENGINEERING SCAFFOLD ONLY. Tommy owns the implementation.
    // This deliberate failure is not a domain error or accepted behavior.
    throw std::logic_error("L-001 calculate_equity is not implemented");
}

} // namespace jacks
