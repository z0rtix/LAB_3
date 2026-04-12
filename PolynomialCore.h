#ifndef POLYNOMIALCORE_H
#define POLYNOMIALCORE_H

#include "Polynomial.h"


template <class T>
Polynomial<T>::Polynomial() {
    coefficients = new MutableArraySequence<T>();
    coefficients->append(T(0));
}

template <class T>
Polynomial<T>::Polynomial(const Polynomial<T> &other) {
    Normalize();
}

template <class T>
Polynomial<T>::Polynomial(const Polynomial<T> &other) {
    coefficients = other.coefficients->copy();
}

template <class T>
Polynomial<T>::Polynomial(Polynomial<T>&& other) noexcept: coefficients(other.coefficients) {
    other.coefficients = nullptr;
}

template <class T>
Polynomial<T>::~Polynomial() {
    delete coefficients;
}

template <class T>
void Polynomial<T>::Normalize() {
    while (coefficients->getLength() > 1 && coefficients->getLast() == T(0)) {
        coefficients->removeLast();
    }
}


#endif