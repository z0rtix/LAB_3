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
}