/**
 * @file dualcomplex/dualcomplex_exponential.h
 * @brief This file provides exponential functions for dual complex types.
 */
#pragma once

namespace dcn
{

/**
 * Returns a exponential of a dual complex number.
 */
template<typename T>
DualComplex<T>
exp(const DualComplex<T>& dc)
{
    const auto temp = std::exp(dc.real());
    return DualComplex<T>(temp, temp * dc.dual());
}

/**
 * Returns a logarithm of a dual complex number.
 */
template<typename T>
DualComplex<T>
log(const DualComplex<T>& dc)
{
    return DualComplex<T>(std::log(dc.real()), dc.dual() / dc.real());
}

/**
 * Returns a dual complex number raised to a power.
 */
template<typename T>
DualComplex<T>
pow(const DualComplex<T>& base, T exponent)
{
#if 0
    return exp(exponent * log(base));
#else
    return DualComplex<T>(
        std::pow(base.real(), exponent),
        exponent * std::pow(base.real(), exponent - static_cast<T>(1)) * base.dual());
#endif
}

}   // namespace dcn
