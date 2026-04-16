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
        void copyFrom(const Matrix<T> &other) {
            size = other.size;
            data = new MutableArraySequence<Sequence<T>*>();
            for (int i = 0; i < size; i++) {
                const Sequence<T> *srcRow = other.data->get(i);
                MutableArraySequence<T> *newRow = new MutableArraySequence<T>();
                for (int j = 0; j < size; j++)
                    newRow->append(srcRow->get(j));
                data->append(newRow);
            }
        }

    public:
        Matrix() : size(1) {
            data = new MutableArraySequence<Sequence<T>*>();
            MutableArraySequence<T> *row = new MutableArraySequence<T>();
            row->append(T(0));
            data->append(row);
        }

        Matrix(const T &val) : size(1) {
            data = new MutableArraySequence<Sequence<T>*>();
            MutableArraySequence<T> *row = new MutableArraySequence<T>();
            row->append(val);
            data->append(row);
        }

        Matrix(int n, ContainerType rowType, ContainerType colType) : size(n) {
            if (rowType == ARRAY) {
                data = new MutableArraySequence<Sequence<T>*>();
            } else {
                data = new MutableListSequence<Sequence<T>*>();
            }
            for (int i = 0; i < n; i++) {
                Sequence<T> *row = (colType == ARRAY)
                    ? static_cast<Sequence<T>*>(new MutableArraySequence<T>())
                    : static_cast<Sequence<T>*>(new MutableListSequence<T>());
                for (int j = 0; j < n; j++)
                    row->append(T(0));
                data->append(row);
            }
        }

        Matrix(const Matrix<T> &other) {
            copyFrom(other);
        }

        ~Matrix() {
            for (int i = 0; i < size; i++) {
                delete data->get(i);
            }
            delete data;
        }

        Matrix<T> &operator=(const Matrix<T> &other) {
            if (this != &other) {
                for (int i = 0; i < size; i++) delete data->get(i);
                delete data;
                copyFrom(other);
            }
            return *this;
        }

        int getSize() const {
            return size;
        }

        T get(int i, int j) const {
            return data->get(i)->get(j);
        }

        void set(int i, int j, const T &val) {
            data->get(i)->set(val, j);
        }

        Matrix<T> operator+(const Matrix<T> &other) const {
            Matrix<T> res(size, ARRAY, ARRAY);
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    res.set(i, j, get(i, j) + other.get(i, j));
                }
            }
            return res;
        }

        bool operator==(const Matrix<T> &other) const {
            if (size != other.size) return false;
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (get(i, j) != other.get(i, j)) return false;
                }
            }
            return true;
        }

        bool operator!=(const Matrix<T> &other) const {
            return !(*this == other);
        }
};

template <typename T>
std::ostream& operator<<(std::ostream &os, const Matrix<T> &m) {
    int sz = m.getSize();
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++)
            os << m.get(i, j) << " ";
        if (i < sz - 1) os << "\n";
    }
    return os;
}


#endif