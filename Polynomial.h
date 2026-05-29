#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "../LAB_2/Sequence.h"
#include "../LAB_2/ArraySequence.h"
#include "../LAB_2/ListSequence.h"

#include "Exceptions.h"


template <class T>
class Polynomial {
    private:
        Sequence<T> *coefficients;
        void Normalize();

    public:
        Polynomial();
        Polynomial(T coeff);
        explicit Polynomial(Sequence<T> *coeffs);
        Polynomial(const Polynomial<T> &other);
        Polynomial(Polynomial<T> &&other) noexcept;
        ~Polynomial();

        const T operator[](int index) const;

        Polynomial<T> &operator=(const Polynomial<T> &other);
        Polynomial<T> &operator=(Polynomial<T> &&other) noexcept;

        Polynomial<T> operator+(const Polynomial<T> &other) const;
        Polynomial<T> operator-(const Polynomial<T> &other) const;
        Polynomial<T> operator*(const Polynomial<T> &other) const;
        Polynomial<T> operator*(const T &scalar) const;

        Polynomial<T> &operator+=(const Polynomial<T> &other);
        Polynomial<T> &operator-=(const Polynomial<T> &other);
        Polynomial<T> &operator*=(const Polynomial<T> &other);
        Polynomial<T> &operator*=(const T &scalar);

        Polynomial<T> operator<<(int exponent) const;
        Polynomial<T> operator>>(int exponent) const;

        bool operator==(const Polynomial<T> &other) const;
        bool operator!=(const Polynomial<T> &other) const;

        int Length() const;
        int Degree() const;

        void print() const;
        void SetCoefficient(T coeff, int index);
        
        T GetCoefficient(int index) const;
        T Evaluate(const T &x) const;

        Polynomial<T> Compose(const Polynomial<T> &other) const;
        Polynomial<T> Derivative() const;
        Polynomial<T> Integral() const;
        Polynomial<T> Pow(int exponent) const;
        Polynomial<T> Shift(int exponent) const;
        Polynomial<T> ReduceFront(int exponent = -1) const;

        const Sequence<T> *GetCoefficients() const;

        struct DivisionResult {
            Polynomial<T> quotient;
            Polynomial<T> remainder;
        };
            
        DivisionResult Divide(const Polynomial<T> &other) const;
        Polynomial<T> GCD(const Polynomial<T> &other) const;
};


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
typename Polynomial<T>::DivisionResult Polynomial<T>::Divide(const Polynomial<T> &other) const {
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

    return DivisionResult{quotient, remainder};
}

template <class T>
Polynomial<T> Polynomial<T>::GCD(const Polynomial<T> &other) const {
    Polynomial<T> polynomial1(*this), polynomial2(other);
    
    while (polynomial2.Degree() != 0 || polynomial2.GetCoefficient(0) != T(0)) {
        DivisionResult divRes = polynomial1.Divide(polynomial2);
        polynomial1 = polynomial2;
        polynomial2 = divRes.remainder;
    }

    return polynomial1;
}


template <class T>
class Polynomial;

template <class T>
std::ostream& operator<<(std::ostream&, const Polynomial<T>&);


#endif