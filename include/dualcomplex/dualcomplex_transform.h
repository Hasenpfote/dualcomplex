/**
 * @file dualcomplex/dualcomplex_transform.h
 * @brief This file provides transform functions for dual complex types.
 */
#pragma once

#include <cmath>
#include "dualcomplex_common.h"

namespace dcn
{

/**
 * Returns a transformation that represents a rotation from the given angle.
 */
template<typename T>
DualComplex<T>
rotation(T angle)
{
    const auto half_angle = angle / static_cast<T>(2);
    return DualComplex<T>(
        std::complex<T>(std::cos(half_angle), std::sin(half_angle)),
        std::complex<T>(static_cast<T>(0), static_cast<T>(0)));
}

/**
 * Returns a transformation that represents a translation from the given displacement.
 */
template<typename T>
DualComplex<T>
translation(const std::complex<T>& d)
{
    return DualComplex<T>(d / static_cast<T>(2));
}

/**
 * Transform a vector with a dual complex number.
 */
template<typename T>
std::complex<T>
transform(const DualComplex<T>& p, const std::complex<T>& v)
{
#if 0
    return (p * DualComplex<T>(v) * complex_conjugate(p)).real();
#else
    return p.real() * p.real() * v + static_cast<T>(2) * p.real() * p.dual();
#endif
}

/**
 * Difference between two transformations.
 */
template<typename T>
DualComplex<T>
transformation_difference(const DualComplex<T>& p, const DualComplex<T>& q)
{
    return total_conjugate(p) * q;
}

}   // namespace dcn
