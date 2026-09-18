#include <gtest/gtest.h>

// Reviewer-owned negative control; never merge into production or apprentice work.
TEST(RiskHostedProbe, IntentionalAssertionFailure) {
    FAIL() << "Expected negative-control failure: verify CI propagates CTest failure.";
}
