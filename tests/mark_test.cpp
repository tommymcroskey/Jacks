#include "jacks/mark.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using MarkSignature = jacks::MarkResult (*)(const jacks::DepositState&, const jacks::MarkUpdate&);
static_assert(std::is_same_v<decltype(&jacks::apply_mark), MarkSignature>);

TEST(Mark, ApprenticeImplementationPending) {
    GTEST_SKIP() << "Infrastructure placeholder only; replace with behavioral tests.";
}
