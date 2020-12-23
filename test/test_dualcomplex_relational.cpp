#include <gtest/gtest.h>
#include <dualcomplex/dualcomplex_base.h>
#include <dualcomplex/dualcomplex_relational.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class DualComplexRelationalTest
    : public ::testing::Test
{
protected:
    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, float>::value, U>::type
    absolute_tolerance(){ return 1e-4f; }

    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, double>::value, U>::type
    absolute_tolerance(){ return 1e-8; }

    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, float>::value, U>::type
    relative_tolerance(){ return 1e-5f; }

    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, double>::value, U>::type
    relative_tolerance(){ return 1e-5; }
};

using MyTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(DualComplexRelationalTest, MyTypes);

TYPED_TEST(DualComplexRelationalTest, almost_equal)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexRelationalTest<TypeParam>::absolute_tolerance();

    {
        constexpr auto val1 = atol;
        constexpr auto val2 = TypeParam(0);
        const auto dc1 = DC(C(val1, val1), C(-val1, -val1));
        const auto dc2 = DC(C(val2, val2), C(val2, val2));

        EXPECT_TRUE(almost_equal(dc1, dc2, atol));
    }
    {
        constexpr auto val1 = atol * TypeParam(10);
        constexpr auto val2 = TypeParam(0);
        const auto dc1 = DC(C(val1, val1), C(-val1, -val1));
        const auto dc2 = DC(C(val2, val2), C(val2, val2));

        EXPECT_FALSE(almost_equal(dc1, dc2, atol));
    }
    {
        constexpr auto val1 = TypeParam(1000);
        constexpr auto val2 = TypeParam(999);
        const auto dc1 = DC(C(val1, val1), C(val1, val1));
        const auto dc2 = DC(C(val2, val2), C(val2, val2));

        EXPECT_FALSE(almost_equal(dc1, dc2, atol));
    }
}

TYPED_TEST(DualComplexRelationalTest, almost_zero)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexRelationalTest<TypeParam>::absolute_tolerance();

    {
        constexpr auto val = atol;
        const auto dc = DC(C(val, val), C(-val, -val));
        EXPECT_TRUE(almost_zero(dc, atol));
    }
    {
        constexpr auto val = atol * TypeParam(10);
        const auto dc = DC(C(val, val), C(-val, -val));
        EXPECT_FALSE(almost_zero(dc, atol));
    }
}

}   // namespace