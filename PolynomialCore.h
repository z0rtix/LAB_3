#ifndef POLYNOMIALCORE_H
#define POLYNOMIALCORE_H

#include "Polynomial.h"
#include <cmath>


template <class T>
Polynomial<T>::Polynomial() {
    coefficients = new MutableArraySequence<T>();
    coefficients->append(T(0));
}

template <class T>
Polynomial<T>::Polynomial(T item) {
    coefficients = new MutableArraySequence<T>();
    coefficients->append(item);
}

template <class T>
Polynomial<T>::Polynomial(Sequence<T> *coeffs) : coefficients(coeffs) {
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

template <class T>
Polynomial<T> &Polynomial<T>::operator=(const Polynomial<T> &other) {
    if (this != &other) {
        delete coefficients;
        coefficients = other.coefficients->copy();
    }

    return *this;
}

template <class T>
Polynomial<T> &Polynomial<T>::operator=(Polynomial<T> &&other) noexcept {
    if (this != &other) {
        delete coefficients;
        coefficients = other.coefficients;
        other.coefficients = nullptr;
    }

    return *this;
}

template <class T>
Polynomial<T> Polynomial<T>::operator+(const Polynomial<T> &other) const {
    int len = coefficients->getLength();
    int other_len = other.coefficients->getLength();
    Polynomial<T> new_polynomial = Polynomial(((len > other_len) ? coefficients: other.coefficients)->copy());
    for (int i = 0; i < ((len < other_len) ? len : other_len); i++) {
        new_polynomial.coefficients->set(coefficients->get(i) + other.coefficients->get(i), i);
    }
    return new_polynomial;
}

template <class T>
Polynomial<T> Polynomial<T>::operator-(const Polynomial<T> &other) const {
    int len = coefficients->getLength();
    int other_len = other.coefficients->getLength();
    Polynomial<T> new_polynomial = Polynomial(((len > other_len) ? coefficients: other.coefficients)->copy());
    for (int i = 0; i < other_len; i++) {
        if (i < len) {
            new_polynomial.coefficients->set(coefficients->get(i) - other.coefficients->get(i), i);
        } else {
            new_polynomial.coefficients->set(-1 * other.coefficients->get(i), i);
        }
    }
    return new_polynomial;
}

template <class T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial<T> &other) const {
    Polynomial<T> new_polynomial;
    for (int i = 0; i < coefficients->getLength(); i++) {
        for (int j = 0; j < other.coefficients->getLength(); j++) {
            if (i + j < new_polynomial.coefficients->getLength()) {
                new_polynomial.coefficients->set(coefficients->get(i) * other.coefficients->get(j) + new_polynomial.coefficients->get(i + j), i + j);
            } else {
                new_polynomial.coefficients->append(coefficients->get(i) * other.coefficients->get(j));
            }
        }
    }
    return new_polynomial;
}

template <class T>
Polynomial<T> Polynomial<T>::operator*(const T &scalar) const {
    Polynomial<T> new_polynomial(coefficients->copy());
    for (int i = 0; i < coefficients->getLength(); i++) {
        new_polynomial.coefficients->set(coefficients->get(i) * scalar, i);
    }
    return new_polynomial;
}

template <class T>
int Polynomial<T>::Degree() const {
    return coefficients->getLength();
}

template <class T>
T Polynomial<T>::GetCoefficient(int index) const {
    if (index < 0 || index >= Degree()) {
        throw PolynomialException();
    }
    return coefficients->get(index);
}

template <class T>
T Polynomial<T>::Evaluate(const T &x) const {
    T evaluate = T(0);
    for (int i = Degree() - 1; i >= 0; i--) {
        evaluate = evaluate * x + GetCoefficient(i);
    }
    return evaluate;
}

template <class T>
Polynomial<T> Polynomial<T>::Compose(const Polynomial<T> &other) const {
    Polynomial<T> new_polynomial;
    for (int i = Degree() - 1; i >= 0; i--) {
        new_polynomial = new_polynomial * other + Polynomial(GetCoefficient(i));
    }
    return new_polynomial;
}

template <class T>
const Sequence<T> *Polynomial<T>::GetCoefficients() const {
    return coefficients;
}

#endif