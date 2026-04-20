#ifndef POLYNOMIALOPERATIONS_H
#define POLYNOMIALOPERATIONS_H

#include "Polynomial.h"


template <class T>
Polynomial<T> Polynomial<T>::Derivative() const {
    Polynomial<T> new_polynomial;
    for (int i = 1; i < Degree(); i++) {
        new_polynomial.coefficients->append(GetCoefficient(i) * T(i));
    }
    if (new_polynomial.Degree() > 1) {
        new_polynomial.coefficients->removeFirst();
    }
    return new_polynomial;
}

template <class T>
Polynomial<T> Polynomial<T>::Integral() const {
    Polynomial<T> new_polynomial;
    for (int i = 0; i < Degree(); i++) {
        new_polynomial.coefficients->append(GetCoefficient(i) / T(i + 1));
    }
    return new_polynomial;
}   

template <class T>
Polynomial<T> Polynomial<T>::Pow(int n) const {
    if (n < 0) {
        throw PolynomialException("Negative exponent");
    }
    Polynomial<T> result(T(1));
    Polynomial<T> base(*this);
    while (n > 0) {
        if (n % 2 == 1) {
            result =  result * base
        } else {
            base = base * base
        }
        n = n / 2;
    }
    return result;
}

template <class T>
Polynomial<T> Polynomial<T>::Shift(int k) const {
    if (k < 0) {
        throw PolynomialException("Shift must be non-negative");
    } else if (k == 0) {
        return *this;
    }
    Polynomial<T> result(*this);
    for (int i = 0; i < k; ++i) {
        result.coefficients->prepend(T(0));
    }
    return result;
}

template <class T>
Polynomial<T> Polynomial<T>::ReduceFront(int n) const {
    if (n == 0) return *this;

    int removed = 0;
    Polynomial<T> result(*this);
    while (result.Degree() > 1 && result.GetCoefficient(0) == T(0)) {
        if (n > 0 && removed >= n) break;
        result.coefficients->removeFirst();
        removed++;
    }
    if (n > 0 && removed < n) {
        throw PolynomialException("Not enough leading zeros to remove");
    }
    return result;
}

template <class T>
std::pair<Polynomial<T>, Polynomial<T>> Polynomial<T>::Divide(const Polynomial<T> &other) const {
    if (other.Degree() == 1 && other.GetCoefficient(0) == T(0)) throw PolynomialException("Division by zero polynomial");
    Polynomial<T> remainder(*this);
    Polynomial<T> quotient;
    Polynomial<T> temporary;
    int index = Degree() - 1;
    while (remainder.Degree() >= other.Degree() && index >= 0) {
        T coeff = remainder.coefficients->getLast() / other.coefficients->getLast();
        quotient.coefficients->prepend(coeff);
        temporary = Polynomial(other) * coeff;
        for (int i = other.Degree(); i < remainder.Degree(); i++) {
            temporary.coefficients->prepend(0);
        }
        remainder -= temporary;
        remainder.Normalize();
        quotient.Normalize();
        index--;
    }
    return {quotient, remainder};
}

template <class T>
Polynomial<T> Polynomial<T>::GCD(const Polynomial<T> &other) const {
    Polynomial<T> polynomial1(*this), polynomial2(other);
    while (polynomial2.Degree() != 1 || polynomial2.GetCoefficient(0) != T(0)) {
        auto [_, remainder] = polynomial1.Divide(polynomial2);
        polynomial1 = polynomial2;
        polynomial2 = remainder;
    }
    return polynomial1;
}


#endif