#include <gtest/gtest.h>
#include <dualcomplex/dualcomplex_base.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class DualComplexBaseTest
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
TYPED_TEST_SUITE(DualComplexBaseTest, MyTypes);

TYPED_TEST(DualComplexBaseTest, Accessor)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto real = C(TypeParam(1), TypeParam(2));
    const auto dual = C(TypeParam(3), TypeParam(4));

    DC res;
    res.real() = real;
    res.dual() = dual;

    EXPECT_COMPLEX_ALMOST_EQUAL(real, res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(dual, res.dual(), atol);
}

TYPED_TEST(DualComplexBaseTest, AdditionAssignment)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto c = C(TypeParam(4), TypeParam(3));
    const auto d = C(TypeParam(2), TypeParam(1));
    const auto real = a + c;
    const auto dual = b + d;

    DC lhs(a, b);
    DC rhs(c, d);
    lhs += rhs;

    EXPECT_COMPLEX_ALMOST_EQUAL(real, lhs.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(dual, lhs.dual(), atol);
}

TYPED_TEST(DualComplexBaseTest, SubtractionAssignment)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto c = C(TypeParam(4), TypeParam(3));
    const auto d = C(TypeParam(2), TypeParam(1));
    const auto real = a - c;
    const auto dual = b - d;

    DC lhs(a, b);
    DC rhs(c, d);
    lhs -= rhs;

    EXPECT_COMPLEX_ALMOST_EQUAL(real, lhs.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(dual, lhs.dual(), atol);
}

TYPED_TEST(DualComplexBaseTest, MultiplicationAssignment)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));

    // dc * dc
    {
        const auto c = C(TypeParam(4), TypeParam(3));
        const auto d = C(TypeParam(2), TypeParam(1));

        const auto real = a * c;
        const auto dual = b * std::conj(c) + a * d;

        DC lhs(a, b);
        DC rhs(c, d);
        lhs *= rhs;

        EXPECT_COMPLEX_ALMOST_EQUAL(real, lhs.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(dual, lhs.dual(), atol);
    }
    // dc * scalar
    {
        const auto s = TypeParam(2);
        const auto real = a * s;
        const auto dual = b * s;

        DC lhs(a, b);
        lhs *= s;

        EXPECT_COMPLEX_ALMOST_EQUAL(real, lhs.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(dual, lhs.dual(), atol);
    }
}

TYPED_TEST(DualComplexBaseTest, DivisionAssignment)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));

    // dc / dc
    {
        const auto c = C(TypeParam(4), TypeParam(3));
        const auto d = C(TypeParam(2), TypeParam(1));

        const auto sn = std::norm(c);
        const auto real = (a * std::conj(c)) / sn;
        const auto dual = (b * c - a * d) / sn;

        DC lhs(a, b);
        DC rhs(c, d);
        lhs /= rhs;

        EXPECT_COMPLEX_ALMOST_EQUAL(real, lhs.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(dual, lhs.dual(), atol);
    }
    // dc * scalar
    {
        const auto s = TypeParam(2);
        const auto real = a / s;
        const auto dual = b / s;

        DC lhs(a, b);
        lhs /= s;

        EXPECT_COMPLEX_ALMOST_EQUAL(real, lhs.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(dual, lhs.dual(), atol);
    }
}

TYPED_TEST(DualComplexBaseTest, UnaryPlus)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto real = a;
    const auto dual = b;

    auto res = +DC(a, b);

    EXPECT_COMPLEX_ALMOST_EQUAL(real, res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(dual, res.dual(), atol);
}

TYPED_TEST(DualComplexBaseTest, UnaryMinus)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto real = -a;
    const auto dual = -b;

    auto res = -DC(a, b);

    EXPECT_COMPLEX_ALMOST_EQUAL(real, res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(dual, res.dual(), atol);
}

TYPED_TEST(DualComplexBaseTest, Addition)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto c = C(TypeParam(4), TypeParam(3));
    const auto d = C(TypeParam(2), TypeParam(1));
    const auto real = a + c;
    const auto dual = b + d;

    auto res = DC(a, b) + DC(c, d);

    EXPECT_COMPLEX_ALMOST_EQUAL(real, res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(dual, res.dual(), atol);
}

TYPED_TEST(DualComplexBaseTest, Subtraction)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));
    const auto c = C(TypeParam(4), TypeParam(3));
    const auto d = C(TypeParam(2), TypeParam(1));
    const auto real = a - c;
    const auto dual = b - d;

    auto res = DC(a, b) - DC(c, d);

    EXPECT_COMPLEX_ALMOST_EQUAL(real, res.real(), atol);
    EXPECT_COMPLEX_ALMOST_EQUAL(dual, res.dual(), atol);
}

TYPED_TEST(DualComplexBaseTest, Multiplication)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));

    // dc * dc
    {
        const auto c = C(TypeParam(4), TypeParam(3));
        const auto d = C(TypeParam(2), TypeParam(1));

        const auto real = a * c;
        const auto dual = b * std::conj(c) + a * d;

        auto res = DC(a, b) * DC(c, d);

        EXPECT_COMPLEX_ALMOST_EQUAL(real, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(dual, res.dual(), atol);

    }
    // dc * scalar
    {
        const auto s = TypeParam(2);
        const auto real = a * s;
        const auto dual = b * s;

        auto res = DC(a, b) * s;

        EXPECT_COMPLEX_ALMOST_EQUAL(real, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(dual, res.dual(), atol);
    }
    // scalar * dc
    {
        const auto s = TypeParam(2);
        const auto real = a * s;
        const auto dual = b * s;

        auto res = s * DC(a, b);

        EXPECT_COMPLEX_ALMOST_EQUAL(real, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(dual, res.dual(), atol);
    }
}

TYPED_TEST(DualComplexBaseTest, Division)
{
    using C = std::complex<TypeParam>;
    using DC = dcn::DualComplex<TypeParam>;

    constexpr auto atol = DualComplexBaseTest<TypeParam>::absolute_tolerance();

    const auto a = C(TypeParam(1), TypeParam(2));
    const auto b = C(TypeParam(3), TypeParam(4));

    // dc / dc
    {
        const auto c = C(TypeParam(4), TypeParam(3));
        const auto d = C(TypeParam(2), TypeParam(1));

        const auto sn = std::norm(c);
        const auto real = (a * std::conj(c)) / sn;
        const auto dual = (b * c - a * d) / sn;

        auto res = DC(a, b) / DC(c, d);

        EXPECT_COMPLEX_ALMOST_EQUAL(real, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(dual, res.dual(), atol);
    }
    // dc * scalar
    {
        const auto s = TypeParam(2);
        const auto real = a / s;
        const auto dual = b / s;

        auto res = DC(a, b) / s;

        EXPECT_COMPLEX_ALMOST_EQUAL(real, res.real(), atol);
        EXPECT_COMPLEX_ALMOST_EQUAL(dual, res.dual(), atol);
    }
}

}   // namespace
