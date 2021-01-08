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

    const auto r_squared = dc.real() * dc.real();
    const auto twice_rd = static_cast<T>(2) * dc.real() * dc.dual();

    m(0,0) = r_squared.real();
    m(0,1) =-r_squared.imag();
    m(0,2) = twice_rd.real();

    m(1,0) = r_squared.imag();
    m(1,1) = r_squared.real();
    m(1,2) = twice_rd.imag();

    m(2,0) = static_cast<T>(0);
    m(2,1) = static_cast<T>(0);
    m(2,2) = static_cast<T>(1);

    return m;
}

}   // namespace dcn