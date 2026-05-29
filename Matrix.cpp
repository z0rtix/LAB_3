#include "Matrix.h"

#include <iostream>


template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
    int sz = m.getSize();
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++)
            os << m.get(i, j) << " ";
        if (i < sz - 1) os << "\n";
    }
    return os;
}

template <typename T>
void Matrix<T>::print() const {
    int sz = getSize();
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++)
            std::cout << get(i, j) << " ";
        if (i < sz - 1)
            std::cout << "\n";
    }
    std::cout << std::endl;
}


template std::ostream& operator<<(std::ostream&, const Matrix<int>&);
template std::ostream& operator<<(std::ostream&, const Matrix<double>&);

template void Matrix<int>::print() const;
template void Matrix<double>::print() const;