/**
 * @file dualcomplex/dualcomplex_query.h
 * @brief This file provides query functions for dual complex types.
 */
#pragma once

#include <cassert>
#include "dualcomplex_relational.h"

namespace dcn
{

namespace detail
{

/**
 * Returns true if a complex number is the additive identity.
 */
template<typename T>
constexpr bool
is_zero(const std::complex<T>& z, T tolerance)
{
    return almost_zero(z.real(), tolerance)
        && almost_zero(z.imag(), tolerance);
}

/**
 * Returns true if a complex number is the multiplicative identity.
 */
template<typename T>
constexpr bool
is_identity(const std::complex<T>& z, T tolerance)
{
    return almost_equal(z.real(), static_cast<T>(1), tolerance)
        && almost_zero(z.imag(), tolerance);
}

}   // namespace detail

/**
 * Returns true if a complex number is the additive identity.
 */
template<typename T>
bool
is_zero(const DualComplex<T>& dc, T tolerance)
{
    return detail::is_zero(dc.real(), tolerance)
        && detail::is_zero(dc.dual(), tolerance);
}

/**
 * Returns true if a dual complex number is the multiplicative identity.
 */
template<typename T>
bool
is_identity(const DualComplex<T>& dc, T tolerance)
{
    return detail::is_identity(dc.real(), tolerance)
        && detail::is_zero(dc.dual(), tolerance);
}

/**
 * Returns true if an unit dual complex number.
 */
template<typename T>
bool
is_unit(const DualComplex<T>& dc, T tolerance)
{
    return detail::almost_equal(std::norm(dc.real()), static_cast<T>(1), tolerance);
}

/**
 * Returns true if the two dual complex numbers represent the same transformation.
 * Note that every transformation is represented by two values, dc and -dc.
 */
template<typename T>
bool
are_same(const DualComplex<T>& dc0, const DualComplex<T>& dc1, T tolerance)
{
    assert(is_unit(dc0, tolerance) && is_unit(dc1, tolerance));

    return almost_equal(dc0, dc1, tolerance)
        || almost_equal(dc0,-dc1, tolerance);
}

}   // namespace dcn
