#ifndef TIME_INTEGRATOR_H
#define TIME_INTEGRATOR_H
#include <iostream>
#include <map>
#include "VectorFunction.h"
#include <memory>
void Output(const std::vector<std::vector<double>>& vec2, const std::string& outputpath,std::ios_base::openmode mode=std::ios::out);
class TimeIntegrator {
public:
    explicit TimeIntegrator(int order) : order_(order) {}
    virtual ~TimeIntegrator() = default;

    TimeIntegrator(const TimeIntegrator&) = delete;
    TimeIntegrator& operator=(const TimeIntegrator&) = delete;

    virtual void solve(
        const std::vector<double>& initial_value, 
        const VectorFunction*& function,
        const int& stepnumber,
        const double& period_begin,
        const double& period_end){std::cerr<<"solve in TimeIntegrator"<<std::endl;}    
    virtual void output(const std::string& outputpath,const bool & ifclean=false,std::ios_base::openmode mode=std::ios::out) {
        Output(result_,outputpath,mode);
        if(ifclean){
            result_.clear();
            result_.reserve(0);
        }    
    }
    void setOrder(int order){order_=order;}
    std::vector<double> finalResult()const {return result_[result_.size()-1];}
protected:
    std::vector<std::vector<double>> result_;
    int order_;
    virtual void initialize(){std::cerr<<"initialize in TimeIntegrator"<<std::endl;}
};

class TimeIntegratorFactory{
public:
    using CreateTimeIntegratorCallBack = std::unique_ptr<TimeIntegrator> (*)(int);

private:
    using CallbackMap = std::map<std::string, CreateTimeIntegratorCallBack>;

public:
    void registerTimeIntegrator(const std::string &ID, CreateTimeIntegratorCallBack createFn){
        callbacks_[ID] = createFn;
    }

    std::unique_ptr<TimeIntegrator> createTimeIntegrator(const std::string &ID, const int &order){
        if(!callbacks_.count(ID)){
            std::cerr << "TimeIntegratorFactory:: No such TimeIntegrator called '" << ID << "'." << std::endl;
            return nullptr;
        }
        return callbacks_[ID](order);
    }

private:
    CallbackMap callbacks_;

private:
    TimeIntegratorFactory() = default;
    TimeIntegratorFactory(const TimeIntegratorFactory&) = default;
    TimeIntegratorFactory& operator = (const TimeIntegratorFactory&) = default;
    ~TimeIntegratorFactory() = default;

public:
    static TimeIntegratorFactory& getInstance(){
        static TimeIntegratorFactory singleton;
        return singleton;
    }
};

#endif