#ifndef CYCLE
#define CYCLE
#include <vector>
#include <cmath>
#include "./BoundaryCondition.h"
class Cycle{
public:
    double _radius=0;
    std::vector<double> _center;
    BoundaryCondition _bound;
    Cycle(){}
    void SetRadius(double radius)
    {
        _radius=radius;
    }
    void SetCenter(const std::vector<double>& center)
    {
        //std::cout<<"input_center"<<center[0]<<","<<center[1]<<std::endl;
        _center.clear();
        _center.push_back(center[0]);
        _center.push_back(center[1]);
    }
    void SetBound(const std::string& type,
    Function* value_func,
    Function* type_func = nullptr
    ){
        _bound.initialize(type,value_func,type_func);
    }
    std::string Type(double x, double y){
        return _bound.Type(x,y);
    }
    double Value(double x, double y){
        return _bound.Value(x,y);
    }
    bool IsInside(double x, double y){
        //std::cout<<"center"<<_center[0]<<","<<_center[1]<<std::endl;
        //std::cout<<"x,y:"<<x<<","<<y<<std::endl;
        //std::cout<<"center"<<_center[0]<<","<<_center[1]<<std::endl;
        double distance = (_center[0]-x)*(_center[0]-x)+(_center[1]-y)*(_center[1]-y);
        //std::cout<<"distance:"<<distance<<std::endl;
        if(distance>_radius*_radius)
        {
            return false;
        }
        return true;
    }
    bool Isvalid(int grid_number)
    {
        int indexx=floor(_center[0]*grid_number);
        int indexy=floor(_center[1]*grid_number);
        if(IsInside(indexx*1.0/grid_number,indexy*1.0/grid_number)
        &&IsInside((indexx+1)*1.0/grid_number,indexy*1.0/grid_number)
        &&IsInside(indexx*1.0/grid_number,(indexy+1)*1.0/grid_number)
        &&IsInside((indexx+1)*1.0/grid_number,(indexy+1)*1.0/grid_number))
        {
            return true;
        }
        return false;
    }
};
#endif