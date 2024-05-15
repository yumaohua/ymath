//g++ test.cpp ./src/PDDistance.cpp -o test
#include <iostream>
#include "include/PDDistance.h"

int main() {
    PD A = {{0.5,3},{1,2},{2,4},{4,5}};
    PD B = {{1,2.5},{3,4},{4.5,5}};
    Map m{A,B};
    m.print();
    std::cout<<"bottleneck:"<<m.bottleneck()<<std::endl;//输出为1
    // m.print();
}