#ifndef POLYNOMIALOPERATIONS_H
#define POLYNOMIALOPERATIONS_H

#include "Polynomial.h"


template <class T>
Polynomial<T> Polynomial<T>::Derivative() const {
    Polynomial<T> new_polynomial;
    for (int i = 1; i < Degree(); i++) {
        new_polynomial.coefficients->append(GetCoefficient(i) * i);
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
        new_polynomial.coefficients->append(GetCoefficient(i) / (i + 1));
    }
    return new_polynomial;
}

template <class T>
std::pair<Polynomial<T>, Polynomial<T>> Polynomial<T>::Divide(const Polynomial<T> &other) const {
    Polynomial<T> quotient(*this);
    Polynomial<T> remainder;
    Polynomial<T> temporary;
    int index = Degree() - 1;
    while (quotient.Degree() >= other.Degree() && index >= 0) {
        T coeff = quotient.coefficients->getLast() / other.coefficients->getLast();
        remainder.coefficients->prepend(coeff);
        temporary = Polynomial(other) * coeff;
        for (int i = other.Degree(); i < quotient.Degree(); i++) {
            temporary.coefficients->prepend(0);
        }
        quotient -= temporary;
        index--;
        quotient.Normalize();
    }
    return {quotient, remainder};
}


#endif