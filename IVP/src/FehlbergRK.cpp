#include "FehlbergRK.h"
#include "VectorFunction.h"
#include <algorithm>
#include <vector>
void FehlbergRK::solve(
    const std::vector<double>& initial_value, 
    const VectorFunction*& function,
    const int& stepnumber,
    const double& period_begin,
    const double& period_end) {
        initialize();
        std::vector<std::vector<double>> initial_values;
        initial_values.push_back(initial_value);
        double stepsize = (period_end-period_begin)*1.0/stepnumber;
        doSolve(
        initial_values,
        function,
        stepnumber,
        period_begin,
        period_end);
}


void FehlbergRK::doSolve(
    const std::vector<std::vector<double>>& initial_values, 
    const VectorFunction*& function,
    const int& stepnumber,
    const double& period_begin,
    const double& period_end) {

        double stepsize = (period_end-period_begin)*1.0/stepnumber;
        result_=initial_values;
        std::vector<std::vector<double>> y1;
        y1.resize(6);
        for(auto& v: y1){
            v.resize(initial_values[0].size(),0.0);
        }
        std::vector<std::vector<double>> y2{y1};
        double time = period_begin;
        while(true){
            if(time>period_end){
                break;
            }
            step(function,result_,stepsize,time,y1,y2);
        }
}

void FehlbergRK::step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    double& stepsize,double&time, 
    std::vector<std::vector<double>>& y1,
    std::vector<std::vector<double>>& y2
    ) {
        int timeindex = result.size();
        std::vector<double> next{result[timeindex-1]};
        next = FehlbergRKstep(next, time, stepsize,Eabs,Erel,function, y1,y2, parameters_);
        // std::cout<<stepsize<<std::endl;
        if(stepsize>maxstepsize){
            stepsize = maxstepsize;
        }
        if(next.size()!=0){
            result.push_back(next);
            stepnum+=1;
        }
}
std::vector<double> FehlbergRKstep(
    const std::vector<double>& last,
    double& time,
    double& stepsize, 
    const double& Eabs,
    const double& Erel,
    const VectorFunction*& func, 
    std::vector<std::vector<double>>& y1, 
    std::vector<std::vector<double>>& y2, 
    const std::vector<double>& para) {
    std::vector<double> maxnormvector;
    int statenumber = 6;
    int vectordim = last.size();
    maxnormvector.resize(statenumber,0.0);
    int count = 0;
    auto tolerance = std::numeric_limits<double>::epsilon();
    while(true){
        std::vector<std::vector<double>> nexty;
        nexty.resize(statenumber);
        for(int i = 0;i<statenumber;i++){
            nexty[i].resize(vectordim,0.0);
            for(int j=0;j<statenumber;j++) {
                nexty[i]=nexty[i]+para[statenumber*i+j]*y1[j];
            }
            nexty[i]=(*func)(last+stepsize*nexty[i],time+stepsize*para[statenumber*(statenumber+2)+i]);
            maxnormvector[i]=maxNorm(nexty[i]-y1[i]);
        }
        if(maxNorm(maxnormvector)<tolerance){
            break;
        }
        y1=nexty;
        count++;
        if(count>10000) {
            std::cerr << "too many fix point iteration times" << std::endl;
            break;
        }
    }
    while(true){
        std::vector<std::vector<double>> nexty;
        nexty.resize(statenumber);
        for(int i = 0;i<statenumber;i++){
            nexty[i].resize(vectordim,0.0);
            for(int j=0;j<statenumber;j++) {
                nexty[i]=nexty[i]+para[statenumber*i+j]*y2[j];
            }
            nexty[i]=(*func)(last+stepsize*nexty[i],time+stepsize*para[statenumber*(statenumber+2)+i]);
            maxnormvector[i]=maxNorm(nexty[i]-y2[i]);
        }
        if(maxNorm(maxnormvector)<tolerance){
            break;
        }
        y2=nexty;
        count++;
        if(count>10000) {
            std::cerr << "too many fix point iteration times" << std::endl;
            break;
        }
    }
    std::vector<double> next1{last};
    for(int i=0;i<statenumber;i++){
        next1=next1+stepsize*para[statenumber*statenumber+i]*y1[i];
    }
    std::vector<double> next2{last};
    for(int i=0;i<statenumber;i++){
        next2=next2+stepsize*para[statenumber*(statenumber+1)+i]*y2[i];
    }
    std::vector<double> e{last};
    for(int i=0; i < e.size(); i++){
        e[i]=Eabs+std::abs(last[i])*Erel;
    }
    double Eind=0;
    for(int i = 0;i<vectordim;i++){
        Eind=Eind+(next1[i]-next2[i])*(next1[i]-next2[i])/(e[i]*e[i]);
    }
    Eind = std::sqrt(Eind/(1.0*vectordim));
    int q = 4;
    double rhomax = 5;
    double rhomin = 0.2;
    double rho = 0.9;
    double newstepsize = stepsize*std::min(rhomax,std::max(rhomin,rho*std::pow(Eind,-1.0/(q+1.0))));
    if(Eind <= 1){
        time+=stepsize;
        stepsize=newstepsize;
        return next1;
    }
    else{
        stepsize = newstepsize;
        return {};
    }
}
void FehlbergRK::initialize() {
    result_.resize(0);
    result_.reserve(0);
    parameters_.resize(6*9,0.0);
    stepnum = 0;
    switch (order_) {
        default:
            parameters_=std::vector<double> {
                0,0,0,0,0,0,
                0.25,0,0,0,0,0,
                3.0/32.0,9.0/32.0,0,0,0,0,
                1932.0/2197.0,-7200.0/2197.0,7296.0/2197.0,0,0,0,
                439.0/216.0,-8,3680.0/513.0,-845.0/4104.0,0,0,
                -8.0/27.0,2.0,-3544.0/2565.0,1859.0/4104.0,-11.0/40.0,0,
                25.0/216.0,0,1408.0/2565.0,2197.0/4104.0,-1.0/5.0,0,
                16.0/135.0,0,6656.0/12825.0,28561.0/56430.0,-9.0/50.0,2.0/55.0,
                0,1.0/4.0,3.0/8.0,12.0/13.0,1,1.0/2.0
            };
            break;
    }
}