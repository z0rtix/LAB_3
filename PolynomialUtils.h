#ifndef POLYNOMIALUTILS_H
#define POLYNOMIALUTILS_H

#include "Polynomial.h"


template <class T>
void Polynomial<T>::print() const {
    int deg = Degree();
    if (deg == 1 && GetCoefficient(0) == T(0)) {
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