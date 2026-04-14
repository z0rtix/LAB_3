// tests.cpp
#include "Polynomial.h"
#include "PolynomialUtils.h"
#include <cassert>
#include <chrono>
#include <iostream>
#include <complex>
#include <vector>

enum ContainerType { ARRAY, LIST };

template <typename T>
Sequence<T>* createEmptySequence(ContainerType type) {
    return (type == ARRAY) ? static_cast<Sequence<T>*>(new MutableArraySequence<T>())
                           : static_cast<Sequence<T>*>(new MutableListSequence<T>());
}

template <typename T>
Sequence<T>* createSequenceFromArray(const T* arr, int size, ContainerType type) {
    Sequence<T>* seq = createEmptySequence<T>(type);
    for (int i = 0; i < size; ++i) seq->append(arr[i]);
    return seq;
}

template <typename T>
Polynomial<T> makePoly(const T* arr, int size, ContainerType type) {
    return Polynomial<T>(createSequenceFromArray<T>(arr, size, type));
}

template <typename ExceptionType, typename Func>
bool throwsException(Func func) {
    try { func(); return false; }
    catch (const ExceptionType&) { return true; }
    catch (...) { return false; }
}

template <typename T>
long long testPolynomial(ContainerType type) {
    using namespace std::chrono;
    const char* container = (type == ARRAY) ? "Array" : "List";
    std::cout << "=== TESTS FOR Polynomial<" << typeid(T).name() << "> ON " << container << " ===\n";
    auto totalStart = high_resolution_clock::now();

    // 1. Конструкторы
    {
        Polynomial<T> p0;                             assert(p0.Degree() == 1 && p0.GetCoefficient(0) == T(0));
        Polynomial<T> pConst(T(5));                   assert(pConst.Degree() == 1 && pConst.GetCoefficient(0) == T(5));
        T a[] = {T(1),T(2),T(3)}; Polynomial<T> p = makePoly(a,3,type);
        assert(p.Degree() == 3 && p[0] == T(1) && p[1] == T(2) && p[2] == T(3));
        Polynomial<T> pCopy(p);                       assert(pCopy == p);
        Polynomial<T> pMove(std::move(pCopy));        assert(pMove == p);
        Sequence<T>* empty = createEmptySequence<T>(type);
        Polynomial<T> pEmpty(empty);                  assert(pEmpty.Degree() == 1 && pEmpty[0] == T(0));
    } std::cout << "✅ Конструкторы\n";

    // 2. Присваивание
    {
        T a1[]={T(1),T(2)}, a2[]={T(3),T(4),T(5)};
        Polynomial<T> p1 = makePoly(a1,2,type), p2 = makePoly(a2,3,type);
        p1 = p2;                                      assert(p1 == p2);
        p1 = std::move(p2);                           assert(p1.Degree() == 3);
        p1 = p1;                                      assert(p1.Degree() == 3);
    } std::cout << "✅ Присваивание\n";

    // 3. Арифметика + граничные случаи
    {
        T a1[]={T(1),T(2),T(3)}, a2[]={T(0),T(1)};
        Polynomial<T> p1 = makePoly(a1,3,type), p2 = makePoly(a2,2,type), zero;
        assert(p1 + zero == p1);
        assert(p1 - zero == p1);
        assert(p1 * zero == zero);
        assert(p1 * T(0) == zero);
        assert((p1 - p1) == zero);
        Polynomial<T> p3 = p1;
        p3 += zero;   assert(p3 == p1);
        p3 -= p1;     assert(p3 == zero);
        p3 = p1;
        p3 *= zero;   assert(p3 == zero);
        p3 = p1;
        p3 *= T(0);   assert(p3 == zero);
        // проверка основных операций
        Polynomial<T> sum = p1 + p2;
        assert(sum.Degree() == 3 && sum[0]==T(1) && sum[1]==T(3) && sum[2]==T(3));
        Polynomial<T> diff = p1 - p2;
        assert(diff[0]==T(1) && diff[1]==T(1) && diff[2]==T(3));
        Polynomial<T> prod = p1 * p2;
        assert(prod.Degree() == 4 && prod[0]==T(0) && prod[1]==T(1) && prod[2]==T(2) && prod[3]==T(3));
        Polynomial<T> scaled = p1 * T(2);
        assert(scaled[0]==T(2) && scaled[1]==T(4) && scaled[2]==T(6));
    } std::cout << "✅ Арифметика\n";

    // 4. Evaluate
    {
        T a[]={T(1),T(2),T(3)}; Polynomial<T> p = makePoly(a,3,type);
        assert(p.Evaluate(T(0)) == T(1));
        assert(p.Evaluate(T(2)) == T(1)+T(2)*T(2)+T(3)*T(2)*T(2));
        if constexpr (std::is_floating_point<T>::value || std::is_same<T,std::complex<double>>::value) {
            T big = T(1e6);
            assert(p.Evaluate(big) == T(1) + T(2)*big + T(3)*big*big);
        }
    } std::cout << "✅ Evaluate\n";

    // 5. Compose
    {
        T aP[]={T(1),T(0),T(1)}, aQ[]={T(1),T(2)};
        Polynomial<T> P = makePoly(aP,3,type), Q = makePoly(aQ,2,type);
        Polynomial<T> comp = P.Compose(Q);
        assert(comp.Degree() == 3 && comp[0]==T(2) && comp[1]==T(4) && comp[2]==T(4));
        Polynomial<T> C(T(3));
        Polynomial<T> compC = P.Compose(C);
        assert(compC.Degree() == 1 && compC[0] == T(10));
    } std::cout << "✅ Compose\n";

    // 6. Derivative
    {
        T a[]={T(3),T(2),T(1)}; Polynomial<T> p = makePoly(a,3,type);
        Polynomial<T> d = p.Derivative();
        assert(d.Degree() == 2 && d[0]==T(2) && d[1]==T(2));
        Polynomial<T> c(T(5)); Polynomial<T> dc = c.Derivative();
        assert(dc.Degree() == 1 && dc[0] == T(0));
    } std::cout << "✅ Derivative\n";

    // 7. Integral
    {
        T a[]={T(2),T(2)}; Polynomial<T> p = makePoly(a,2,type);
        Polynomial<T> i = p.Integral();
        assert(i.Degree() == 3 && i[0]==T(0) && i[1]==T(2) && i[2]==T(1));
        Polynomial<T> zero; Polynomial<T> iz = zero.Integral();
        assert(iz.Degree() == 2 && iz[0]==T(0) && iz[1]==T(0));
    } std::cout << "✅ Integral\n";

    // 8. Pow
    {
        T a[]={T(1),T(1)}; Polynomial<T> p = makePoly(a,2,type);
        Polynomial<T> p3 = p.Pow(3);
        assert(p3.Degree() == 4 && p3[0]==T(1) && p3[1]==T(3) && p3[2]==T(3) && p3[3]==T(1));
        assert(p.Pow(0).Degree() == 1 && p.Pow(0)[0] == T(1));
        Polynomial<T> zero; assert(zero.Pow(5) == zero);
        assert(throwsException<PolynomialException>([&](){ p.Pow(-1); }));
    } std::cout << "✅ Pow\n";

    // 9. Shift
    {
        T a[]={T(1),T(2),T(3)}; Polynomial<T> p = makePoly(a,3,type);
        Polynomial<T> s = p.Shift(2);
        assert(s.Degree() == 5 && s[0]==T(0) && s[1]==T(0) && s[2]==T(1) && s[3]==T(2) && s[4]==T(3));
        assert(p.Shift(0) == p);
        assert(throwsException<PolynomialException>([&](){ p.Shift(-1); }));
    } std::cout << "✅ Shift\n";

    // 10. ReduceFront
    {
        T a[]={T(1),T(2),T(3)}; Polynomial<T> p = makePoly(a,3,type);
        Polynomial<T> s = p.Shift(2);
        assert(s.ReduceFront() == p);
        assert(p.ReduceFront(5) == p);
        assert(p.ReduceFront(0) == p);
        Polynomial<T> zero; assert(zero.ReduceFront(3) == zero);
    } std::cout << "✅ ReduceFront\n";

    // 11. Divide
    {
        T aA[]={T(-1),T(0),T(1)}, aB[]={T(-1),T(1)};
        Polynomial<T> A = makePoly(aA,3,type), B = makePoly(aB,2,type);
        auto [Q,R] = A.Divide(B);
        assert(Q.Degree() == 2 && Q[0]==T(1) && Q[1]==T(1));
        assert(R.Degree() == 1 && R[0]==T(0));
    } std::cout << "✅ Divide\n";

    // 12. GCD
    {
        T aA[]={T(-1),T(0),T(1)}, aB[]={T(-1),T(1)};
        Polynomial<T> A = makePoly(aA,3,type), B = makePoly(aB,2,type);
        assert(A.GCD(B) == B);
        Polynomial<T> zero;
        assert(A.GCD(zero) == A);
        assert(zero.GCD(A) == A);
        Polynomial<T> c1(T(6)), c2(T(10));
        Polynomial<T> g = c1.GCD(c2);
        assert(g.Degree() == 1);
        if constexpr (std::is_same<T,int>::value) assert(g[0] == 2);
    } std::cout << "✅ GCD\n";

    // 13. Сравнение и доступ
    {
        T a[]={T(1),T(2),T(3)}; Polynomial<T> p1 = makePoly(a,3,type), p2 = makePoly(a,3,type);
        assert(p1 == p2);
        p1.SetCoefficient(T(10),1);
        assert(p1[1] == T(10) && p1 != p2);
        const Polynomial<T>& cp = p1;
        assert(cp[0] == T(1) && cp[2] == T(3));
        assert(throwsException<PolynomialException>([&](){ cp[-1]; }));
        assert(throwsException<PolynomialException>([&](){ cp[100]; }));
        assert(throwsException<PolynomialException>([&](){ p1.SetCoefficient(T(0),-1); }));
        assert(throwsException<PolynomialException>([&](){ p1.SetCoefficient(T(0),100); }));
    } std::cout << "✅ Доступ и сравнение\n";

    // 14. Нормализация
    {
        Sequence<T>* seq = createEmptySequence<T>(type);
        seq->append(T(1)); seq->append(T(2)); seq->append(T(0)); seq->append(T(0));
        Polynomial<T> p(seq);
        assert(p.Degree() == 2 && p[0]==T(1) && p[1]==T(2));
        T a[]={T(1),T(2)}; Polynomial<T> p2 = makePoly(a,2,type);
        assert((p2 - p2).Degree() == 1 && (p2 - p2)[0] == T(0));
    } std::cout << "✅ Нормализация\n";

    // 15. Исключения
    {
        assert(throwsException<PolynomialException>([&](){ Polynomial<T> p(nullptr); }));
        Polynomial<T> p; Polynomial<T> zero;
        assert(throwsException<PolynomialException>([&](){ p.Divide(zero); }));
    } std::cout << "✅ Исключения\n";

    auto totalEnd = high_resolution_clock::now();
    auto totalTime = duration_cast<milliseconds>(totalEnd - totalStart).count();

    // Стресс-тест
    std::cout << "⏳ Стресс-тест (10M appends)...\n";
    auto stressStart = high_resolution_clock::now();
    Sequence<T>* bigSeq = createEmptySequence<T>(type);
    for (int i = 0; i < 10'000'000; ++i) bigSeq->append(T(i % 100));
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
    struct Result { const char* name; long long arr; long long lst; };
    std::vector<Result> res;
    res.push_back({"int", testPolynomial<int>(ARRAY), testPolynomial<int>(LIST)});
    res.push_back({"double", testPolynomial<double>(ARRAY), testPolynomial<double>(LIST)});
    res.push_back({"complex<double>", testPolynomial<std::complex<double>>(ARRAY), testPolynomial<std::complex<double>>(LIST)});

    std::cout << "\n========== PERFORMANCE SUMMARY ==========\n";
    std::cout << "Type               Array (ms)   List (ms)   Faster\n";
    std::cout << "--------------------------------------------------\n";
    for (const auto& r : res) {
        std::cout << r.name << "\t\t" << r.arr << "\t\t" << r.lst << "\t";
        if (r.arr < r.lst) std::cout << "Array is " << (double)r.lst/r.arr << "x faster";
        else if (r.lst < r.arr) std::cout << "List is " << (double)r.arr/r.lst << "x faster";
        else std::cout << "Equal";
        std::cout << "\n";
    }
    std::cout << "==========================================\n";
}