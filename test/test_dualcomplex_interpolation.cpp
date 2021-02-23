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

TYPED_TEST(DualComplexInterpolationTest, lerp)
{
    using C = std::complex<TypeParam>;

    constexpr auto atol = DualComplexInterpolationTest<TypeParam>::absolute_tolerance();

    const auto angle0 = DualComplexInterpolationTest<TypeParam>::PI / TypeParam(4);
    const auto d0 = C(TypeParam(1), TypeParam(2));
    const auto dc0 = dcn::translation(d0) * dcn::rotation(angle0);

    const auto angle1 = DualComplexInterpolationTest<TypeParam>::PI / TypeParam(2);
    const auto d1 = C(TypeParam(3), TypeParam(4));
    const auto dc1 = dcn::translation(d1) * dcn::rotation(angle1);
    // t == 0
    {
        const auto t = TypeParam(0);

        auto dc = dc0;
        auto res = lerp(dc0, dc1, t);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
    // t == 1
    {
        const auto t = TypeParam(1);

        auto dc = dc1;
        auto res = lerp(dc0, dc1, t);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
    // t == 0.5
    {
        const auto t = TypeParam(0.5);

        auto dc = (TypeParam(1) - t) * dc0 + t * dc1;
        auto res = lerp(dc0, dc1, t);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
        EXPECT_NOT_ALMOST_EQUAL(TypeParam, TypeParam(1), norm(res), atol);
    }
}

TYPED_TEST(DualComplexInterpolationTest, nlerp)
{
    using C = std::complex<TypeParam>;

    constexpr auto atol = DualComplexInterpolationTest<TypeParam>::absolute_tolerance();

    const auto angle0 = DualComplexInterpolationTest<TypeParam>::PI / TypeParam(4);
    const auto d0 = C(TypeParam(1), TypeParam(2));
    const auto dc0 = dcn::translation(d0) * dcn::rotation(angle0);

    const auto angle1 = DualComplexInterpolationTest<TypeParam>::PI / TypeParam(2);
    const auto d1 = C(TypeParam(3), TypeParam(4));
    const auto dc1 = dcn::translation(d1) * dcn::rotation(angle1);
    // t == 0
    {
        const auto t = TypeParam(0);

        auto dc = dc0;
        auto res = lerp(dc0, dc1, t);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
    // t == 1
    {
        const auto t = TypeParam(1);

        auto dc = dc1;
        auto res = lerp(dc0, dc1, t);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
    // t == 0.5
    {
        const auto t = TypeParam(0.5);

        auto dc = (TypeParam(1) - t) * dc0 + t * dc1;
        dc /= norm(dc);
        auto res = nlerp(dc0, dc1, t);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(1), norm(res), atol);
    }
}

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

TYPED_TEST(DualComplexInterpolationTest, dlb)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexInterpolationTest<TypeParam>::absolute_tolerance();

    std::vector<DC> transforms;
    std::vector<TypeParam> weights;

    const auto angle0 = DualComplexInterpolationTest<TypeParam>::PI / TypeParam(4);
    const auto d0 = C(TypeParam(1), TypeParam(2));
    transforms.push_back(dcn::translation(d0) * dcn::rotation(angle0));

    const auto angle1 = DualComplexInterpolationTest<TypeParam>::PI / TypeParam(2);
    const auto d1 = C(TypeParam(3), TypeParam(4));
    transforms.push_back(dcn::translation(d1) * dcn::rotation(angle1));
    // t == 0
    {
        const auto t = TypeParam(0);
        weights.clear();
        weights.push_back(TypeParam(1) - t);
        weights.push_back(t);

        auto dc = transforms[0];
        auto res = dlb(transforms, weights);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
    // t == 1
    {
        const auto t = TypeParam(1);
        weights.clear();
        weights.push_back(TypeParam(1) - t);
        weights.push_back(t);

        auto dc = transforms[1];
        auto res = dlb(transforms, weights);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
    // t == 0.5
    {
        const auto t = TypeParam(0.5);
        weights.clear();
        weights.push_back(TypeParam(1) - t);
        weights.push_back(t);

        auto dc = (TypeParam(1) - t) * transforms[0] + t * transforms[1];
        dc /= norm(dc);
        auto res = dlb(transforms, weights);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
    }
}

}   // namespace
