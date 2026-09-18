#include "jacks/mark.hpp"

#include <stdexcept>

namespace jacks {

MarkResult apply_mark(const DepositState&, const MarkUpdate&) {
    throw std::logic_error("L-004 apprentice implementation pending");
}

} // namespace jacks
