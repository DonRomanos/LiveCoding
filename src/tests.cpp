#include "challenge.hpp"
#include "reference.hpp"

#include "gtest/gtest.h"

template <typename T>
class Some_typed_Test : public ::testing::Test
{

};

// Now we can instantiate it with our types.
#ifdef BUILD_REFERENCE
    using TypesToTest = ::testing::Types<challenge::SomeClass, reference::SomeClass>;
#else
    using TypesToTest = ::testing::Types<reference::SomeClass>;
#endif

TYPED_TEST_SUITE(Some_typed_Test, TypesToTest);

/***************************************************************/
TYPED_TEST(Some_typed_Test, Should_Succeed) {
    // Inside a test, refer to TypeParam to get the type parameter.
    // TypeParam n = 0;
    SUCCEED();
}

TEST(GtestDependency, Should_Compile_if_gtest_was_found)
{
    SUCCEED();
}