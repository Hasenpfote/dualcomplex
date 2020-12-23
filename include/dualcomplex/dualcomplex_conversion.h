/**
 * @file dualcomplex/dualcomplex_conversion.h
 * @brief This file provides conversion functions for dual complex types.
 */
#pragma once

#include <Eigen/Geometry>

namespace dcn
{

/**
 * Converts from a dual complex number that represents a tranformation to homogeneous transformation matrix.
 */
template<typename T>
Eigen::Matrix<T, 3, 3>
convert_to_matrix(const DualComplex<T>& dc)
{
    Eigen::Matrix<T, 3, 3> m;
    //
    const auto a = dc.real().real();
    const auto b = dc.real().imag();
    const auto c = dc.dual().real();
    const auto d = dc.dual().imag();

    const auto temp0 = a * a - b * b;
    const auto temp1 = static_cast<T>(2) * a * b;

    m(0,0) = temp0;
    m(0,1) = -temp1;
    m(0,2) = static_cast<T>(2) * (a * c - b * d);

    m(1,0) = temp1;
    m(1,1) = temp0;
    m(1,2) = static_cast<T>(2) * (a * d + b * c);

    m(2,0) = static_cast<T>(0);
    m(2,1) = static_cast<T>(0);
    m(2,2) = static_cast<T>(1);

    return m;
}

}   // namespace dcn