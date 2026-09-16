#include "jacks/deposit.hpp"
#include <gtest/gtest.h>

#include <type_traits>

// Infrastructure/interface check only; it establishes no deposit behavior.
using DepositSignature = jacks::DepositResult (*)(
    const jacks::DepositState&, const jacks::Deposit&);
static_assert(std::is_same_v<decltype(&jacks::apply_deposit), DepositSignature>);

TEST(Deposit, ApprenticeTestsPending) {
    // Tommy owns the first deposit test and subsequent component tests.
    GTEST_SKIP() << "L-002 component tests have not been written";
}
