#ifndef FUNCTION
#define FUNCTION
#include<vector>
#include"./Assistance.h"
class Function{
    public:
    bool Prepare();
    virtual double operator() (double x,double y) = 0;
};
#endif