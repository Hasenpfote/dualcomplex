#include <gtest/gtest.h>
#include <dualcomplex/dualcomplex_base.h>
#include <dualcomplex/dualcomplex_transform.h>
#include <dualcomplex/dualcomplex_query.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class DualComplexQueryTest
    : public ::testing::Test
{
protected:
    static const T PI;

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

template<typename T>
const T
DualComplexQueryTest<T>::PI = std::acos(-T(1));

using MyTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(DualComplexQueryTest, MyTypes);

TYPED_TEST(DualComplexQueryTest, is_zero)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexQueryTest<TypeParam>::absolute_tolerance();

    {
        constexpr auto val = atol;
        const auto dc = DC(C(val, val), C(-val, -val));

        EXPECT_TRUE(is_zero(dc, atol));
    }
    {
        constexpr auto val = atol * TypeParam(10);
        const auto dc = DC(C(val, val), C(-val, -val));

        EXPECT_FALSE(is_zero(dc, atol));
    }
}

TYPED_TEST(DualComplexQueryTest, is_identity)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexQueryTest<TypeParam>::absolute_tolerance();

    auto dc = DC(C(TypeParam(1), TypeParam(0)), C(TypeParam(0), TypeParam(0)));
    EXPECT_TRUE(is_identity(dc, atol));

    dc = DC(C(TypeParam(0), TypeParam(1)), C(TypeParam(0), TypeParam(0)));
    EXPECT_FALSE(is_identity(dc, atol));

    dc = DC(C(TypeParam(0), TypeParam(0)), C(TypeParam(1), TypeParam(0)));
    EXPECT_FALSE(is_identity(dc, atol));

    dc = DC(C(TypeParam(0), TypeParam(0)), C(TypeParam(0), TypeParam(1)));
    EXPECT_FALSE(is_identity(dc, atol));
}

TYPED_TEST(DualComplexQueryTest, is_unit)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexQueryTest<TypeParam>::absolute_tolerance();

    const auto angle = DualComplexQueryTest<TypeParam>::PI / TypeParam(2);

    {
        const auto real = std::polar(TypeParam(1), angle / TypeParam(2));
        const auto dual = C(TypeParam(2), TypeParam(3));
        EXPECT_TRUE(is_unit(DC(real, dual), atol));
    }
    {
        const auto real = std::polar(TypeParam(2), angle / TypeParam(2));
        const auto dual = C(TypeParam(2), TypeParam(3));
        EXPECT_FALSE(is_unit(DC(real, dual), atol));
    }
}

TYPED_TEST(DualComplexQueryTest, are_same)
{
    using C = std::complex<TypeParam>;

    constexpr auto atol = DualComplexQueryTest<TypeParam>::absolute_tolerance();

#ifndef NDEBUG
    // When not a unit dual complex number.
    {
        const auto angle = DualComplexQueryTest<TypeParam>::PI / TypeParam(4);
        const auto d = C(TypeParam(1), TypeParam(2));

        auto dc = dcn::translation(d) * (TypeParam(2) * dcn::rotation(angle));

        EXPECT_DEATH({ are_same(dc, dc, atol); }, "");
    }
#endif
    // dc == dc
    {
        const auto angle = DualComplexQueryTest<TypeParam>::PI / TypeParam(4);
        const auto d = C(TypeParam(1), TypeParam(2));

        auto dc = dcn::translation(d) * dcn::rotation(angle);

        EXPECT_TRUE(are_same(dc, dc, atol));
    }
    // dc == -dc
    {
        const auto angle = DualComplexQueryTest<TypeParam>::PI / TypeParam(4);
        const auto d = C(TypeParam(1), TypeParam(2));

        auto dc = dcn::translation(d) * dcn::rotation(angle);

        EXPECT_TRUE(are_same(dc, -dc, atol));
    }
    // angle == -(pi - angle)
    {
        // Deg 90
        const auto angle0 = DualComplexQueryTest<TypeParam>::PI * (TypeParam(1) / TypeParam(2));
        const auto d0 = C(TypeParam(2), TypeParam(3));
        // Deg -270
        const auto angle1 =-DualComplexQueryTest<TypeParam>::PI * (TypeParam(3) / TypeParam(2));
        const auto d1 = C(TypeParam(2), TypeParam(3));

        auto dc0 = dcn::translation(d0) * dcn::rotation(angle0);
        auto dc1 = dcn::translation(d1) * dcn::rotation(angle1);

        EXPECT_TRUE(are_same(dc0, dc1, atol));
        EXPECT_TRUE(almost_equal(dc0, -dc1, atol));
        EXPECT_TRUE(almost_equal(-dc0, dc1, atol));
    }
}

}   // namespace