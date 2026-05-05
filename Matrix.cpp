#include "Matrix.h"


template <typename T>
void Matrix<T>::copyFrom(const Matrix<T> &other) {
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


template <typename T>
Matrix<T>::Matrix() : size(1) {
    data = new MutableArraySequence<Sequence<T>*>();
    MutableArraySequence<T> *row = new MutableArraySequence<T>();
    row->append(T(0));
    data->append(row);
}

template <typename T>
Matrix<T>::Matrix(const T &val) : size(1) {
    data = new MutableArraySequence<Sequence<T>*>();
    MutableArraySequence<T> *row = new MutableArraySequence<T>();
    row->append(val);
    data->append(row);
}

template <typename T>
Matrix<T>::Matrix(int n, ContainerType rowType, ContainerType colType) : size(n) {
    if (rowType == ARRAY)
        data = new MutableArraySequence<Sequence<T>*>();
    else
        data = new MutableListSequence<Sequence<T>*>();

    for (int i = 0; i < n; i++) {
        Sequence<T> *row = (colType == ARRAY) ? static_cast<Sequence<T>*>(new MutableArraySequence<T>()) : static_cast<Sequence<T>*>(new MutableListSequence<T>());
        for (int j = 0; j < n; j++)
            row->append(T(0));
        data->append(row);
    }
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &other) {
    copyFrom(other);
}

template <typename T>
Matrix<T>::Matrix(Matrix<T> &&other) noexcept : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
}

template <typename T>
Matrix<T>::~Matrix() {
    for (int i = 0; i < size; i++)
        delete data->get(i);
    delete data;
}


template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &other) {
    if (this != &other) {
        for (int i = 0; i < size; i++)
            delete data->get(i);
        delete data;
        copyFrom(other);
    }
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(Matrix<T> &&other) noexcept {
    if (this != &other) {
        for (int i = 0; i < size; i++)
            delete data->get(i);
        delete data;
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
    }
    return *this;
}


template <typename T>
int Matrix<T>::getSize() const { return size; }

template <typename T>
T Matrix<T>::get(int i, int j) const {
    if (i < 0 || i >= size || j < 0 || j >= size)
        throw IndexOutOfRange();
    return data->get(i)->get(j);
}

template <typename T>
void Matrix<T>::set(int i, int j, const T &val) {
    if (i < 0 || i >= size || j < 0 || j >= size)
        throw IndexOutOfRange();
    data->get(i)->set(val, j);
}


template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const {
    Matrix<T> res(size, ARRAY, ARRAY);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res.set(i, j, get(i, j) + other.get(i, j));
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &other) const {
    Matrix<T> res(size, ARRAY, ARRAY);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res.set(i, j, get(i, j) - other.get(i, j));
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &other) const {
    Matrix<T> res(size, ARRAY, ARRAY);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            T sum = T(0);
            for (int k = 0; k < size; ++k)
                sum = sum + get(i, k) * other.get(k, j);
            res.set(i, j, sum);
        }
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T &scalar) const {
    Matrix<T> res(size, ARRAY, ARRAY);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res.set(i, j, get(i, j) * scalar);
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator-() const {
    Matrix<T> res(size, ARRAY, ARRAY);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res.set(i, j, -get(i, j));
    return res;
}


template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &other) const {
    if (size != other.size) return false;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (get(i, j) != other.get(i, j)) return false;
    return true;
}

template <typename T>
bool Matrix<T>::operator!=(const Matrix<T> &other) const {
    return !(*this == other);
}


template class Matrix<int>;
template class Matrix<double>;