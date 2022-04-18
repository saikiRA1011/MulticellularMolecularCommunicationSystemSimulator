#include "Vec3.hpp"
#include <iostream>

using namespace std;

int main(void)
{
    Vec3 a(1, 2, 3);
    Vec3 b(4, 5, 6);
    Vec3 c(7, 8, 9);

    Vec3 d   = a + b;
    Vec3 e   = a - b;
    Vec3 g   = a.timesScalar(2);
    double h = a.dot(b);

    a.print();
    b.print();
    c.print();
    d.print();
    e.print();
    g.print();
    cout << h << endl;

    return 0;
}