#include "menu.h"
#include "Exceptions.h"
#include "Polynomial.h"

#include "../LAB_2/ArraySequence.h"
#include "../LAB_2/ListSequence.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <functional>
#include <chrono>


enum DataType { INT, DOUBLE };


template <typename T>
T inputNumber(const std::string &prompt) {
    T val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка ввода, попробуйте снова.\n";
    }
}

std::string inputLine(const std::string &prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

template <typename T>
Sequence<T> *createEmptySequence(ContainerType cont) {
    if (cont == ARRAY) {
        return new MutableArraySequence<T>();
    } else {
        return new MutableListSequence<T>();
    }
}

template <typename T>
Polynomial<T> createPolynomialManual(ContainerType cont) {
    Sequence<T> *seq = createEmptySequence<T>(cont);

    int deg = inputNumber<int>("Введите степень (начиная с 0): ");
    for (int i = 0; i <= deg; i++) {
        std::ostringstream oss;
        oss << "Коэффициент при x^" << i << ": ";
        T coeff = inputNumber<T>(oss.str());
        seq->append(coeff);
    }

    return Polynomial<T>(seq);
}

template <typename T>
Polynomial<T> createTestPolynomial(ContainerType cont) {
    Sequence<T> *seq = createEmptySequence<T>(cont);

    seq->append(T(1));
    seq->append(T(2));
    seq->append(T(3));

    return Polynomial<T>(seq);
}

template <typename T>
class PolynomialMenu {
    ContainerType containerType;
    std::vector<Polynomial<T>> polynomials;
    size_t currentIndex;

    const Polynomial<T> &current() const {
        return polynomials[currentIndex];
    }

    Polynomial<T> &current() {
        return polynomials[currentIndex];
    }

    void addPolynomial(const Polynomial<T> &p) {
        polynomials.push_back(p);
        currentIndex = polynomials.size() - 1;
    }

    void printCurrent() const {
        if (polynomials.empty()) {
            std::cout << "Нет многочленов.\n";
            return;
        }

        std::cout << "Текущий многочлен [" << currentIndex << "]: ";
        std::cout << current() << "  (степень " << current().Length() << ")\n";
    }

    bool ensureNotEmpty() const {
        if (polynomials.empty()) {
            std::cout << "Список многочленов пуст. Создайте новый.\n";
            return false;
        }

        return true;
    }

    template <typename Func>
    void benchmark(const std::string &name, Func f) {
        using namespace std::chrono;
        auto start = high_resolution_clock::now();
        f();
        auto end = high_resolution_clock::now();
        auto ms = duration_cast<milliseconds>(end - start).count();
        std::cout << name << " выполнена за " << ms << " мс\n";
    }

    public:
        PolynomialMenu(ContainerType cont) : containerType(cont), currentIndex(0) {
            addPolynomial(Polynomial<T>());
        }

        void run() {
            bool exit = false;
            while (!exit) {
                std::cout << "\n===== МЕНЮ МНОГОЧЛЕНОВ (тип " << typeid(T).name() << ") =====\n";
                printCurrent();
                std::cout << "Список многочленов: ";

                for (size_t i = 0; i < polynomials.size(); i++) {
                    if (i == currentIndex) std::cout << "[" << i << "] ";
                    else std::cout << i << " ";
                }

                std::cout << "\n";
                std::cout << "1. Создать новый многочлен\n";
                std::cout << "2. Переключиться на другой многочлен\n";
                std::cout << "3. Арифметика (+, -, *, скаляр)\n";
                std::cout << "4. Evaluate\n";
                std::cout << "5. Compose\n";
                std::cout << "6. Derivative\n";
                std::cout << "7. Integral\n";
                std::cout << "8. Pow\n";
                std::cout << "9. Shift\n";
                std::cout << "10. ReduceFront\n";
                std::cout << "11. Divide\n";
                std::cout << "12. GCD\n";
                std::cout << "13. Сравнение\n";
                std::cout << "14. Доступ к коэффициентам\n";
                std::cout << "0. Выход\n";
                int choice = inputNumber<int>("> ");

                switch (choice) {
                    case 1: createPolynomial(); break;
                    case 2: switchPolynomial(); break;
                    case 3: arithmetic(); break;
                    case 4: evaluate(); break;
                    case 5: compose(); break;
                    case 6: derivative(); break;
                    case 7: integral(); break;
                    case 8: power(); break;
                    case 9: shift(); break;
                    case 10: reduceFront(); break;
                    case 11: divide(); break;
                    case 12: gcd(); break;
                    case 13: compare(); break;
                    case 14: access(); break;
                    case 0: exit = true; break;
                    default: std::cout << "Неверный выбор.\n";
                }
            }
        }

        void createPolynomial() {
            std::cout << "1 - Ввести вручную\n2 - Тестовый [1,2,3]\n";
            int c = inputNumber<int>("> ");

            Polynomial<T> p;
            if (c == 1) {
                p = createPolynomialManual<T>(containerType);
            } else {
                p = createTestPolynomial<T>(containerType);
            }
            addPolynomial(p);

            std::cout << "Создан: " << current() << "\n";
        }

        void switchPolynomial() {
            if (!ensureNotEmpty()) return;
            size_t idx = inputNumber<size_t>("Введите индекс многочлена: ");

            if (idx < polynomials.size()) {
                currentIndex = idx;
                printCurrent();
            } else {
                std::cout << "Неверный индекс.\n";
            }
        }

        void arithmetic() {
            if (!ensureNotEmpty()) return;
            std::cout << "1. +\n2. -\n3. * (многочлен)\n4. * (скаляр)\n";
            int op = inputNumber<int>("> ");

            Polynomial<T> result;
            try {
                if (op == 4) {
                    T scalar = inputNumber<T>("Скаляр: ");
                    benchmark("Умножение на скаляр", [&]() {
                        result = current() * scalar;
                    });
                } else {
                    size_t idx = inputNumber<size_t>("Индекс второго многочлена: ");
                    if (idx >= polynomials.size()) {
                        std::cout << "Неверный индекс.\n";
                        return;
                    }
                    const Polynomial<T> &other = polynomials[idx];
                    if (op == 1) {
                        benchmark("Сложение", [&]() { result = current() + other; });
                    } else if (op == 2) {
                        benchmark("Вычитание", [&]() { result = current() - other; });
                    } else if (op == 3) {
                        benchmark("Умножение", [&]() { result = current() * other; });
                    } else {
                        return;
                    }
                }
                std::cout << "Результат: " << result << "\n";
                if (askReplace()) current() = result;
            } catch (const PolynomialException &e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            } catch (const std::exception &e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }

        void evaluate() {
            if (!ensureNotEmpty()) return;
            T x = inputNumber<T>("x = ");
            T val;

            benchmark("Evaluate", [&]() {
                val = current().Evaluate(x);
            });

            std::cout << "P(" << x << ") = " << val << "\n";
        }

        void compose() {
            if (!ensureNotEmpty()) return;
            size_t idx = inputNumber<size_t>("Индекс многочлена Q: ");
            if (idx >= polynomials.size()) {
                std::cout << "Неверный индекс.\n";
                return;
            }
            try {
                Polynomial<T> result;
                benchmark("Compose", [&]() { result = current().Compose(polynomials[idx]); });
                std::cout << "P(Q) = " << result << "\n";
                if (askReplace()) current() = result;
            } catch (const PolynomialException &e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }

        void derivative() {
            if (!ensureNotEmpty()) return;
            try {
                Polynomial<T> result;
                benchmark("Derivative", [&]() { result = current().Derivative(); });
                std::cout << "P' = " << result << "\n";
                if (askReplace()) current() = result;
            } catch (const PolynomialException &e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }

        void integral() {
            if (!ensureNotEmpty()) return;
            try {
                Polynomial<T> result;
                benchmark("Integral", [&]() { result = current().Integral(); });
                std::cout << "∫P dx = " << result << " + C\n";
                if (askReplace()) current() = result;
            } catch (const PolynomialException &e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }
        
        void power() {
            if (!ensureNotEmpty()) return;
            int n = inputNumber<int>("Степень n: ");
            if (n < 0) {
                std::cout << "Степень должна быть >= 0\n";
                return;
            }
            try {
                Polynomial<T> result;
                benchmark("Pow", [&]() { result = current().Pow(n); });
                std::cout << "P^" << n << " = " << result << "\n";
                if (askReplace()) current() = result;
            } catch (const PolynomialException &e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }

        void shift() {
            if (!ensureNotEmpty()) return;
            int k = inputNumber<int>("Сдвиг k: ");
            if (k < 0) {
                std::cout << "Сдвиг должен быть >= 0\n";
                return;
            }
            try {
                Polynomial<T> result;
                benchmark("Shift", [&]() { result = current().Shift(k); });
                std::cout << "P * x^" << k << " = " << result << "\n";
                if (askReplace()) current() = result;
            } catch (const PolynomialException &e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }

        void reduceFront() {
            if (!ensureNotEmpty()) return;
            int n = inputNumber<int>("Удалить ведущих нулей (0 - без ограничений): ");
            if (n == 0) n = -1;
            try {
                Polynomial<T> result;
                benchmark("ReduceFront", [&]() { result = current().ReduceFront(n); });
                std::cout << "После сокращения: " << result << "\n";
                if (askReplace()) current() = result;
            } catch (const PolynomialException &e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }

        void divide() {
            if (!ensureNotEmpty()) return;
            size_t idx = inputNumber<size_t>("Индекс делителя: ");
            if (idx >= polynomials.size()) {
                std::cout << "Неверный индекс.\n";
                return;
            }
            try {
                auto result = current().Divide(polynomials[idx]);
                std::cout << "Частное: " << result.quotient << "\n";
                std::cout << "Остаток: " << result.remainder << "\n";
                if (askReplace()) current() = result.quotient;
            } catch (const PolynomialException &e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }

        void gcd() {
            if (!ensureNotEmpty()) return;
            size_t idx = inputNumber<size_t>("Индекс второго многочлена: ");
            if (idx >= polynomials.size()) {
                std::cout << "Неверный индекс.\n";
                return;
            }
            try {
                Polynomial<T> result = current().GCD(polynomials[idx]);
                std::cout << "НОД = " << result << "\n";
                if (askReplace()) current() = result;
            } catch (const PolynomialException &e) {
                std::cout << "Ошибка: " << e.what() << "\n";
            }
        }

        void compare() {
            if (!ensureNotEmpty()) return;
            size_t idx = inputNumber<size_t>("Индекс второго многочлена: ");
            if (idx >= polynomials.size()) {
                std::cout << "Неверный индекс.\n";
                return;
            }

            bool eq = current() == polynomials[idx];
            std::cout << "Равны: " << (eq ? "да" : "нет") << "\n";
        }

        void access() {
            if (!ensureNotEmpty()) return;
            std::cout << "1. Получить коэффициент\n2. Установить коэффициент\n";

            int c = inputNumber<int>("> ");
            if (c == 1) {
                int idx = inputNumber<int>("Индекс: ");
                try {
                    std::cout << "P[" << idx << "] = " << current()[idx] << "\n";
                } catch (...) {
                    std::cout << "Ошибка: выход за границы.\n";
                }
            } else if (c == 2) {
                int idx = inputNumber<int>("Индекс: ");
                T val = inputNumber<T>("Новое значение: ");
                try {
                    current().SetCoefficient(val, idx);
                    std::cout << "Установлено. Текущий: " << current() << "\n";
                } catch (...) {
                    std::cout << "Ошибка: выход за границы.\n";
                }
            }
        }

        bool askReplace() {
            std::cout << "Заменить текущий многочлен на результат? (y/n): ";
            std::string ans;
            std::cin >> ans;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return !ans.empty() && (ans[0] == 'y' || ans[0] == 'Y');
        }
};

void runMenu() {
    std::cout << "===== ВЫБОР ТИПА ДАННЫХ =====\n";
    std::cout << "1 - int\n2 - double\n";
    int dt = inputNumber<int>("> ");

    std::cout << "===== ВЫБОР КОНТЕЙНЕРА =====\n";
    std::cout << "1 - Array\n2 - List\n";
    int ct = inputNumber<int>("> ");
    
    ContainerType cont = (ct == 1) ? ARRAY : LIST;
    if (dt == 1) {
        PolynomialMenu<int> menu(cont);
        menu.run();
    } else {
        PolynomialMenu<double> menu(cont);
        menu.run();
    }
}