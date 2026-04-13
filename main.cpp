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
    MutableArraySequence<int>* seq4 = new MutableArraySequence<int>();
    seq4->append(1); seq4->append(2); seq4->append(3);   // 1 + 2x + 3x^2
    Polynomial<int> p10(seq4);
    Polynomial<int> p11 = p10.Compose(Polynomial<int>(2)); // P3(2) = 1 + 4 + 12 = 17
    p11.print(); // "17"
    p7.print();
    Polynomial<int> p12 = p7.Derivative();
    p12.print();
    p12.GetCoefficients()->print();
    p5.print();
    Polynomial<int> p13 = p5.Derivative();
    p13.print();
    std::cout << p12 << std::endl;
    p7.print();
    Polynomial<int> p14 = p7.Integral();
    p14.print();
    p14.GetCoefficients()->print();
}