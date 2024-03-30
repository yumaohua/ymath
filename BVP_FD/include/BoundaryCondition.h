#ifndef BOUNDARYCONDITION
#define BOUNDARYCONDITION
#include <string>
#include <cmath>
#include "./Function.h"
class BoundaryCondition{
private:
    std::string _type;
    Function* _value_func=nullptr;//返回对应的边界条件的值
    Function* _type_func=nullptr;//输入点坐标,返回数字.0:未定义.1:Dirichlet.2:Neumann
public:
    BoundaryCondition(){}
    BoundaryCondition(const std::string& type,
    Function* value_func,
    Function* type_func = nullptr
    );
    void initialize(const std::string& type,
    Function* value_func,
    Function* type_func = nullptr
    );
    std::string Type(double x,double y);
    double Value(double x,double y);
};
#endif