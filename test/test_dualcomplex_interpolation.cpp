#include <gtest/gtest.h>
#include <dualcomplex/dualcomplex_base.h>
#include <dualcomplex/dualcomplex_interpolation.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class DualComplexInterpolationTest
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
DualComplexInterpolationTest<T>::PI = std::acos(-T(1));

using MyTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(DualComplexInterpolationTest, MyTypes);

TYPED_TEST(DualComplexInterpolationTest, slerp)
{
    using C = std::complex<TypeParam>;

    constexpr auto atol = DualComplexInterpolationTest<TypeParam>::absolute_tolerance();

    const auto angle0 = DualComplexInterpolationTest<TypeParam>::PI / TypeParam(4);
    const auto d0 = C(TypeParam(1), TypeParam(2));
    const auto angle1 = DualComplexInterpolationTest<TypeParam>::PI / TypeParam(2);
    const auto d1 = C(TypeParam(3), TypeParam(4));

    auto p = dcn::translation(d0) * dcn::rotation(angle0);
    auto q = dcn::translation(d1) * dcn::rotation(angle1);
    // t = 0
    {
        auto dc = p;
        auto res = slerp(p, q, TypeParam(0));

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
    // t = 1
    {
        auto dc = q;
        auto res = slerp(p, q, TypeParam(1));

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
    // t
    {
        auto t = TypeParam(0.75);
        auto dc = p * exp(t * log(transformation_difference(p, q)));
        auto res = slerp(p, q, t);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
}

TYPED_TEST(DualComplexInterpolationTest, slerp_shortestpath)
{
    using C = std::complex<TypeParam>;

    constexpr auto atol = DualComplexInterpolationTest<TypeParam>::absolute_tolerance();

    const auto angle0 = DualComplexInterpolationTest<TypeParam>::PI / TypeParam(4);
    const auto d0 = C(TypeParam(1), TypeParam(2));
    const auto angle1 = DualComplexInterpolationTest<TypeParam>::PI * (TypeParam(3) / TypeParam(2));
    const auto d1 = C(TypeParam(3), TypeParam(4));

    auto p = dcn::translation(d0) * dcn::rotation(angle0);
    auto q = dcn::translation(d1) * dcn::rotation(angle1);
    // t = 0
    {
        auto dc = p;
        auto res = slerp_shortestpath(p, q, TypeParam(0));

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
    // t = 1
    {
        auto dc = -q;
        auto res = slerp_shortestpath(p, q, TypeParam(1));

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
    // t
    {
        auto t = TypeParam(0.75);
        auto dc = p * exp(t * log(transformation_difference(p, -q)));
        auto res = slerp_shortestpath(p, q, t);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
}

}   // namespace