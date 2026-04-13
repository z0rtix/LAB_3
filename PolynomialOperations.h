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


#endif