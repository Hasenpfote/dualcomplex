#pragma once

#include <complex>
#include <gtest/gtest.h>

namespace gtest_helper
{

namespace detail
{

template<typename T>
constexpr T abs(T x)
{
    return x < T(0.0) ? -x : x;
}

#if ((defined(_MSVC_LANG) && _MSVC_LANG < 201402L) || __cplusplus < 201402L)
template <class T>
constexpr const T& max(const T& a, const T& b)
{
    return a < b ? b : a;
}
#endif

template<typename T>
bool almost_equal(T lhs, T rhs, T rel_tolerance, T abs_tolerance)
{
#if (__cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L))
    return abs(lhs - rhs)
        <= std::max(abs_tolerance, rel_tolerance * std::max(abs(lhs), abs(rhs)));
#else
    return abs(lhs - rhs)
        <= max(abs_tolerance, rel_tolerance * max(abs(lhs), abs(rhs)));
#endif
}

template<typename T>
bool almost_equal(T lhs, T rhs, T tolerance)
{
#if (__cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L))
    return abs(lhs - rhs)
        <= tolerance * std::max(T(1), std::max(abs(lhs), abs(rhs)));
#else
    return abs(lhs - rhs)
        <= tolerance * max(T(1), max(abs(lhs), abs(rhs)));
#endif
}

template<typename T>
bool not_almost_equal(T lhs, T rhs, T tolerance)
{
    return !almost_equal(lhs, rhs, tolerance);
}

template<typename T>
::testing::AssertionResult
assert_almost_equal_pred3_format(
    const char* e1,
    const char* e2,
    const char* e3,
    T v1,
    T v2,
    T v3)
{
    if(almost_equal(v1, v2, v3))
        return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure()
        << "pred_text" << "(" << e1 << ", " << e2 << ", " << e3
        << ") evaluates to false, where"
        << "\n"
        << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
        << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
        << e3 << " evaluates to " << ::testing::PrintToString(v3);
}

template<typename T>
::testing::AssertionResult
assert_not_almost_equal_pred3_format(
    const char* e1,
    const char* e2,
    const char* e3,
    T v1,
    T v2,
    T v3)
{
    if(!almost_equal(v1, v2, v3))
        return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure()
        << "pred_text" << "(" << e1 << ", " << e2 << ", " << e3
        << ") evaluates to false, where"
        << "\n"
        << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
        << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
        << e3 << " evaluates to " << ::testing::PrintToString(v3);
}

template<typename T>
bool complex_almost_equal(const std::complex<T>& lhs, const std::complex<T>& rhs, T tolerance)
{
    return almost_equal(lhs.real(), rhs.real(), tolerance) && almost_equal(lhs.imag(), rhs.imag(), tolerance);
}

template<typename T>
bool complex_not_almost_equal(const std::complex<T>& lhs, const std::complex<T>& rhs, T tolerance)
{
    return !complex_almost_equal(lhs, rhs, tolerance);
}

template<typename T>
::testing::AssertionResult
assert_complex_almost_equal_pred3_format(
    const char* e1,
    const char* e2,
    const char* e3,
    const std::complex<T>& v1,
    const std::complex<T>& v2,
    T v3)
{
    if(complex_almost_equal(v1, v2, v3))
        return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure()
        << "pred_text" << "(" << e1 << ", " << e2 << ", " << e3
        << ") evaluates to false, where"
        << "\n"
        << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
        << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
        << e3 << " evaluates to " << ::testing::PrintToString(v3);
}

template<typename T>
::testing::AssertionResult
assert_not_complex_almost_equal_pred3_format(
    const char* e1,
    const char* e2,
    const char* e3,
    const std::complex<T>& v1,
    const std::complex<T>& v2,
    T v3)
{
    if(!complex_almost_equal(v1, v2, v3))
        return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure()
        << "pred_text" << "(" << e1 << ", " << e2 << ", " << e3
        << ") evaluates to false, where"
        << "\n"
        << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
        << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
        << e3 << " evaluates to " << ::testing::PrintToString(v3);
}

}   // namespace detail

}   // namespace gtest_helper

#define EXPECT_ALMOST_EQUAL(lhs, rhs, tolerance) \
    EXPECT_PRED_FORMAT3(gtest_helper::detail::assert_almost_equal_pred3_format, lhs, rhs, tolerance)

#define EXPECT_NOT_ALMOST_EQUAL(lhs, rhs, tolerance) \
    EXPECT_PRED_FORMAT3(gtest_helper::detail::assert_not_almost_equal_pred3_format, lhs, rhs, tolerance)

#define EXPECT_COMPLEX_ALMOST_EQUAL(lhs, rhs, tolerance) \
    EXPECT_PRED_FORMAT3(gtest_helper::detail::assert_complex_almost_equal_pred3_format, lhs, rhs, tolerance)

#define EXPECT_COMPLEX_NOT_ALMOST_EQUAL(lhs, rhs, tolerance) \
    EXPECT_PRED_FORMAT3(gtest_helper::detail::assert_complex_not_almost_equal_pred3_format, lhs, rhs, tolerance)
