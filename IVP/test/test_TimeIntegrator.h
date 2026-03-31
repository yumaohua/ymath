#pragma once
#include "TimeIntegrator.h"
#include <iostream>
class Test:public TimeIntegrator {
private:
    int test_=0;

public:
    explicit Test(int order):TimeIntegrator(order) {}
    void initialize(int vir) {
        test_=vir;
    }
    void step(int vir) {
        test_+=vir;
    }
    void solve(int vir,int time) {
        initialize(vir);
        for(int i=0;i<time;i++){
            step(vir);
        }
    }
    double result(){return test_;}

};
int test_TimeIntegrator() {
    
    std::cout<<"test_TimeIntegrator:"<<std::endl;
    std::cout<<"solve:2+2*8 = ";
    Test test(1);
    test.solve(2,8);
    std::cout<<test.result()<<std::endl;
    std::cout<<"-------------------------------------------------"<<std::endl;
    return 0;
}