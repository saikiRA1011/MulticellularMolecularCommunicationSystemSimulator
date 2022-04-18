#include "Cell.hpp"
#include <iostream>

int main(void)
{
    Cell c(0, 0);

    c.printCell();

    c.addForce(2, -2);

    c.printCell();

    c.nextStep();
    c.printCell();
    c.nextStep();
    c.printCell();

    c.addForce(-1, 1);
    c.printCell();

    return 0;
}
