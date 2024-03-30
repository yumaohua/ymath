#include "../include/BoundaryCondition.h"

BoundaryCondition::BoundaryCondition(const std::string& type,
    Function* value_func ,
    Function* type_func
    ):_type(type),_value_func{value_func},_type_func{type_func}{
        if(_type=="mixed"&&_type_func==nullptr)
        {
            std::cerr<<"边界类型函数未定义"<<std::endl;
        }
    }

void BoundaryCondition::initialize(const std::string& type,
    Function* value_func,
    Function* type_func
    ){
        _type = type;
        _value_func = value_func;
        _type_func = type_func;
        if(_type=="mixed"&&_type_func==nullptr)
        {
            std::cerr<<"边界类型函数未定义"<<std::endl;
        }
    }


std::string BoundaryCondition::Type(double x,double y)
{
    if(_type!="mixed")
    {
        return _type;
    }
    if(round((*_type_func)(x,y))==1)
    {
        return "dirichlet";
    }
    else if(round((*_type_func)(x,y))==2)
    {
        return "neumann";
    }
    else
    {
        std::cerr<<"未定义的点"<<std::endl;
        return "none";
    }       
}

double BoundaryCondition::Value(double x, double y){
    return (*_value_func)(x,y);
}