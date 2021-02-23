#include <gtest/gtest.h>
#include <dualcomplex/dualcomplex_base.h>
#include <dualcomplex/dualcomplex_common.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class DualComplexCommonTest
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
TYPED_TEST_SUITE(DualComplexCommonTest, MyTypes);

TYPED_TEST(DualComplexCommonTest, squared_norm)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexCommonTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));

    const auto sn = std::norm(a);
    auto res = squared_norm(DC(a, b));

    EXPECT_ALMOST_EQUAL(TypeParam, sn, res, atol);
}

TYPED_TEST(DualComplexCommonTest, norm)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexCommonTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));

    const auto sn = std::sqrt(std::norm(a));
    auto res = norm(DC(a, b));

    EXPECT_ALMOST_EQUAL(TypeParam, sn, res, atol);
}

TYPED_TEST(DualComplexCommonTest, inverse)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexCommonTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto dc = DC(a, b);
    const auto inv = inverse(dc);
    //
    {
        auto sn = std::norm(a);
        auto real = std::conj(a) / sn;
        auto dual = -b / sn;
        auto res = inv;

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, real, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dual, res.dual(), atol);
    }
    // inv * dc
    {
        auto res = inv * dc;
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, C(TypeParam(1), TypeParam(0)), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, C(TypeParam(0), TypeParam(0)), res.dual(), atol);
    }
    // dc * inv
    {
        auto res = dc * inv;
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, C(TypeParam(1), TypeParam(0)), res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, C(TypeParam(0), TypeParam(0)), res.dual(), atol);
    }
}

TYPED_TEST(DualComplexCommonTest, complex_conjugate)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexCommonTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(0));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto dc = DC(a, b);
    const auto conj = complex_conjugate(dc);
    //
    {
        auto real = std::conj(a);
        auto dual = b;
        auto res = conj;

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, real, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dual, res.dual(), atol);
    }
}

TYPED_TEST(DualComplexCommonTest, dual_conjugate)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexCommonTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(0));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto dc = DC(a, b);
    const auto conj = dual_conjugate(dc);
    //
    {
        auto real = a;
        auto dual = -b;
        auto res = conj;

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, real, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dual, res.dual(), atol);
    }
}

TYPED_TEST(DualComplexCommonTest, total_conjugate)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexCommonTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(0));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto dc = DC(a, b);
    const auto conj = total_conjugate(dc);
    //
    {
        auto real = std::conj(a);
        auto dual = -b;
        auto res = conj;

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, real, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dual, res.dual(), atol);
    }
}

TYPED_TEST(DualComplexCommonTest, normalize)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexCommonTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto dc = DC(a, b);

    {
        auto real = a / std::abs(a);
        auto dual = b / std::abs(a);
        auto res = normalize(dc);

        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, real, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, dual, res.dual(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(1), std::norm(res.real()), atol);
    }
    {
        auto ndc = normalize(dc);
        auto inv = DC(std::conj(ndc.real()), -ndc.dual());

        auto inv_ndc = inv * ndc;
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, C(TypeParam(1), TypeParam(0)), inv_ndc.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, C(TypeParam(0), TypeParam(0)), inv_ndc.dual(), atol);

        auto ndc_inv = ndc * inv;
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, C(TypeParam(1), TypeParam(0)), ndc_inv.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, C(TypeParam(0), TypeParam(0)), ndc_inv.dual(), atol);
    }
}

}   // namespace
