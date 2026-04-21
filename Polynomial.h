#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "Exceptions.h"
#include "../LAB_2/ListSequence.h"
#include "../LAB_2/ArraySequence.h"

#include <iostream>


template <class T>
class Polynomial {
    private:
        Sequence<T> *coefficients;
        void Normalize();

    public:
        Polynomial();
        Polynomial(T item);
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

        Polynomial<T> operator<<(int n) const;
        Polynomial<T> operator>>(int n) const;

        bool operator==(const Polynomial<T> &other) const;
        bool operator!=(const Polynomial<T> &other) const;

        int Degree() const;
        void print() const;
        void SetCoefficient(T item, int index);
        
        T GetCoefficient(int index) const;
        T Evaluate(const T &x) const;

        Polynomial<T> Compose(const Polynomial<T> &other) const;
        Polynomial<T> Derivative() const;
        Polynomial<T> Integral() const;
        Polynomial<T> Pow(int n) const;
        Polynomial<T> Shift(int k) const;
        Polynomial<T> ReduceFront(int n = -1) const;

        const Sequence<T> *GetCoefficients() const;
        
        std::pair<Polynomial<T>, Polynomial<T>> Divide(const Polynomial<T> &other) const;
        Polynomial<T> GCD(const Polynomial<T> &other) const;
};


#include "PolynomialCore.h"
#include "PolynomialUtils.h"
#include "PolynomialOperations.h"


#endif