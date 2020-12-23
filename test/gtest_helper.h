#pragma once
#include <complex>
#include <gtest/gtest.h>

namespace gtest_helper_detail
{

template<typename T>
bool almost_equal(T lhs, T rhs, T tolerance);

extern template bool almost_equal(float lhs, float rhs, float tolerance);
extern template bool almost_equal(double lhs, double rhs, double tolerance);

template<typename T>
bool not_almost_equal(T lhs, T rhs, T tolerance);

extern template bool not_almost_equal(float lhs, float rhs, float tolerance);
extern template bool not_almost_equal(double lhs, double rhs, double tolerance);

template<typename T>
bool complex_almost_equal(const std::complex<T>& lhs, const std::complex<T>& rhs, T tolerance);

extern template bool complex_almost_equal(const std::complex<float>& lhs, const std::complex<float>& rhs, float);
extern template bool complex_almost_equal(const std::complex<double>& lhs, const std::complex<double>& rhs, double);

template<typename T>
bool complex_not_almost_equal(const std::complex<T>& lhs, const std::complex<T>& rhs, T tolerance);

extern template bool complex_not_almost_equal(const std::complex<float>& lhs, const std::complex<float>& rhs, float);
extern template bool complex_not_almost_equal(const std::complex<double>& lhs, const std::complex<double>& rhs, double);

}   // namespace gtest_helper_detail

#define EXPECT_ALMOST_EQUAL(type, val1, val2, tolerance) \
    EXPECT_PRED3(gtest_helper_detail::almost_equal<type>, val1, val2, tolerance)

#define EXPECT_NOT_ALMOST_EQUAL(type, val1, val2, tolerance) \
    EXPECT_PRED3(gtest_helper_detail::not_almost_equal<type>, val1, val2, tolerance)

#define EXPECT_COMPLEX_ALMOST_EQUAL(type, val1, val2, tolerance) \
    EXPECT_PRED3(gtest_helper_detail::complex_almost_equal<type>, val1, val2, tolerance)

#define EXPECT_COMPLEX_NOT_ALMOST_EQUAL(type, val1, val2, tolerance) \
    EXPECT_PRED3(gtest_helper_detail::complex_not_almost_equal<type>, val1, val2, tolerance)
