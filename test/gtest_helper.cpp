#include "gtest_helper.h"

namespace gtest_helper_detail
{

template<typename T>
constexpr T
abs(T x)
{
    return x < T(0.0) ? -x : x;
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
bool
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
bool
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

template bool almost_equal(float lhs, float rhs, float tolerance);
template bool almost_equal(double lhs, double rhs, double tolerance);

template<typename T>
bool not_almost_equal(T lhs, T rhs, T tolerance)
{
    return !almost_equal(lhs, rhs, tolerance);
}

template bool not_almost_equal(float lhs, float rhs, float tolerance);
template bool not_almost_equal(double lhs, double rhs, double tolerance);

template<typename T>
bool complex_almost_equal(const std::complex<T>& lhs, const std::complex<T>& rhs, T tolerance)
{
    return almost_equal(lhs.real(), rhs.real(), tolerance) && almost_equal(lhs.imag(), rhs.imag(), tolerance);
}

template bool complex_almost_equal(const std::complex<float>& lhs, const std::complex<float>& rhs, float);
template bool complex_almost_equal(const std::complex<double>& lhs, const std::complex<double>& rhs, double);

template<typename T>
bool complex_not_almost_equal(const std::complex<T>& lhs, const std::complex<T>& rhs, T tolerance)
{
    return !complex_almost_equal(lhs, rhs, tolerance);
}

template bool complex_not_almost_equal(const std::complex<float>& lhs, const std::complex<float>& rhs, float);
template bool complex_not_almost_equal(const std::complex<double>& lhs, const std::complex<double>& rhs, double);

}   // namespace gtest_helper_detail
