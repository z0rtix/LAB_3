#ifndef POLYNOMIAL_h
#define POLYNOMIAL_h

#include "Exceptions.h"
#include "../LAB_2/Sequence.h"

#include <iostream>
#include <cstddef>


template <class T>
class Polynomial {
    private:
        Sequence<T> *coefficients;

        void Normalize();

    public:
        Polynomial();
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


        size_t Degree() const;
        T GetCoefficient(size_t index) const;
        T Evaluate(const T &x) const;
        Polynomial<T> Compose(const Polynomial<T> &other) const;

        const Sequence<T> *GetCoefficients() const { return coefficients; }
};

#include "PolynomialCore.h"

#endif