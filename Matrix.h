#ifndef MATRIX_H
#define MATRIX_H

#include "../LAB_2/Sequence.h"
#include "../LAB_2/ArraySequence.h"
#include "../LAB_2/ListSequence.h"

#include <iostream>


enum ContainerType { ARRAY, LIST };


template <typename T>
class Matrix {
    private:
        Sequence<Sequence<T>*> *data;
        int size;

        void copyFrom(const Matrix<T> &other);

    public:
        Matrix();
        Matrix(const T &val);
        Matrix(int n, ContainerType rowType, ContainerType colType);
        Matrix(const Matrix<T> &other);
        Matrix(Matrix<T> &&other) noexcept;
        ~Matrix();

        Matrix<T> &operator=(const Matrix<T> &other);
        Matrix<T> &operator=(Matrix<T> &&other) noexcept;

        int getSize() const;
        T get(int i, int j) const;
        void set(int i, int j, const T &val);

        Matrix<T> operator+(const Matrix<T> &other) const;
        Matrix<T> operator-(const Matrix<T> &other) const;
        Matrix<T> operator*(const Matrix<T> &other) const;
        Matrix<T> operator*(const T &scalar) const;
        Matrix<T> operator-() const;

        bool operator==(const Matrix<T> &other) const;
        bool operator!=(const Matrix<T> &other) const;
};


template <typename T>
Matrix<T> operator*(const T &scalar, const Matrix<T> &m) {
    return m * scalar;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &m) {
    int sz = m.getSize();
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j)
            os << m.get(i, j) << " ";
        if (i < sz - 1) os << "\n";
    }
    return os;
}


#endif