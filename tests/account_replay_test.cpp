#include "jacks/account_replay.hpp"

#include <gtest/gtest.h>
#include <type_traits>

using AccountReplaySignature = jacks::AccountReplayResult (*)(
    const jacks::DepositState&, const std::vector<jacks::AccountEvent>&);
static_assert(std::is_same_v<decltype(&jacks::replay_account), AccountReplaySignature>);

TEST(AccountReplay, ImplementationPending) {
    GTEST_SKIP() << "L-006 implementation and behavioral tests belong to Tommy";
}
