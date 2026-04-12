#include "Polynomial.h"

int main() {
    Polynomial<int> p1;
    int data[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> *seq1 = new MutableArraySequence(data, 5);
    Polynomial<int> p2(seq1);
    Polynomial<int> p3 = p1 + p2;
    Polynomial<int> p4 = p1 - p2;
    Polynomial<int> p5 = p3 * p2;
    std::cout << p2.Degree() << std::endl;
    std::cout << p2.GetCoefficient(1) << std::endl;
    std::cout << p2.Evaluate(1) << std::endl;
    Polynomial<int> p6 = p2 * 3;
    p2.GetCoefficients()->print();
    p3.GetCoefficients()->print();
    p4.GetCoefficients()->print();
    p5.GetCoefficients()->print();
    p6.GetCoefficients()->print();

    int data2[] = {1, 2, 3};
    int data3[] = {1, 1};
    MutableArraySequence<int> *seq2 = new MutableArraySequence(data2, 3);
    MutableArraySequence<int> *seq3 = new MutableArraySequence(data3, 2);
    Polynomial<int> p7(seq2);
    Polynomial<int> p8(seq3);
    Polynomial<int> p9 = p7.Compose(p8);
    p9.GetCoefficients()->print();
    p5.print();
    bool b = p2 == p3;
    std::cout << b << std::endl;
}