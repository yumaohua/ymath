#include "Dormand_PrinceRK.h"
#include "VectorFunction.h"
#include <algorithm>
#include <vector>
void Dormand_PrinceRK::solve(
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


void Dormand_PrinceRK::doSolve(
    const std::vector<std::vector<double>>& initial_values, 
    const VectorFunction*& function,
    const int& stepnumber,
    const double& period_begin,
    const double& period_end) {

        double stepsize = (period_end-period_begin)*1.0/stepnumber;
        result_=initial_values;
        std::vector<std::vector<double>> y1;
        y1.resize(7);
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

void Dormand_PrinceRK::step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    double& stepsize,double&time, 
    std::vector<std::vector<double>>& y1,
    std::vector<std::vector<double>>& y2
    ) {
        int timeindex = result.size();
        std::vector<double> next{result[timeindex-1]};
        next = Dormand_PrinceRKstep(next, time, stepsize,Eabs,Erel,function, y1,y2, parameters_);
        if(stepsize>maxstepsize){
            stepsize = maxstepsize;
        }
        if(next.size()!=0){
            result.push_back(next);
            stepnum+=1;
        }
}
std::vector<double> Dormand_PrinceRKstep(
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
    int statenumber = 7;
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
    int q = 5;
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
void Dormand_PrinceRK::initialize() {
    result_.resize(0);
    result_.reserve(0);
    parameters_.resize(7*10,0.0);
    stepnum = 0;
    switch (order_) {
        default:
            parameters_=std::vector<double> {
                0,0,0,0,0,0,0,
                0.2,0,0,0,0,0,0,
                3.0/40.0,9.0/40.0,0,0,0,0,0,
                44.0/45.0,-56.0/15.0,32.0/9.0,0,0,0,0,
                19372.0/6561.0,-25360.0/2187.0,64448.0/6561.0,-212.0/729.0,0,0,0,
                9017.0/3168.0,-355.0/33.0,46732.0/5247.0,49.0/176.0,-5103.0/18656.0,0,0,
                35.0/384.0,0,500.0/1113.0,125.0/192.0,-2187.0/6784.0,11.0/84.0,0,
                35.0/384.0,0,500.0/1113.0,125.0/192.0,-2187.0/6784.0,11.0/84.0,0,
                5179.0/57600.0,0,7571.0/16695.0,393.0/640.0,-92097.0/339200.0,187.0/2100.0,1.0/40.0,
                0,1.0/5.0,3.0/10.0,4.0/5.0,8.0/9.0,1.0,1.0
            };
            break;
    }
}