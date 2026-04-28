#ifndef POLYNOMIALOPERATIONS_H
#define POLYNOMIALOPERATIONS_H

#include "Polynomial.h"


template <class T>
Polynomial<T> Polynomial<T>::Derivative() const {
    Polynomial<T> derivative;

    derivative.coefficients->clear();

    for (int i = 1; i < Length(); i++) {
        derivative.coefficients->append(GetCoefficient(i) * T(i));
    }

    if (derivative.coefficients->getLength() == 0) {
        derivative.coefficients->append(T(0));
    }

    return derivative;
}

template <class T>
Polynomial<T> Polynomial<T>::Integral() const {
    Polynomial<T> integral;

    for (int i = 0; i < Length(); i++) {
        integral.coefficients->append(GetCoefficient(i) / T(i + 1));
    }

    return integral;
}   

template <class T>
Polynomial<T> Polynomial<T>::Pow(int exponent) const {
    if (exponent < 0) throw PolynomialException("Negative exponent");

    Polynomial<T> power(T(1));
    Polynomial<T> base(*this);

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            power =  power * base;
        }
        base = base * base;
        exponent = exponent / 2;
    }

    return power;
}

template <class T>
Polynomial<T> Polynomial<T>::Shift(int exponent) const {
    if (exponent < 0) {
        throw PolynomialException("Shift must be non-negative");
    } else if (exponent == 0) {
        return *this;
    }

    Polynomial<T> shifted(*this);

    for (int i = 0; i < exponent; i++) {
        shifted.coefficients->prepend(T(0));
    }

    return shifted;
}

template <class T>
Polynomial<T> Polynomial<T>::ReduceFront(int exponent) const {
    if (exponent == 0) return *this;

    int removed = 0;
    Polynomial<T> reduced(*this);

    while (reduced.Degree() > 0 && reduced.GetCoefficient(0) == T(0)) {
        if (exponent > 0 && removed >= exponent) break;
        reduced.coefficients->removeFirst();
        removed++;
    }

    if (exponent > 0 && removed < exponent) {
        throw PolynomialException("Not enough leading zeros to remove");
    }

    return reduced;
}

template <class T>
std::pair<Polynomial<T>, Polynomial<T>> Polynomial<T>::Divide(const Polynomial<T> &other) const {
    if (other.Degree() == 0 && other.GetCoefficient(0) == T(0)) {
        throw PolynomialException("Division by zero polynomial");
    }

    Polynomial<T> remainder(*this);
    Polynomial<T> quotient;
    Polynomial<T> temporary;

    int index = Degree();
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
    
    while (polynomial2.Degree() != 0 || polynomial2.GetCoefficient(0) != T(0)) {
        auto [_, remainder] = polynomial1.Divide(polynomial2);
        polynomial1 = polynomial2;
        polynomial2 = remainder;
    }

    return polynomial1;
}


#endif