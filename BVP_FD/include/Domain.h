#ifndef DOMAIN
#define DOMAIN
#include <iostream>
#include <vector>
#include "./Assistance.h"
#include "./Cycle.h"
class Domain{
private:
    int _grid_number;
    bool _if_substract_a_cycle;
public:
    BoundaryCondition _bound;
    Cycle _cycle;
    bool Prepare();
    Domain(){}
    void SetBound(const std::string& type,
    Function* value_func,
    Function* type_func = nullptr
    ){
        _bound.initialize(type,value_func,type_func);
    }
    void SetGridNumber(int grid_number)
    {
        _grid_number=grid_number;
    }
    void SetCycle(double radius,
    const std::vector<double>& center,
    const std::string& type,
    Function* value_func,
    Function* type_func = nullptr)
    {
        _cycle.SetRadius(radius);
        _cycle.SetCenter(center);
        _cycle.SetBound(type,value_func,type_func);
    }
    bool test_connection(double radius,const std::vector<double>& center)
    {
        //std::cout<<"in test connection"<<std::endl;
        double up = center[1]+radius;
        double down = center[1]-radius;
        double left = center[0]-radius;
        double right = center[0]+radius;
        if(up>=1&&left<=0)
        {
            if(!_cycle.IsInside(0,1))
            {
                return false;
            }
        }
        //std::cout<<"1"<<std::endl;
        if(left<=0&&down<=0)
        {
            //std::cout<<"try"<<std::endl;
            if(!_cycle.IsInside(0,0))
            {
                //std::cout<<"2"<<std::endl;
                return false;
            }
        }
        //std::cout<<"4"<<std::endl;
        if(down<=0&&right>=1)
        {
            if(!_cycle.IsInside(1,0))
            {
                return false;
            }
        }
        if(up>=1&&right>=1)
        {
            if(!_cycle.IsInside(1,1))
            {
                return false;
            }
        }
        if(up>=1&&down<=0)
        {
            if(left<=0)
            {
                if(right>=1)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                if(right>=1)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        if(left<=0&&right>=1)
        {
            if(up>=1)
            {
                if(down<=0)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                if(down>0)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        //std::cout<<"true"<<std::endl;
        return true;
    }
    void SetIfSubtractACycle(bool if_sub)
    {
        _if_substract_a_cycle = if_sub;
    }
    bool Isvalid()
    {
       return  _cycle.Isvalid(_grid_number);
    }
    bool InCycle(double x,double y)
    {
        if(_if_substract_a_cycle==false)
        {
            return false;
        }
        return _cycle.IsInside(x,y);
    }
    bool InBoundary(double x,double y)
    {
        if(x==0||x==1)
        {
            return true;
        }
        if(y==0||y==1)
        {
            return true;
        }
        return false;
    }
    void Initialize(bool if_sub,
    const std::string& type,
    Function* value_func,
    Function* type_func = nullptr,
    double radius=0,
    const std::vector<double>& center=std::vector<double>{},
    const std::string& ctype="",
    Function* cvalue_func=nullptr,
    Function* ctype_func = nullptr){
        //std::cout<<"Setting Domain"<<std::endl;
        SetIfSubtractACycle(if_sub);
        //std::cout<<"Setting Bound"<<std::endl;
        SetBound(type,value_func,type_func);
        //std::cout<<"111"<<std::endl;
        //std::cout<<"if_sub:"<<_if_substract_a_cycle<<std::endl;
        if(_if_substract_a_cycle)
        {
            //std::cout<<"Seting Cycle"<<std::endl;
            SetCycle(radius,center,ctype,cvalue_func,ctype_func);
        }
    }
};
#endif