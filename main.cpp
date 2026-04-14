#include "Polynomial.h"

#include <cassert>
#include "tests.h"


// void testConstructors() {
//     Polynomial<int> p1;
//     assert(p1.Degree() == 1);
//     assert(p1.GetCoefficient(0) == 0);

//     MutableArraySequence<int>* seq = new MutableArraySequence<int>();
//     seq->append(1); seq->append(2); seq->append(3);
//     Polynomial<int> p2(seq);
//     assert(p2.Degree() == 3);
//     assert(p2.GetCoefficient(0) == 1);
//     assert(p2.GetCoefficient(1) == 2);
//     assert(p2.GetCoefficient(2) == 3);
// }

// void testArithmetic() {
//     int a[] = {1,2,3};   // 1 + 2x + 3x^2
//     int b[] = {0,1};     // x
//     MutableArraySequence<int>* seq1 = new MutableArraySequence<int>(a, 3);
//     MutableArraySequence<int>* seq2 = new MutableArraySequence<int>(b, 2);
//     Polynomial<int> p1(seq1);
//     Polynomial<int> p2(seq2);

//     Polynomial<int> sum = p1 + p2;
//     assert(sum.Degree() == 3);
//     assert(sum.GetCoefficient(0) == 1);
//     assert(sum.GetCoefficient(1) == 3);
//     assert(sum.GetCoefficient(2) == 3);

//     Polynomial<int> diff = p1 - p2;
//     assert(diff.GetCoefficient(0) == 1);
//     assert(diff.GetCoefficient(1) == 1);
//     assert(diff.GetCoefficient(2) == 3);

//     Polynomial<int> prod = p1 * p2;
//     assert(prod.Degree() == 4);
//     assert(prod.GetCoefficient(0) == 0);
//     assert(prod.GetCoefficient(1) == 1);
//     assert(prod.GetCoefficient(2) == 2);
//     assert(prod.GetCoefficient(3) == 3);
// }

// void testEvaluate() {
//     int a[] = {1,2,3};   // 1 + 2x + 3x^2
//     MutableArraySequence<int>* seq = new MutableArraySequence<int>(a, 3);
//     Polynomial<int> p(seq);
//     assert(p.Evaluate(2) == 1 + 4 + 12);   // 17
//     assert(p.Evaluate(0) == 1);
//     assert(p.Evaluate(1) == 6);
// }

// void testCompose() {
//     // P(x) = x^2 + 1
//     MutableArraySequence<int>* seqP = new MutableArraySequence<int>();
//     seqP->append(1); seqP->append(0); seqP->append(1);
//     Polynomial<int> P(seqP);
//     // Q(x) = 2x + 1
//     MutableArraySequence<int>* seqQ = new MutableArraySequence<int>();
//     seqQ->append(1); seqQ->append(2);
//     Polynomial<int> Q(seqQ);
//     // P(Q(x)) = (2x+1)^2 + 1 = 4x^2 + 4x + 2
//     Polynomial<int> comp = P.Compose(Q);
//     assert(comp.Degree() == 3);
//     assert(comp.GetCoefficient(0) == 2);
//     assert(comp.GetCoefficient(1) == 4);
//     assert(comp.GetCoefficient(2) == 4);
// }

// void testDerivative() {
//     int a[] = {3,2,1};   // 3 + 2x + x^2
//     MutableArraySequence<int>* seq = new MutableArraySequence<int>(a, 3);
//     Polynomial<int> p(seq);
//     Polynomial<int> deriv = p.Derivative();
//     assert(deriv.Degree() == 2);
//     assert(deriv.GetCoefficient(0) == 2);
//     assert(deriv.GetCoefficient(1) == 2);
// }

// void testIntegral() {
//     int a[] = {2,2};   // 2 + 2x
//     MutableArraySequence<int>* seq = new MutableArraySequence<int>(a, 2);
//     Polynomial<int> p(seq);
//     Polynomial<int> integ = p.Integral();
//     // ∫(2+2x)dx = 2x + x^2 + C, C=0
//     assert(integ.Degree() == 3);
//     assert(integ.GetCoefficient(0) == 0);
//     assert(integ.GetCoefficient(1) == 2);
//     assert(integ.GetCoefficient(2) == 1);
// }

// void testDivide() {
//     // (x^2 - 1) / (x - 1) = x + 1
//     MutableArraySequence<int>* seqA = new MutableArraySequence<int>();
//     seqA->append(-1); seqA->append(0); seqA->append(1);
//     Polynomial<int> A(seqA);
//     MutableArraySequence<int>* seqB = new MutableArraySequence<int>();
//     seqB->append(-1); seqB->append(1);
//     Polynomial<int> B(seqB);
//     auto [Q, R] = A.Divide(B);
//     assert(Q.Degree() == 2);
//     assert(Q.GetCoefficient(0) == 1);
//     assert(Q.GetCoefficient(1) == 1);
//     assert(R.Degree() == 1);
//     assert(R.GetCoefficient(0) == 0);
// }

// void testShiftReduce() {
//     int a[] = {1,2,3};
//     MutableArraySequence<int>* seq = new MutableArraySequence<int>(a, 3);
//     Polynomial<int> p(seq);
//     Polynomial<int> shifted = p.Shift(2);
//     assert(shifted.Degree() == 5);
//     assert(shifted.GetCoefficient(0) == 0);
//     assert(shifted.GetCoefficient(1) == 0);
//     assert(shifted.GetCoefficient(2) == 1);
//     assert(shifted.GetCoefficient(3) == 2);
//     assert(shifted.GetCoefficient(4) == 3);

//     Polynomial<int> reduced = shifted.ReduceFront();
//     assert(reduced == p);
// }

// void testPow() {
//     int a[] = {1,1};   // 1 + x
//     MutableArraySequence<int>* seq = new MutableArraySequence<int>(a, 2);
//     Polynomial<int> p(seq);
//     Polynomial<int> p3 = p.Pow(3);
//     assert(p3.Degree() == 4);
//     assert(p3.GetCoefficient(0) == 1);
//     assert(p3.GetCoefficient(1) == 3);
//     assert(p3.GetCoefficient(2) == 3);
//     assert(p3.GetCoefficient(3) == 1);
// }

// void runAllTests() {
//     testConstructors();
//     testArithmetic();
//     testEvaluate();
//     testCompose();
//     testDerivative();
//     testIntegral();
//     testDivide();
//     testShiftReduce();
//     testPow();
//     std::cout << "All tests passed!\n";
// }

int main() {
    runAllTests();
}