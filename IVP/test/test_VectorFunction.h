#pragma once
#include <iostream>
#include "VectorFunction.h"
class test_Vec : public VectorFunction{
public:
    using ScalarFunction = std::function<double(double)>;

    void addFunction(ScalarFunction func) {
        functions.push_back(func);
        dim+=1;
    }

    std::vector<double> operator()(const std::vector<double>& input,const double& time) const {
        std::vector<double> result;
        result.reserve(functions.size());

        for (const auto& func : functions) {
            result.push_back(func(input[result.size()]));
        }
        return result;
    }

private:
    std::vector<ScalarFunction> functions;
};
int test_VectorFunction(){

    std::cout<<"test_VectorFunction:"<<std::endl;

    test_Vec testvecFunc;
    VectorFunction* vecFunc=&testvecFunc;

    testvecFunc.addFunction([](double x) { return x * x; }); // f1(x) = x^2
    testvecFunc.addFunction([](double x) { return x * x * x ; }); // f2(x) = x^3
    testvecFunc.addFunction([](double x) { return 1.0 / (x + 1); }); // f3(x) = 1 / (x + 1)
    std::cout<<"function = (x^2,y^3,1/(z+1))"<<std::endl;
    std::vector<double> input = {1.0,2.0,3.0};
    std::vector<double> output = (*vecFunc)(input,0);
    std::cout<<"input: ";
    for(auto v:input) {
        std::cout << v << "  ";
    }
    std::cout<<std::endl;
    std::cout<<"output: ";
    for(auto v:output) {
        std::cout << v << "  ";
    }
    std::cout<<std::endl;
    std::cout<<"3*output: ";
    for(auto v:3*output) {
        std::cout<< v << "  ";
    }
    std::cout<<std::endl;
    std::cout<<"input+output: ";
    for(auto v:input+output) {
        std::cout<< v << "  ";
    }
    std::cout<<std::endl;
    std::cout<<"input-output: ";
    for(auto v:input-output) {
        std::cout<< v << "  ";
    }
    std::cout<<std::endl;
    std::cout<<"fix_point: ";
    for(auto v:(*vecFunc).fixPointIteration()) {
        std::cout<< v << "  ";
    }
    std::cout<<std::endl;
    std::cout<<"-------------------------------------------------"<<std::endl;

    return 0;
}