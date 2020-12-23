/**
 * @file dualcomplex/dualcomplex_interpolation.h
 * @brief This file provides interpolation functions for dual complex types.
 */
#pragma once

#include "dualcomplex_common.h"
#include "dualcomplex_exponential.h"
#include "dualcomplex_transform.h"

namespace dcn
{

template<typename T>
DualComplex<T>
slerp(const DualComplex<T>& dc0, const DualComplex<T>& dc1, T t)
{
    return dc0 * pow(transformation_difference(dc0, dc1), t);
}

template<typename T>
DualComplex<T>
slerp_shortestpath(const DualComplex<T>& dc0, const DualComplex<T>& dc1, T t)
{
    auto dot = [](const std::complex<T>& c0, const std::complex<T>& c1)
    {
        return c0.real() * c1.real() + c0.imag() * c1.imag();
    };

    const auto cos_half_angle = dot(dc0.real(), dc1.real());
    return slerp(dc0, (cos_half_angle < static_cast<T>(0))? -dc1 : dc1, t);
}

}   // namespace dcn