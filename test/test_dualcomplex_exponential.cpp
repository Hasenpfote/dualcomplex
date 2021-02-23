#include <gtest/gtest.h>
#include <dualcomplex/dualcomplex_base.h>
#include <dualcomplex/dualcomplex_exponential.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class DualComplexExponentialTest
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
TYPED_TEST_SUITE(DualComplexExponentialTest, MyTypes);

TYPED_TEST(DualComplexExponentialTest, exp)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexExponentialTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));

    const auto temp = std::exp(a);
    auto real = temp;
    auto dual = temp * b;
    auto res = exp(DC(a, b));

    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, real, res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dual, res.dual(), atol);
}

TYPED_TEST(DualComplexExponentialTest, log)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexExponentialTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));

    auto real = std::log(a);
    auto dual = b / a;
    auto res = log(DC(a, b));

    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, real, res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dual, res.dual(), atol);
}

TYPED_TEST(DualComplexExponentialTest, exp_log)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexExponentialTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));
    // log(exp(dc)) == dc
    {
        auto res = log(exp(DC(a, b)));

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, a, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, b, res.dual(), atol);
    }
    // exp(log(dc)) == dc
    {
        auto res = exp(log(DC(a, b)));

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, a, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, b, res.dual(), atol);
    }
}

TYPED_TEST(DualComplexExponentialTest, pow)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexExponentialTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto exponent = TypeParam(2);
    const auto base = DC(a, b);

    auto dc = exp(exponent * log(base));
    auto res = pow(base, exponent);

    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.real(), res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dc.dual(), res.dual(), atol);
}

}   // namespace
