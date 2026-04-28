#ifndef POLYNOMIALCORE_H
#define POLYNOMIALCORE_H

#include "Polynomial.h"


template <class T>
Polynomial<T>::Polynomial() {
    coefficients = new MutableArraySequence<T>();
    coefficients->append(T(0));
}

template <class T>
Polynomial<T>::Polynomial(T coeff) {
    coefficients = new MutableArraySequence<T>();
    coefficients->append(coeff);
}

template <class T>
Polynomial<T>::Polynomial(Sequence<T> *coeffs) : coefficients(coeffs) {
    if (!coefficients) {
        throw PolynomialException("Null coefficients");
    } else if (coefficients->getLength() == 0) {
        coefficients->append(T(0));
    }

    Normalize();
}

template <class T>
Polynomial<T>::Polynomial(const Polynomial<T> &other) {
    if (!other.coefficients) {
        throw PolynomialException("Null sequence");
    } else {
        coefficients = other.coefficients->copy();
    }
}

template <class T>
Polynomial<T>::Polynomial(Polynomial<T> &&other) noexcept: coefficients(other.coefficients) {
    other.coefficients = nullptr;
}

template <class T>
Polynomial<T>::~Polynomial() {
    delete coefficients;
}

template <class T>
const T Polynomial<T>::operator[](int index) const {
    if (index < 0 || index >= coefficients->getLength()) {
        throw PolynomialException("Index out of range");
    } else {
        return coefficients->get(index);
    }
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
    Polynomial<T> sum = Polynomial(((len > other_len) ? coefficients: other.coefficients)->copy());

    for (int i = 0; i < ((len < other_len) ? len : other_len); i++) {
        sum.coefficients->set(coefficients->get(i) + other.coefficients->get(i), i);
    }

    sum.Normalize();

    return sum;
}

template <class T>
Polynomial<T> Polynomial<T>::operator-(const Polynomial<T> &other) const {
    int len = coefficients->getLength();
    int other_len = other.coefficients->getLength();
    Polynomial<T> difference = Polynomial(((len > other_len) ? coefficients : other.coefficients)->copy());

    for (int i = 0; i < other_len; i++) {
        if (i < len) {
            difference.coefficients->set(coefficients->get(i) - other.coefficients->get(i), i);
        } else {
            difference.coefficients->set(-other.coefficients->get(i), i);
        }
    }

    difference.Normalize();

    return difference;
}

template <class T>
Polynomial<T> Polynomial<T>::operator*(const Polynomial<T> &other) const {
    if ((Degree() == 0  && GetCoefficient(0) == T(0)) || (other.Degree() == 0  && other.GetCoefficient(0) == T(0))) {
        return Polynomial<T>();
    }

    Polynomial<T> multiplication;

    for (int i = 0; i < coefficients->getLength(); i++) {
        for (int j = 0; j < other.coefficients->getLength(); j++) {
            if (i + j < multiplication.coefficients->getLength()) {
                multiplication.coefficients->set(coefficients->get(i) * other.coefficients->get(j) + multiplication.coefficients->get(i + j), i + j);
            } else {
                multiplication.coefficients->append(coefficients->get(i) * other.coefficients->get(j));
            }
        }
    }

    multiplication.Normalize();

    return multiplication;
}

template <class T>
Polynomial<T> Polynomial<T>::operator*(const T &scalar) const {
    if (scalar == T(0)) {
        return Polynomial<T>();
    }

    Polynomial<T> scaled(coefficients->copy());

    for (int i = 0; i < coefficients->getLength(); i++) {
        scaled.coefficients->set(coefficients->get(i) * scalar, i);
    }

    scaled.Normalize();

    return scaled;
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
Polynomial<T> Polynomial<T>::operator<<(int exponent) const {
    if (exponent < 0) {
        throw PolynomialException("Negative value");
    } else if (exponent == 0) {
        return *this;
    }

    Polynomial<T> result(*this);

    for (int i = 0; i < exponent; i++) {
        result.coefficients->prepend(T(0));
    }

    return result;
}

template <class T>
Polynomial<T> Polynomial<T>::operator>>(int exponent) const {
    if (exponent < 0) {
        throw PolynomialException("Negative value");
    } else if (exponent == 0) {
        return *this;
    }

    if (exponent >= Length()) {
        return Polynomial<T>();
    }

    Polynomial<T> result(*this);

    for (int i = 0; i < exponent; i++) {
        result.coefficients->removeFirst();
    }

    return result;
}

template <class T>
bool Polynomial<T>::operator==(const Polynomial<T> &other) const {
    int len = Length();
    int other_len = other.Length();

    if (len != other_len) return false;
    
    for (int i = 0; i < len; i++) {
        if (GetCoefficient(i) != other.GetCoefficient(i)) {
            return false;
        }
    }

    return true;
}

template <class T>
bool Polynomial<T>::operator!=(const Polynomial<T> &other) const {
    return !(*this == other);
}

template <class T>
int Polynomial<T>::Length() const {
    return coefficients->getLength();
}

template <class T>
int Polynomial<T>::Degree() const {
    return coefficients->getLength() - 1;
}

template <class T>
void Polynomial<T>::SetCoefficient(T coeff, int index) {
    if (index < 0 || index >= Length()) {
        throw PolynomialException("Index out of range");
    }

    coefficients->set(coeff, index);
}

template <class T>
T Polynomial<T>::GetCoefficient(int index) const {
    if (index < 0 || index >= Length()) {
        throw PolynomialException("Index out of range");
    }

    return coefficients->get(index);
}

template <class T>
T Polynomial<T>::Evaluate(const T &x) const {
    T evaluate = T(0);

    for (int i = Length() - 1; i >= 0; i--) {
        evaluate = evaluate * x + GetCoefficient(i);
    }

    return evaluate;
}

template <class T>
Polynomial<T> Polynomial<T>::Compose(const Polynomial<T> &other) const {
    Polynomial<T> composition;

    for (int i = Degree(); i >= 0; i--) {
        composition = composition * other + Polynomial(GetCoefficient(i));
    }
    
    return composition;
}

template <class T>
const Sequence<T> *Polynomial<T>::GetCoefficients() const {
    return coefficients;
}


#endif