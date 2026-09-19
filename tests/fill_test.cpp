#include "jacks/fill.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using FillSignature = jacks::FillResult (*)(const jacks::DepositState&, const jacks::Fill&);
static_assert(std::is_same_v<decltype(&jacks::apply_fill), FillSignature>);

TEST(Fill, ImplementationPending) {
    GTEST_SKIP() << "L-005 implementation and behavioral tests belong to Tommy";
}
