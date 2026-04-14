#ifndef POLYNOMIALUTILS_H
#define POLYNOMIALUTILS_H

#include "Polynomial.h"


template <class T>
std::ostream &operator<<(std::ostream &os, const Polynomial<T> &polynomial) {
    int deg = polynomial.Degree();
    if (deg == 1  &&polynomial.GetCoefficient(0) == T(0)) {
        os << "0";
        return os;
    }
    bool isFirst = true;
    for (int i = deg - 1; i >= 0; --i) {
        T coeff = polynomial.GetCoefficient(i);
        if (coeff == T(0)) continue;

        if (isFirst) {
            if (coeff < T(0)) os << "-";
            isFirst = false;
        } else {
            os << (coeff < T(0) ? " - " : " + ");
        }

        T absCoeff = (coeff < T(0)) ? -coeff : coeff;
        bool needCoeff = (i == 0) || (absCoeff != T(1));
        if (needCoeff) os << absCoeff;

        if (i > 0) {
            os << "x";
            if (i > 1) os << "^" << i;
        }
    }
    return os;
}

template <class T>
void Polynomial<T>::print() const {
    int deg = Degree();
    if (deg == 1  &&GetCoefficient(0) == T(0)) {
        std::cout << "0" << std::endl;
        return;
    }
    bool isFirst = true;
    for (int i = deg - 1; i >= 0; --i) {
        T coeff = GetCoefficient(i);
        if (coeff == T(0)) continue;

        if (isFirst) {
            if (coeff < T(0)) std::cout << "-";
            isFirst = false;
        } else {
            std::cout << (coeff < T(0) ? " - " : " + ");
        }

        T absCoeff = (coeff < T(0)) ? -coeff : coeff;
        bool needCoeff = (i == 0) || (absCoeff != T(1));
        if (needCoeff) std::cout << absCoeff;

        if (i > 0) {
            std::cout << "x";
            if (i > 1) std::cout << "^" << i;
        }
    }
    std::cout << std::endl;
}


#endif