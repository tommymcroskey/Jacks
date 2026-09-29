#include "jacks/itch_add_order.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using DecodeSignature = jacks::itch::AddOrderResult (*)(const std::vector<std::uint8_t>&);
static_assert(std::is_same_v<decltype(&jacks::itch::decode_add_order), DecodeSignature>);

TEST(ItchAddOrder, ImplementationPending) {
    GTEST_SKIP() << "L-007 decoder implementation and behavioral tests belong to Tommy";
}
