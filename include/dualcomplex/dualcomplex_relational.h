/**
 * @file dualcomplex/dualcomplex_relational.h
 * @brief This file provides relational functions for dual complex types.
 */
#pragma once

#include <cmath>

namespace dcn
{

namespace detail
{

template<typename T>
constexpr T
abs(T x)
{
    return x < static_cast<T>(0) ? -x : x;
}

#if ((defined(_MSVC_LANG) && _MSVC_LANG < 201402L) || __cplusplus < 201402L)
template <class T>
constexpr const T&
max(const T& a, const T& b)
{
    return a < b ? b : a;
}
#endif

template<typename T>
constexpr bool
almost_equal(T lhs, T rhs, T rel_tolerance, T abs_tolerance)
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
constexpr bool
almost_equal(T lhs, T rhs, T tolerance)
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
constexpr bool
almost_zero(T x, T tolerance)
{
    return abs(x) <= tolerance;
}

/**
 * Returns true if two complex numbers are element-wise equal within tolerance.
 */
template<typename T>
constexpr bool
almost_equal(const std::complex<T>& lhs, const std::complex<T>& rhs, T rel_tolerance, T abs_tolerance)
{
    return almost_equal(lhs.real(), rhs.real(), rel_tolerance, abs_tolerance)
        && almost_equal(lhs.imag(), rhs.imag(), rel_tolerance, abs_tolerance);
}

/**
 * Returns true if two complex numbers are element-wise equal within tolerance.
 */
template<typename T>
constexpr bool
almost_equal(const std::complex<T>& lhs, const std::complex<T>& rhs, T tolerance)
{
    return almost_equal(lhs.real(), rhs.real(), tolerance)
        && almost_equal(lhs.imag(), rhs.imag(), tolerance);
}

/**
 * Returns true if a complex number is element-wise equal to zero within tolerance.
 */
template<typename T>
constexpr bool
almost_zero(const std::complex<T>& z, T tolerance)
{
    return almost_zero(z.real(), tolerance)
        && almost_zero(z.imag(), tolerance);
}

}   // namespace detail

/**
 * Returns true if two dual complex numbers are element-wise equal within tolerance.
 */
template<typename T>
bool
almost_equal(const DualComplex<T>& lhs, const DualComplex<T>& rhs, T rel_tolerance, T abs_tolerance)
{
    return detail::almost_equal(lhs.real(), rhs.real(), rel_tolerance, abs_tolerance)
        && detail::almost_equal(lhs.dual(), rhs.dual(), rel_tolerance, abs_tolerance);
}

/**
 * Returns true if two dual complex numbers are element-wise equal within tolerance.
 */
template<typename T>
bool
almost_equal(const DualComplex<T>& lhs, const DualComplex<T>& rhs, T tolerance)
{
    return detail::almost_equal(lhs.real(), rhs.real(), tolerance)
        && detail::almost_equal(lhs.dual(), rhs.dual(), tolerance);
}

/**
 * Returns true if a dual complex number is element-wise equal to zero within tolerance.
 */
template<typename T>
bool
almost_zero(const DualComplex<T>& dc, T tolerance)
{
    return detail::almost_zero(dc.real(), tolerance)
        && detail::almost_zero(dc.dual(), tolerance);
}

}   // namespace dcn