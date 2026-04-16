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
    if (!coefficients) {
        throw PolynomialException();
    } else if (coefficients->getLength() == 0) {
        coefficients->append(T(0));
    }
    Normalize();
}

template <class T>
Polynomial<T>::Polynomial(const Polynomial<T> &other) {
    if (!other.coefficients) throw PolynomialException();
    coefficients = other.coefficients->copy();
}

template <class T>
Polynomial<T>::Polynomial(Polynomial<T> &&other): coefficients(other.coefficients) {
    other.coefficients = nullptr;
}

template <class T>
Polynomial<T>::~Polynomial() {
    delete coefficients;
}

template <class T>
const T Polynomial<T>::operator[](int index) const {
    if (index < 0 || index >= coefficients->getLength()) {
        throw PolynomialException();
    }
    return coefficients->get(index);
}

template <class T>
void Polynomial<T>::Normalize() {
    while (coefficients->getLength() > 1  &&coefficients->getLast() == T(0)) {
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
    new_polynomial.Normalize();
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
            new_polynomial.coefficients->set(T(-1) * other.coefficients->get(i), i);
        }
    }
    new_polynomial.Normalize();
    return new_polynomial;
}

template <class T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial<T> &other) const {
    if ((Degree() == 1  && GetCoefficient(0) == T(0)) ||
        (other.Degree() == 1  && other.GetCoefficient(0) == T(0))) {
        return Polynomial<T>();
    }
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
    new_polynomial.Normalize();
    return new_polynomial;
}

template <class T>
Polynomial<T> Polynomial<T>::operator*(const T &scalar) const {
    if (scalar == T(0)) {
        return Polynomial<T>();
    }
    Polynomial<T> new_polynomial(coefficients->copy());
    for (int i = 0; i < coefficients->getLength(); i++) {
        new_polynomial.coefficients->set(coefficients->get(i) * scalar, i);
    }
    new_polynomial.Normalize();
    return new_polynomial;
}

template <class T>
Polynomial<T> &Polynomial<T>::operator+=(const Polynomial<T> &other) {
    *this = *this + other;
    return *this;
}

template <class T>
Polynomial<T> &Polynomial<T>::operator-=(const Polynomial<T> &other) {
    *this = *this - other;
    return *this;
}

template <class T>
Polynomial<T> &Polynomial<T>::operator*=(const Polynomial<T> &other) {
    *this = *this * other;
    return *this;
}

template <class T>
Polynomial<T> &Polynomial<T>::operator*=(const T &scalar) {
    *this = *this * scalar;
    return *this;
}

template <class T>
Polynomial<T> Polynomial<T>::operator<<(int k) const {
    if (k < 0) {
        throw PolynomialException();
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
Polynomial<T> Polynomial<T>::operator>>(int k) const {
    if (k < 0) {
        throw PolynomialException();
    } else if (k == 0) {
        return *this;
    }
    if (k >= Degree()) {
        return Polynomial<T>();
    }
    Polynomial<T> result(*this);
    for (int i = 0; i < k; ++i) {
        result.coefficients->removeFirst();
    }
    return result;
}

template <class T>
bool Polynomial<T>::operator==(const Polynomial<T> &other) const {
    int len1 = Degree();
    int len2 = other.Degree();
    if (len1 != len2) return false;
    
    for (int i = 0; i < len1; ++i) {
        if (GetCoefficient(i) != other.GetCoefficient(i))
            return false;
    }
    return true;
}

template <class T>
bool Polynomial<T>::operator!=(const Polynomial<T> &other) const {
    return !(*this == other);
}

template <class T>
int Polynomial<T>::Degree() const {
    return coefficients->getLength();
}

template <class T>
void Polynomial<T>::SetCoefficient(T item, int index) {
    if (index < 0 || index >= Degree()) {
        throw PolynomialException();
    }
    coefficients->set(item, index);
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