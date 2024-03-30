#ifndef FUNCTION_EXTENSION
#define FUNCTION_EXTENSION
#include "./Function.h"
#include"./ExpressionTree.h"
class FunctionString:public Function{
private:
    ExpressionTree _expression;
public:
    FunctionString(){}
    FunctionString(const std::string & expression)
    {
        std::vector<std::string> valuenames;
        valuenames.push_back("@x");
        valuenames.push_back("@y");
        _expression.initialize(expression,valuenames);
    }
    double operator()(double x,double y) override{
        std::vector<double> values;
        values.push_back(x);
        values.push_back(y);
        _expression.passvariablevalues(values);
        return _expression.calculate();
    }
};
class RHSFunction1:public Function{
public:
    double operator()(double x,double y)override{
        return (sin(x)-cos(x)*cos(x)-1)*exp(y+sin(x));
    }
};
class RealFunction1:public Function{
public:
    double operator()(double x,double y)override{
        return exp(y+sin(x));
    } 
};
class MixedFunction1: public Function{
public:
    double operator()(double x,double y)override{
        if(y==1)
        {
            return exp(1+sin(x));
        }
        else if(y==0)
        {
            return exp(sin(x));
        }
        else if(x==0)
        {
            return -exp(y);
        }
        else if(x==1)
        {
//            return cos(1)*exp(y+sin(1));
            return exp(y+sin(1));

        }
    } 
};
class MixedTypeFunction1:public Function{
    double operator()(double x,double y)override{
        if(y==1)
        {
            return 2;
        }
        else if(y==0)
        {
            return 1;
        }
        else if(x==0)
        {
            return 2;
        }
        else if(x==1)
        {
            return 1;
        }
    }
};//参见BoundaryCondition类,TypeFunction返回1表示该点是Dirichlet条件,为2表示该点是Neumann条件.
class CMixedFunction1: public Function{
public:
    double operator()(double x,double y)override{
        return cos(x)*exp(y+sin(x))*(-x)/sqrt(x*x+y*y)+exp(y+sin(x))*(-y)/sqrt(x*x+y*y);
    } 
};
class CMixedTypeFunction1:public Function{
    double operator()(double x,double y)override{
        return 2;
    }
};
class RHSFunction2:public Function{
public:
    double operator()(double x,double y)override{
        return -4;
    }
};
class RealFunction2:public Function{
public:
    double operator()(double x,double y)override{
        return x*x+y*y;
    } 
};
class MixedFunction2: public Function{
public:
    double operator()(double x,double y)override{
        if(y==1)
        {
            return 2;
        }
        else if(y==0)
        {
            return x*x;
        }
        else if(x==0)
        {
            return 0;
        }
        else if(x==1)
        {
            return 2;

        }
    } 
};
class MixedTypeFunction2:public Function{
    double operator()(double x,double y)override{
        if(y==1)
        {
            return 2;
        }
        else if(y==0)
        {
            return 1;
        }
        else if(x==0)
        {
            return 2;
        }
        else if(x==1)
        {
            return 2;
        }
    }
};//参见BoundaryCondition类,TypeFunction返回1表示该点是Dirichlet条件,为2表示该点是Neumann条件.
class CMixedFunction2: public Function{
public:
    double operator()(double x,double y)override{
        return -(2*x*x/sqrt(x*x+y*y)+2*y*y/sqrt(x*x+y*y));
    } 
};
class CMixedTypeFunction2:public Function{
    double operator()(double x,double y)override{
        return 2;
    }
};//参见BoundaryCondition类,TypeFunction返回1表示该点是Dirichlet条件,为2表示该点是Neumann条件.

#endif