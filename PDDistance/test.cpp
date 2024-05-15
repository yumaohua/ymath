//g++ test.cpp ./src/PDDistance.cpp -o test
#include <iostream>
#include "include/PDDistance.h"

int main() {
    PD A = {{0.5,3},{1,2},{2,4},{4,5}};
    PD B = {{1,2.5},{3,4},{4.5,5}};
    // PD A={{2,3}};
    // PD B={{3,6}};
    Map m{A,B};
    // m.print();
    // m.print();
    std::cout<<"bottleneck:"<<m.bottleneck()<<std::endl;
    std::cout<<"2-wasserstein:"<<m.wasserstein(2)<<std::endl;
    // m.print();
}