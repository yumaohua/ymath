#include "pos.h"
#include <iostream>

int main () {
    double result = POS(
        100,
        10000,
        0.9,
        0.5,
        2.0,
        2.0,
        0.7,
        0.2,
        10000,
        1e-6//, 100000
    );
    std::cout<<"result: "<<result<<std::endl;
}
