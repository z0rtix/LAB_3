// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H

#include "../LAB_2/Sequence.h"
#include "../LAB_2/ArraySequence.h"
#include "../LAB_2/ListSequence.h"

// Перечисление типов контейнера (уже есть в tests.cpp, поэтому ставим guard)
#ifndef CONTAINER_TYPE_ENUM
#define CONTAINER_TYPE_ENUM
enum ContainerType { ARRAY, LIST };
#endif

// Вспомогательная функция создания пустой Sequence заданного типа
template <typename T>
Sequence<T> *createEmptySequence(ContainerType type) {
    if (type == ARRAY)
        return new MutableArraySequence<T>();
    else
        return new MutableListSequence<T>();
}

// Класс-обёртка над квадратной матрицей
template <typename T>
class Matrix {
private:
    Sequence< Sequence<T> * > *data;   // строки матрицы, каждая строка - Sequence<T>*
    int size;                          // размер (квадратная)

    void copyFrom(const Matrix<T> &other) {
        size = other.size;
        // создаём новую внешнюю последовательность того же типа, что и у other (определяем динамически)
        // для простоты используем MutableArraySequence для внешней оболочки (строки)
        data = new MutableArraySequence< Sequence<T> * >();
        for (int i = 0; i < size; ++i) {
            const Sequence<T> *srcRow = other.data->get(i);
            // создаём новую строку, копируя элементы
            MutableSequence<T> *newRow = new MutableArraySequence<T>();
            for (int j = 0; j < size; ++j) {
                newRow->append(srcRow->get(j));
            }
            data->append(newRow);
        }
    }

public:
    // Конструктор квадратной матрицы заданного размера, заполненной нулями
    Matrix(int n = 1, ContainerType rowType = ARRAY, ContainerType colType = ARRAY) : size(n) {
        // Внешняя последовательность (строки) - MutableArraySequence для простоты
        data = new MutableArraySequence< Sequence<T> * >();
        for (int i = 0; i < n; ++i) {
            MutableSequence<T> *row = (colType == ARRAY) 
                ? static_cast< MutableSequence<T> * >(new MutableArraySequence<T>())
                : static_cast< MutableSequence<T> * >(new MutableListSequence<T>());
            for (int j = 0; j < n; ++j) {
                row->append(T(0));
            }
            data->append(row);
        }
    }

    // Конструктор копирования
    Matrix(const Matrix<T> &other) {
        copyFrom(other);
    }

    // Оператор присваивания копированием
    Matrix<T> &operator=(const Matrix<T> &other) {
        if (this != &other) {
            // удаляем старые данные
            for (int i = 0; i < size; ++i) {
                delete data->get(i);
            }
            delete data;
            copyFrom(other);
        }
        return *this;
    }

    // Деструктор
    ~Matrix() {
        for (int i = 0; i < size; ++i) {
            delete data->get(i);
        }
        delete data;
    }

    // Размер
    int getSize() const { return size; }

    // Доступ к элементу (i, j)
    T get(int i, int j) const {
        return data->get(i)->get(j);
    }

    void set(int i, int j, const T &value) {
        data->get(i)->set(value, j);
    }

    // Оператор сложения
    Matrix<T> operator+(const Matrix<T> &other) const {
        Matrix<T> result(size, ARRAY, ARRAY);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                result.set(i, j, get(i, j) + other.get(i, j));
            }
        }
        return result;
    }

    // Оператор вычитания
    Matrix<T> operator-(const Matrix<T> &other) const {
        Matrix<T> result(size, ARRAY, ARRAY);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                result.set(i, j, get(i, j) - other.get(i, j));
            }
        }
        return result;
    }

    // Оператор матричного умножения
    Matrix<T> operator*(const Matrix<T> &other) const {
        Matrix<T> result(size, ARRAY, ARRAY);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                T sum = T(0);
                for (int k = 0; k < size; ++k) {
                    sum = sum + get(i, k) * other.get(k, j);
                }
                result.set(i, j, sum);
            }
        }
        return result;
    }

    // Умножение на скаляр
    Matrix<T> operator*(const T &scalar) const {
        Matrix<T> result(size, ARRAY, ARRAY);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                result.set(i, j, get(i, j) * scalar);
            }
        }
        return result;
    }

    // Оператор сравнения
    bool operator==(const Matrix<T> &other) const {
        if (size != other.size) return false;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (get(i, j) != other.get(i, j)) return false;
            }
        }
        return true;
    }

    bool operator!=(const Matrix<T> &other) const {
        return !(*this == other);
    }

    // Статические методы для создания специальных матриц
    static Matrix<T> identity(int n) {
        Matrix<T> I(n);
        for (int i = 0; i < n; ++i) {
            I.set(i, i, T(1));
        }
        return I;
    }

    static Matrix<T> zero(int n) {
        return Matrix<T>(n);
    }
};

// Умножение скаляра на матрицу (коммутативность)
template <typename T>
Matrix<T> operator*(const T &scalar, const Matrix<T> &m) {
    return m * scalar;
}

#endif // MATRIX_H