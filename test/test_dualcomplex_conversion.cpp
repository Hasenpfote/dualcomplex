#include <gtest/gtest.h>
#include <dualcomplex/dualcomplex_base.h>
#include <dualcomplex/dualcomplex_transform.h>
#include <dualcomplex/dualcomplex_conversion.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class DualComplexConversionTest
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
DualComplexConversionTest<T>::PI = std::acos(-T(1));

using MyTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(DualComplexConversionTest, MyTypes);

TYPED_TEST(DualComplexConversionTest, convert_to_matrix)
{
    using C = std::complex<TypeParam>;

    constexpr auto atol = DualComplexConversionTest<TypeParam>::absolute_tolerance();

    const auto angle = DualComplexConversionTest<TypeParam>::PI / TypeParam(4);
    const auto d = C(TypeParam(1), TypeParam(2));
    const auto v = C(TypeParam(3), TypeParam(4));

    auto p = dcn::translation(d) * dcn::rotation(angle);
    auto pv = transform(p, v);

    auto vec = Eigen::Matrix<TypeParam, 3, 1>(v.real(), v.imag(), TypeParam(1));
    auto m = convert_to_matrix(p);
    auto temp = m * vec;
    auto mv = C(temp.x(), temp.y());

    EXPECT_COMPLEX_ALMOST_EQUAL(TypeParam, pv, mv, atol);
}

}   // namespace
