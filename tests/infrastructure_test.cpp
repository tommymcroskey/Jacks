#include "jacks/valuation.hpp"
#include <gtest/gtest.h>
#include <string_view>
#include <type_traits>
static_assert(__cplusplus >= 201703L, "C++17 is required");
using ExpectedSignature = jacks::ValuationResult (*)(
    std::int64_t, std::int64_t, std::optional<std::int64_t>);
static_assert(std::is_same_v<decltype(&jacks::calculate_equity), ExpectedSignature>);
TEST(Infrastructure, Cpp17AndGoogleTestRun) {
    const std::optional<std::string_view> message{"infrastructure ready"};
    ASSERT_TRUE(message.has_value());
    EXPECT_EQ(*message, "infrastructure ready");
}
