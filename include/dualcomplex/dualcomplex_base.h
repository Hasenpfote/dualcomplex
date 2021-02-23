/**
 * @file dualcomplex/dualcomplex_base.h
 */
#pragma once

#include <complex>

namespace dcn
{

template<typename T>
class DualComplex
{
    static_assert(std::is_floating_point<T>::value,
        "Template parameter T must be floating_point type.");
public:
    using value_type = T;

/* Constructors */
    DualComplex()
    {}

    /**
     * Constructs a dual complex number from two complex numbers,
     * using one as the real and the other one as the dual part.
     */
    DualComplex(const std::complex<T>& real, const std::complex<T>& dual)
        : real_(real), dual_(dual)
    {}

    /**
     * Constructs a dual complex number from four real numbers.
     */
    DualComplex(T a, T b, T c, T d)
        : DualComplex(std::complex<T>(a, b), std::complex<T>(c, d))
    {}

    /**
     * Constructs a dual complex number from a vector.
     */
    explicit DualComplex(const std::complex<T>& v)
        : DualComplex(std::complex<T>(static_cast<T>(1), static_cast<T>(0)), v)
    {}

/* Accessors */
    const std::complex<T>& real() const noexcept { return real_; }
    const std::complex<T>& dual() const noexcept { return dual_; }

    std::complex<T>& real() noexcept { return real_; }
    std::complex<T>& dual() noexcept { return dual_; }

/* Assignment operators */
    DualComplex& operator += (const DualComplex&);
    DualComplex& operator -= (const DualComplex&);
    DualComplex& operator *= (const DualComplex&);
    DualComplex& operator *= (T);
    DualComplex& operator /= (const DualComplex&);
    DualComplex& operator /= (T);

private:
    std::complex<T> real_;
    std::complex<T> dual_;
};

/* Assignment operators */

template<typename T>
DualComplex<T>&
DualComplex<T>::operator += (const DualComplex& rhs)
{
    real_ += rhs.real_;
    dual_ += rhs.dual_;
    return *this;
}

template<typename T>
DualComplex<T>&
DualComplex<T>::operator -= (const DualComplex& rhs)
{
    real_ -= rhs.real_;
    dual_ -= rhs.dual_;
    return *this;
}

template<typename T>
DualComplex<T>&
DualComplex<T>::operator *= (const DualComplex& rhs)
{
    auto temp = real_;
    real_ = temp * rhs.real_;
    dual_ = dual_ * std::conj(rhs.real_) + temp * rhs.dual_;
    return *this;
}

template<typename T>
DualComplex<T>&
DualComplex<T>::operator *= (T rhs)
{
    real_ *= rhs;
    dual_ *= rhs;
    return *this;
}

template<typename T>
DualComplex<T>&
DualComplex<T>::operator /= (const DualComplex& rhs)
{
    auto temp = real_;
    auto sn = std::norm(rhs.real_);
    real_ = (temp * std::conj(rhs.real_)) / sn;
    dual_ = (dual_ * rhs.real_ - temp * rhs.dual_) / sn;
    return *this;
}

template<typename T>
DualComplex<T>&
DualComplex<T>::operator /= (T rhs)
{
    real_ /= rhs;
    dual_ /= rhs;
    return *this;
}

/* Unary operators */

template<typename T>
DualComplex<T>
operator + (const DualComplex<T>& dc)
{
    return dc;
}

template<typename T>
DualComplex<T>
operator - (const DualComplex<T>& dc)
{
    return DualComplex<T>(-dc.real(), -dc.dual());
}

/* Binary operators */

template<typename T>
DualComplex<T>
operator + (const DualComplex<T>& lhs, const DualComplex<T>& rhs)
{
    DualComplex<T> temp(lhs);
    return temp += rhs;
}

template<typename T>
DualComplex<T>
operator - (const DualComplex<T>& lhs, const DualComplex<T>& rhs)
{
    DualComplex<T> temp(lhs);
    return temp -= rhs;
}

template<typename T>
DualComplex<T>
operator * (const DualComplex<T>& lhs, const DualComplex<T>& rhs)
{
    DualComplex<T> temp(lhs);
    return temp *= rhs;
}

template<typename T>
DualComplex<T>
operator * (const DualComplex<T>& lhs, T rhs)
{
    DualComplex<T> temp(lhs);
    return temp *= rhs;
}

template<typename T>
DualComplex<T>
operator * (T lhs, const DualComplex<T>& rhs)
{
    DualComplex<T> temp(rhs);
    return temp *= lhs;
}

template<typename T>
DualComplex<T>
operator / (const DualComplex<T>& lhs, const DualComplex<T>& rhs)
{
    DualComplex<T> temp(lhs);
    return temp /= rhs;
}

template<typename T>
DualComplex<T>
operator / (const DualComplex<T>& lhs, T rhs)
{
    DualComplex<T> temp(lhs);
    return temp /= rhs;
}

}   // namespace dcn
