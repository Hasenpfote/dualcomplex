#include <gtest/gtest.h>
#include <dualcomplex/dualcomplex_base.h>
#include <dualcomplex/dualcomplex_transform.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class DualComplexTransformTest
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
DualComplexTransformTest<T>::PI = std::acos(-T(1));

using MyTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(DualComplexTransformTest, MyTypes);

TYPED_TEST(DualComplexTransformTest, rotation)
{
    using C = std::complex<TypeParam>;

    constexpr auto atol = DualComplexTransformTest<TypeParam>::absolute_tolerance();

    const auto angle = DualComplexTransformTest<TypeParam>::PI;

    auto real = C(std::cos(angle / TypeParam(2)), std::sin(angle / TypeParam(2)));
    auto dual = C(TypeParam(0), TypeParam(0));
    auto res = dcn::rotation(angle);

    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, real, res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dual, res.dual(), atol);
}

TYPED_TEST(DualComplexTransformTest, translation)
{
    using C = std::complex<TypeParam>;

    constexpr auto atol = DualComplexTransformTest<TypeParam>::absolute_tolerance();

    const auto d = C(TypeParam(1), TypeParam(2));

    auto real = C(TypeParam(1), TypeParam(0));
    auto dual = d / TypeParam(2);
    auto res = dcn::translation(d);

    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, real, res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dual, res.dual(), atol);
}

TYPED_TEST(DualComplexTransformTest, transform)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexTransformTest<TypeParam>::absolute_tolerance();

    const auto angle = DualComplexTransformTest<TypeParam>::PI;
    const auto d = C(TypeParam(1), TypeParam(2));
    const auto r = dcn::rotation(angle);
    const auto t = dcn::translation(d);
    const auto v = C(TypeParam(3), TypeParam(4));
    // rotation -> translation
    {
        auto p = t * r;
        auto dc = p * DC(v) * DC(std::conj(p.real()), p.dual());
        auto res = dcn::transform(p, v);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res, atol);
    }
    // translation -> rotation
    {
        auto p = r * t;
        auto dc = p * DC(v) * DC(std::conj(p.real()), p.dual());
        auto res = dcn::transform(p, v);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res, atol);
    }
}

TYPED_TEST(DualComplexTransformTest, transformation_difference)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexTransformTest<TypeParam>::absolute_tolerance();

    const auto angle0 = DualComplexTransformTest<TypeParam>::PI / TypeParam(4);
    const auto d0 = C(TypeParam(1), TypeParam(2));
    const auto angle1 = DualComplexTransformTest<TypeParam>::PI / TypeParam(2);
    const auto d1 = C(TypeParam(3), TypeParam(4));

    auto p = dcn::translation(d0) * dcn::rotation(angle0);
    auto q = dcn::translation(d1) * dcn::rotation(angle1);

    auto dc = DC(std::conj(p.real()), -p.dual()) * q;
    auto res = transformation_difference(p, q);

    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
}

}   // namespace