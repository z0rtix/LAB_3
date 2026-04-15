#include "Polynomial.h"
#include "PolynomialUtils.h"

#include <cassert>
#include <chrono>
#include <complex>
#include <vector>


enum ContainerType { ARRAY, LIST };

template <typename T>
Sequence<T> *createEmptySequence(ContainerType type) {
    if (type == ARRAY)
        return static_cast<Sequence<T> *>(new MutableArraySequence<T>());
    else
        return static_cast<Sequence<T> *>(new MutableListSequence<T>());
}

template <typename T>
Sequence<T> *createSequenceFromArray(const T *arr, int size, ContainerType type) {
    Sequence<T> *seq = createEmptySequence<T>(type);
    for (int i = 0; i < size; ++i)
        seq->append(arr[i]);
    return seq;
}

template <typename T>
Polynomial<T> makePoly(const T *arr, int size, ContainerType type) {
    return Polynomial<T>(createSequenceFromArray<T>(arr, size, type));
}

template <typename ExceptionType, typename Func>
bool throwsException(Func func) {
    try {
        func();
        return false;
    } catch (const ExceptionType &) {
        return true;
    } catch (...) {
        return false;
    }
}

template <typename T>
long long testPolynomial(ContainerType type) {
    using namespace std::chrono;
    const char *container = (type == ARRAY) ? "Array" : "List";
    std::cout << "=== TESTS FOR Polynomial<" << typeid(T).name() << "> ON " << container << " ===\n";
    auto totalStart = high_resolution_clock::now();

    Polynomial<T> p0;
    assert(p0.Degree() == 1);
    assert(p0.GetCoefficient(0) == T(0));

    Polynomial<T> pConst(T(5));
    assert(pConst.Degree() == 1);
    assert(pConst.GetCoefficient(0) == T(5));

    T arr1[] = {T(1), T(2), T(3)};
    Polynomial<T> p1 = makePoly(arr1, 3, type);
    assert(p1.Degree() == 3);
    assert(p1[0] == T(1));
    assert(p1[1] == T(2));
    assert(p1[2] == T(3));

    Polynomial<T> pCopy(p1);
    assert(pCopy == p1);

    Polynomial<T> pMove(std::move(pCopy));
    assert(pMove == p1);

    Sequence<T> *emptySeq = createEmptySequence<T>(type);
    Polynomial<T> pEmpty(emptySeq);
    assert(pEmpty.Degree() == 1);
    assert(pEmpty[0] == T(0));
    std::cout << "✅ Конструкторы\n";

    T arr2[] = {T(1), T(2)};
    T arr3[] = {T(3), T(4), T(5)};
    Polynomial<T> p2 = makePoly(arr2, 2, type);
    Polynomial<T> p3 = makePoly(arr3, 3, type);
    p2 = p3;
    assert(p2 == p3);
    p2 = std::move(p3);
    assert(p2.Degree() == 3);
    assert(p2.Degree() == 3);
    std::cout << "✅ Присваивание\n";

    T a1[] = {T(1), T(2), T(3)};
    T a2[] = {T(0), T(1)};
    Polynomial<T> p4 = makePoly(a1, 3, type);
    Polynomial<T> p5 = makePoly(a2, 2, type);
    Polynomial<T> zero;

    assert(p4 + zero == p4);
    assert(p4 - zero == p4);
    assert(p4 * zero == zero);
    assert(p4 * T(0) == zero);
    assert((p4 - p4) == zero);

    Polynomial<T> p6 = p4;
    p6 += zero;
    assert(p6 == p4);
    p6 -= p4;
    assert(p6 == zero);
    p6 = p4;
    p6 *= zero;
    assert(p6 == zero);
    p6 = p4;
    p6 *= T(0);
    assert(p6 == zero);

    Polynomial<T> sum = p4 + p5;
    assert(sum.Degree() == 3);
    assert(sum[0] == T(1));
    assert(sum[1] == T(3));
    assert(sum[2] == T(3));

    Polynomial<T> diff = p4 - p5;
    assert(diff[0] == T(1));
    assert(diff[1] == T(1));
    assert(diff[2] == T(3));

    Polynomial<T> prod = p4 * p5;
    assert(prod.Degree() == 4);
    assert(prod[0] == T(0));
    assert(prod[1] == T(1));
    assert(prod[2] == T(2));
    assert(prod[3] == T(3));

    Polynomial<T> scaled = p4 * T(2);
    assert(scaled[0] == T(2));
    assert(scaled[1] == T(4));
    assert(scaled[2] == T(6));
    std::cout << "✅ Арифметика\n";

    T arr4[] = {T(1), T(2), T(3)};
    Polynomial<T> p7 = makePoly(arr4, 3, type);
    assert(p7.Evaluate(T(0)) == T(1));
    assert(p7.Evaluate(T(2)) == T(1) + T(2) * T(2) + T(3) * T(2) * T(2));
    if constexpr (std::is_floating_point<T>::value || std::is_same<T, std::complex<double>>::value) {
        T big = T(1e6);
        assert(p7.Evaluate(big) == T(1) + T(2) * big + T(3) * big * big);
    }
    std::cout << "✅ Evaluate\n";

    T aP[] = {T(1), T(0), T(1)};
    T aQ[] = {T(1), T(2)};
    Polynomial<T> P = makePoly(aP, 3, type);
    Polynomial<T> Q = makePoly(aQ, 2, type);
    Polynomial<T> comp = P.Compose(Q);
    assert(comp.Degree() == 3);
    assert(comp[0] == T(2));
    assert(comp[1] == T(4));
    assert(comp[2] == T(4));

    Polynomial<T> C(T(3));
    Polynomial<T> compC = P.Compose(C);
    assert(compC.Degree() == 1);
    assert(compC[0] == T(10));
    std::cout << "✅ Compose\n";

    T arr5[] = {T(3), T(2), T(1)};
    Polynomial<T> p8 = makePoly(arr5, 3, type);
    Polynomial<T> deriv = p8.Derivative();
    assert(deriv.Degree() == 2);
    assert(deriv[0] == T(2));
    assert(deriv[1] == T(2));

    Polynomial<T> constPoly(T(5));
    Polynomial<T> derivConst = constPoly.Derivative();
    assert(derivConst.Degree() == 1);
    assert(derivConst[0] == T(0));
    std::cout << "✅ Derivative\n";

    T arr6[] = {T(2), T(2)};
    Polynomial<T> p9 = makePoly(arr6, 2, type);
    Polynomial<T> integ = p9.Integral();
    assert(integ.Degree() == 3);
    assert(integ[0] == T(0));
    assert(integ[1] == T(2));
    assert(integ[2] == T(1));

    Polynomial<T> integZero = zero.Integral();
    assert(integZero.Degree() == 2);
    assert(integZero[0] == T(0));
    assert(integZero[1] == T(0));
    std::cout << "✅ Integral\n";

    T arr7[] = {T(1), T(1)};
    Polynomial<T> p10 = makePoly(arr7, 2, type);
    Polynomial<T> p3pow = p10.Pow(3);
    assert(p3pow.Degree() == 4);
    assert(p3pow[0] == T(1));
    assert(p3pow[1] == T(3));
    assert(p3pow[2] == T(3));
    assert(p3pow[3] == T(1));

    assert(p10.Pow(0).Degree() == 1);
    assert(p10.Pow(0)[0] == T(1));

    assert(zero.Pow(5) == zero);
    assert(throwsException<PolynomialException>([&]() { p10.Pow(-1); }));
    std::cout << "✅ Pow\n";

    T arr8[] = {T(1), T(2), T(3)};
    Polynomial<T> p11 = makePoly(arr8, 3, type);
    Polynomial<T> shifted = p11.Shift(2);
    assert(shifted.Degree() == 5);
    assert(shifted[0] == T(0));
    assert(shifted[1] == T(0));
    assert(shifted[2] == T(1));
    assert(shifted[3] == T(2));
    assert(shifted[4] == T(3));

    assert(p11.Shift(0) == p11);
    assert(throwsException<PolynomialException>([&]() { p11.Shift(-1); }));
    std::cout << "✅ Shift\n";

    Polynomial<T> shifted2 = p11.Shift(2);
    assert(shifted2.ReduceFront() == p11);
    assert(p11.ReduceFront(5) == p11);
    assert(p11.ReduceFront(0) == p11);
    assert(zero.ReduceFront(3) == zero);
    std::cout << "✅ ReduceFront\n";

    T aA[] = {T(-1), T(0), T(1)};
    T aB[] = {T(-1), T(1)};
    Polynomial<T> A = makePoly(aA, 3, type);
    Polynomial<T> B = makePoly(aB, 2, type);
    auto [Qdiv, Rdiv] = A.Divide(B);
    assert(Qdiv.Degree() == 2);
    assert(Qdiv[0] == T(1));
    assert(Qdiv[1] == T(1));
    assert(Rdiv.Degree() == 1);
    assert(Rdiv[0] == T(0));
    std::cout << "✅ Divide\n";

    assert(A.GCD(B) == B);
    assert(A.GCD(zero) == A);
    assert(zero.GCD(A) == A);

    Polynomial<T> c1(T(6));
    Polynomial<T> c2(T(10));
    Polynomial<T> g = c1.GCD(c2);
    assert(g.Degree() == 1);
    if constexpr (std::is_same<T, int>::value)
        assert(g[0] == 2);
    std::cout << "✅ GCD\n";

    T arr9[] = {T(1), T(2), T(3)};
    Polynomial<T> p12 = makePoly(arr9, 3, type);
    Polynomial<T> p13 = makePoly(arr9, 3, type);
    assert(p12 == p13);
    p12.SetCoefficient(T(10), 1);
    assert(p12[1] == T(10));
    assert(p12 != p13);

    const Polynomial<T> &cp = p12;
    assert(cp[0] == T(1));
    assert(cp[2] == T(3));

    assert(throwsException<PolynomialException>([&]() { cp[-1]; }));
    assert(throwsException<PolynomialException>([&]() { cp[100]; }));
    assert(throwsException<PolynomialException>([&]() { p12.SetCoefficient(T(0), -1); }));
    assert(throwsException<PolynomialException>([&]() { p12.SetCoefficient(T(0), 100); }));
    std::cout << "✅ Доступ и сравнение\n";

    Sequence<T> *seq = createEmptySequence<T>(type);
    seq->append(T(1));
    seq->append(T(2));
    seq->append(T(0));
    seq->append(T(0));
    Polynomial<T> pNorm(seq);
    assert(pNorm.Degree() == 2);
    assert(pNorm[0] == T(1));
    assert(pNorm[1] == T(2));

    T arr10[] = {T(1), T(2)};
    Polynomial<T> p14 = makePoly(arr10, 2, type);
    assert((p14 - p14).Degree() == 1);
    assert((p14 - p14)[0] == T(0));
    std::cout << "✅ Нормализация\n";

    assert(throwsException<PolynomialException>([&]() { Polynomial<T> pNull(nullptr); }));
    assert(throwsException<PolynomialException>([&]() { zero.Divide(zero); }));
    std::cout << "✅ Исключения\n";

    auto totalEnd = high_resolution_clock::now();
    auto totalTime = duration_cast<milliseconds>(totalEnd - totalStart).count();

    std::cout << "⏳ Стресс-тест (10M appends)...\n";
    auto stressStart = high_resolution_clock::now();
    Sequence<T> *bigSeq = createEmptySequence<T>(type);
    for (int i = 0; i < 10'000'000; ++i)
        bigSeq->append(T(i % 100));
    auto stressEnd = high_resolution_clock::now();
    auto stressTime = duration_cast<milliseconds>(stressEnd - stressStart).count();
    delete bigSeq;
    std::cout << "✅ Стресс-тест: " << stressTime << " ms\n";

    long long overall = totalTime + stressTime;
    std::cout << "⏱️  Общее время: " << overall << " ms\n";
    std::cout << "==========================================\n\n";
    return overall;
}

void runAllTests() {
    struct Result {
        const char *name;
        long long arr;
        long long lst;
    };
    std::vector<Result> res;
    res.push_back({"int\t\t", testPolynomial<int>(ARRAY), testPolynomial<int>(LIST)});
    res.push_back({"double\t\t", testPolynomial<double>(ARRAY), testPolynomial<double>(LIST)});
    res.push_back({"complex<double>\t  ", testPolynomial<std::complex<double>>(ARRAY), testPolynomial<std::complex<double>>(LIST)});

    std::cout << "\n========================= PERFORMANCE SUMMARY ===========================\n";
    std::cout << "Type                  Array (ms)   List (ms)       Faster\n";
    std::cout << "-------------------------------------------------------------------------\n";
    for (const auto &r : res) {
        std::cout << r.name << "\t" << r.arr << "\t    " << r.lst;
        if (r.arr < r.lst)
            std::cout << " \tArray is " << (double)r.lst / r.arr << "x faster";
        else if (r.lst < r.arr)
            std::cout << " \tList is " << (double)r.arr / r.lst << "x faster";
        else
            std::cout << "Equal";
        std::cout << "\n";
    }
    std::cout << "==========================================\n";
}