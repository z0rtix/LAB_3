#ifndef POLYNOMIALCORE_H
#define POLYNOMIALCORE_H

#include "Polynomial.h"


template <class T>
Polynomial<T>::Polynomial() : coefficients(new MutableArraySequence<T>()) {
    coefficients->Append(T(0));
}


#endif