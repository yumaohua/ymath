#include "../include/Solver.h"
#include "../include/FunctionExtension.h"
#include "../include/Assistance.h"
#include "../include/JsonRead.h"
int main(){
    // Solver s;
    // // s.Prepare();
    // // std::cout<<"test:"<<std::endl;
    // // std::string expression = "abs(@x*@y)";
    // // FunctionString f(expression);
    // // std::cout<<"abs(3*(-5))="<<f(3,-5)<<std::endl;
    // // std::cout<<VS(1,2)<<std::endl;
    // // std::string st{"1,2"}; 
    // // std::vector<int> v{SV(st)};
    // // std::cout<<v[0]<<","<<v[1]<<std::endl;
    // RHSFunction1 rhs_function;
    // RealFunction1 value_function;
    // // NeumannFunction neumann_function;
    // Function* rhs_func=&rhs_function;
    // Function* value_func=&value_function;
    // // Function* neumann_func=&neumann_function;
    // JsonRead("./input/input.json",rhs_func,value_func,value_func,nullptr,nullptr);
    // // s.SetError(value_func);
    // // std::cout<<s.ErrorMaxNorm()<<","<<s.ErrorTwoNorm()<<","<<s.ErrorOneNorm()<<std::endl;
    // std::ifstream ifs("./input/input.json");
    // std::cout<<ifs.is_open()<<std::endl;
    // json::value j;
    // ifs>>json::wrap(j);
    // double x=j["domain"]["cycle"]["center"][1];
    // std::cout<<x<<std::endl;
    RealFunction2 value_function2;
    RHSFunction2 rhs_function2;
    MixedFunction2 mixed_function2;
    MixedTypeFunction2 type_function2;
    Function* value2 = &value_function2;
    Function* rhs2 = &rhs_function2;
    Function* mixed2 = &mixed_function2;
    Function* type2 = &type_function2;

    CMixedFunction2 Cmixed_function2;
    CMixedTypeFunction2 Ctype_function2;
    Function* Cmixed2 = &Cmixed_function2;
    Function* Ctype2 = &Ctype_function2;
    //std::cout<<"real:"<<(*value2)(0.5,0.5)<<std::endl;
    JsonRead("./input/input.json",rhs2,value2,value2,nullptr,Cmixed2,Ctype2);
}