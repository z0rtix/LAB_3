#ifndef POLYNOMIAL_h
#define POLYNOMIAL_h

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

        bool operator==(const Polynomial<T> &other) const;
        bool operator!=(const Polynomial<T> &other) const;

        int Degree() const;

        T GetCoefficient(int index) const;
        T Evaluate(const T &x) const;

        Polynomial<T> Compose(const Polynomial<T> &other) const;
        Polynomial<T> Derivative() const;
        Polynomial<T> Integral() const;

        const Sequence<T> *GetCoefficients() const;

        void print() const;
};

#include "PolynomialCore.h"
#include "PolynomialUtils.h"
#include "PolynomialOperations.h"

#endif