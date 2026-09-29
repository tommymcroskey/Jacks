#include "jacks/itch_add_order.hpp"

#include <stdexcept>

namespace jacks::itch {

AddOrderResult decode_add_order(const std::vector<std::uint8_t>&) {
    throw std::logic_error("L-007 decode_add_order is not implemented");
}

} // namespace jacks::itch
