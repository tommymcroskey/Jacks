#include "jacks/deposit.hpp"

#include <stdexcept>

namespace jacks {

DepositResult apply_deposit(
    const DepositState& /*current*/,
    const Deposit& /*deposit*/) {
    // ENGINEERING SCAFFOLD ONLY. Tommy owns the transition implementation.
    // Temporary deliberate failure, not a domain result or accepted behavior.
    throw std::logic_error("L-002 apply_deposit is not implemented");
}

} // namespace jacks
