#include "jacks/fill.hpp"

#include <stdexcept>

namespace jacks {

FillResult apply_fill(const DepositState&, const Fill&) {
    throw std::logic_error("L-005 apply_fill is not implemented");
}

} // namespace jacks
