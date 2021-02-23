/**
 * @file dualcomplex/dualcomplex_common.h
 * @brief This file provides common functions for dual complex types.
 */
#pragma once

namespace dcn
{

template<typename T>
T
squared_norm(const DualComplex<T>& dc)
{
    return std::norm(dc.real());
}

template<typename T>
T
norm(const DualComplex<T>& dc)
{
    return std::abs(dc.real());
}

template<typename T>
DualComplex<T>
inverse(const DualComplex<T>& dc)
{
    const auto sn = std::norm(dc.real());
    return DualComplex<T>(std::conj(dc.real()) / sn, -dc.dual() / sn);
}

template<typename T>
DualComplex<T>
complex_conjugate(const DualComplex<T>& dc)
{
    return DualComplex<T>(std::conj(dc.real()), dc.dual());
}

template<typename T>
DualComplex<T>
dual_conjugate(const DualComplex<T>& dc)
{
    return DualComplex<T>(dc.real(), -dc.dual());
}

template<typename T>
DualComplex<T>
total_conjugate(const DualComplex<T>& dc)
{
    return DualComplex<T>(std::conj(dc.real()), -dc.dual());
}

/**
 * Returns the normalized version of a dual complex number.
 */
template<typename T>
DualComplex<T>
normalize(const DualComplex<T>& dc)
{
    return dc / norm(dc);
}

}   // namespace dcn
