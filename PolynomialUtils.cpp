#include "Exceptions.h"
#include "Polynomial.h"

#include "../LAB_2/ArraySequence.h"
#include "../LAB_2/ListSequence.h"

#include <iostream>


template <class T>
std::ostream &operator<<(std::ostream &os, const Polynomial<T> &polynomial) {
    int deg = polynomial.Length();

    if (deg == 1  &&polynomial.GetCoefficient(0) == T(0)) {
        os << "0";
        return os;
    }

    bool isFirst = true;
    for (int i = deg - 1; i >= 0; i--) {
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
    int deg = Length();

    if (deg == 1  &&GetCoefficient(0) == T(0)) {
        std::cout << "0" << std::endl;
        return;
    }

    bool isFirst = true;
    for (int i = deg - 1; i >= 0; i--) {
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


template std::ostream& operator<<(std::ostream&, const Polynomial<int>&);
template std::ostream& operator<<(std::ostream&, const Polynomial<double>&);

template void Polynomial<int>::print() const;
template void Polynomial<double>::print() const;