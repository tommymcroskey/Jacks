#include "jacks/profit.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using ProfitSignature = jacks::ProfitResult (*)(
    jacks::Equity, jacks::Equity, std::int64_t);
static_assert(std::is_same_v<decltype(&jacks::calculate_profit), ProfitSignature>);

TEST(Profit, ApprenticeImplementationPending) {
    GTEST_SKIP() << "Infrastructure placeholder only; replace with behavioral tests.";
}
