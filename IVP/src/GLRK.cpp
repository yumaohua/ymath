#include "GLRK.h"
#include "VectorFunction.h"
#include <vector>
void GLRK::solve(
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


void GLRK::doSolve(
    const std::vector<std::vector<double>>& initial_values, 
    const VectorFunction*& function,
    const int& stepnumber,
    const double& period_begin,
    const double& period_end) {

        double stepsize = (period_end-period_begin)*1.0/stepnumber;
        result_=initial_values;
        std::vector<std::vector<double>> y;
        y.resize(order_);
        for(auto& v: y){
            v.resize(initial_values[0].size(),0.0);
        }
        for (int i = initial_values.size()-1; i < stepnumber; i++) {
            step(function,result_,stepsize,y,period_begin);
        }
}

void GLRK::step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    const double& stepsize, std::vector<std::vector<double>>& y,
    const double& period_begin) {
        int timeindex = result.size();
        std::vector<double> next{result[timeindex-1]};
        next = GLRKstep(next, period_begin+(timeindex-1)*stepsize, stepsize, function, y, parameters_);
        result.push_back(next);
}
std::vector<double> GLRKstep(const std::vector<double>& last,const double& time,const double& stepsize, const VectorFunction*& func, std::vector<std::vector<double>>& y, const std::vector<double>& para) {
    std::vector<double> maxnormvector;
    int statenumber = y.size();
    int vectordim = y[0].size();
    maxnormvector.resize(statenumber,0.0);
    int count = 0;
    auto tolerance = std::numeric_limits<double>::epsilon();
    while(true){
        std::vector<std::vector<double>> nexty;
        nexty.resize(statenumber);
        for(int i = 0;i<statenumber;i++){
            nexty[i].resize(vectordim,0.0);
            for(int j=0;j<statenumber;j++) {
                nexty[i]=nexty[i]+para[statenumber*i+j]*y[j];
            }
            nexty[i]=(*func)(last+stepsize*nexty[i],time+stepsize*para[statenumber*(statenumber+1)+i]);
            maxnormvector[i]=maxNorm(nexty[i]-y[i]);
        }
        if(maxNorm(maxnormvector)<tolerance){
            break;
        }
        y=nexty;
        count++;
        if(count>10000) {
            std::cerr << "too many fix point iteration times" << std::endl;
            break;
        }
    }
    std::vector<double> next{last};
    for(int i=0;i<statenumber;i++){
        next=next+stepsize*para[statenumber*statenumber+i]*y[i];
    }
    return next;
}
void GLRK::initialize() {
    result_.resize(0);
    result_.reserve(0);
    parameters_.resize(order_*(order_+2),0.0);
    switch (order_) {
        case 2:
            parameters_=std::vector<double>{
                1.0/4.0, 1.0/4.0 - sqrt(3.0)/6.0,
                1.0/4.0 + sqrt(3.0)/6.0, 1.0/4.0,
                1.0/2.0, 1.0/2.0,
                1.0/2.0 - sqrt(3.0)/6.0, sqrt(3.0)/6.0 + 1.0/2.0
            };
            break;
        case 3:
            parameters_=std::vector<double>{
                5.0/36.0, 2.0/9.0 - sqrt(15.0)/15.0, 5.0/36.0 - sqrt(15.0)/30.0,
                5.0/36.0 + sqrt(15.0)/24.0, 2.0/9.0, 5.0/36.0 - sqrt(15.0)/24.0,
                sqrt(15.0)/30.0 + 5.0/36.0, 2.0/9.0 + sqrt(15.0)/15.0, 5.0/36.0,
                5.0/18.0, 4.0/9.0, 5.0/18.0,
                1.0/2.0 - sqrt(15.0)/10.0, 1.0/2.0, sqrt(15.0)/10.0 + 1.0/2.0
            };
            break;
        case 4:
            parameters_=std::vector<double>{
                1.0/8.0 - sqrt(30.0)/144.0, (-195.0*sqrt(42.0) - 15.0*sqrt(42.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) - 12.0*sqrt(35.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 35.0*sqrt(30.0)*sqrt(15.0 - 2.0*sqrt(30.0)) + 180.0*sqrt(35.0) + 630.0*sqrt(15.0 - 2.0*sqrt(30.0)))/(5040.0*sqrt(15.0 - 2.0*sqrt(30.0))), (-180.0*sqrt(35.0) - 15.0*sqrt(42.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) - 12.0*sqrt(35.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 35.0*sqrt(30.0)*sqrt(15.0 - 2.0*sqrt(30.0)) + 195.0*sqrt(42.0) + 630.0*sqrt(15.0 - 2.0*sqrt(30.0)))/(5040.0*sqrt(15.0 - 2.0*sqrt(30.0))), (-72.0*sqrt(35.0) - 7.0*sqrt(30.0)*sqrt(2.0*sqrt(30.0) + 15.0) - 6.0*sqrt(42.0) + 126.0*sqrt(2.0*sqrt(30.0) + 15.0))/(1008.0*sqrt(2.0*sqrt(30.0) + 15.0)),
                (-35.0*sqrt(30.0)*sqrt(2.0*sqrt(30.0) + 15.0) - 12.0*sqrt(35.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 15.0*sqrt(42.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 180.0*sqrt(35.0) + 195.0*sqrt(42.0) + 630.0*sqrt(2.0*sqrt(30.0) + 15.0))/(5040.0*sqrt(2.0*sqrt(30.0) + 15.0)), sqrt(30.0)/144.0 + 1.0/8.0, (-72.0*sqrt(35.0) + 6.0*sqrt(42.0) + 7.0*sqrt(30.0)*sqrt(15.0 - 2.0*sqrt(30.0)) + 126.0*sqrt(15.0 - 2.0*sqrt(30.0)))/(1008.0*sqrt(15.0 - 2.0*sqrt(30.0))), (-195.0*sqrt(42.0) - 180.0*sqrt(35.0) - 35.0*sqrt(30.0)*sqrt(2.0*sqrt(30.0) + 15.0) - 12.0*sqrt(35.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 15.0*sqrt(42.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 630.0*sqrt(2.0*sqrt(30.0) + 15.0))/(5040.0*sqrt(2.0*sqrt(30.0) + 15.0)),
                (-15.0*sqrt(42.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) - 35.0*sqrt(30.0)*sqrt(2.0*sqrt(30.0) + 15.0) + 12.0*sqrt(35.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 180.0*sqrt(35.0) + 195.0*sqrt(42.0) + 630.0*sqrt(2.0*sqrt(30.0) + 15.0))/(5040.0*sqrt(2.0*sqrt(30.0) + 15.0)), (-6.0*sqrt(42.0) + 7.0*sqrt(30.0)*sqrt(15.0 - 2.0*sqrt(30.0)) + 126.0*sqrt(15.0 - 2.0*sqrt(30.0)) + 72.0*sqrt(35.0))/(1008.0*sqrt(15.0 - 2.0*sqrt(30.0))), sqrt(30.0)/144.0 + 1.0/8.0, (-195.0*sqrt(42.0) - 180.0*sqrt(35.0) - 15.0*sqrt(42.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) - 35.0*sqrt(30.0)*sqrt(2.0*sqrt(30.0) + 15.0) + 12.0*sqrt(35.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 630.0*sqrt(2.0*sqrt(30.0) + 15.0))/(5040.0*sqrt(2.0*sqrt(30.0) + 15.0)),
                (-7.0*sqrt(30.0)*sqrt(2.0*sqrt(30.0) + 15.0) + 6.0*sqrt(42.0) + 72.0*sqrt(35.0) + 126.0*sqrt(2.0*sqrt(30.0) + 15.0))/(1008.0*sqrt(2.0*sqrt(30.0) + 15.0)), (-195.0*sqrt(42.0) + 35.0*sqrt(30.0)*sqrt(15.0 - 2.0*sqrt(30.0)) + 12.0*sqrt(35.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 15.0*sqrt(42.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 180.0*sqrt(35.0) + 630.0*sqrt(15.0 - 2.0*sqrt(30.0)))/(5040.0*sqrt(15.0 - 2.0*sqrt(30.0))), (-180.0*sqrt(35.0) + 35.0*sqrt(30.0)*sqrt(15.0 - 2.0*sqrt(30.0)) + 12.0*sqrt(35.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 15.0*sqrt(42.0)*sqrt(15.0 - 2.0*sqrt(30.0))*sqrt(2.0*sqrt(30.0) + 15.0) + 195.0*sqrt(42.0) + 630.0*sqrt(15.0 - 2.0*sqrt(30.0)))/(5040.0*sqrt(15.0 - 2.0*sqrt(30.0))), 1.0/8.0 - sqrt(30.0)/144.0,
                1.0/4.0 - sqrt(30.0)/72.0, sqrt(30.0)/72.0 + 1.0/4.0, sqrt(30.0)/72.0 + 1.0/4.0, 1.0/4.0 - sqrt(30.0)/72.0,
                -sqrt(35.0)*sqrt(2.0*sqrt(30.0) + 15.0)/70.0 + 1.0/2.0, -sqrt(35.0)*sqrt(15.0 - 2.0*sqrt(30.0))/70.0 + 1.0/2.0, sqrt(35.0)*sqrt(15.0 - 2.0*sqrt(30.0))/70.0 + 1.0/2.0, sqrt(35.0)*sqrt(2.0*sqrt(30.0) + 15.0)/70.0 + 1.0/2.0
            };
            break;
        case 5:
            parameters_=std::vector<double>{
                (21.0 + 15.0*sqrt(70.0))/(800.0 + 200.0*sqrt(70.0)), (-215.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) - 285.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) - 140.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0) - 1323.0 + 210.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 945.0*sqrt(70.0))/(-50400.0 + 12600.0*sqrt(70.0)), -92.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0)/8505.0 + 4.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0)/1215.0 + 32.0/225.0, (-215.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) - 140.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0) - 210.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) - 1323.0 + 285.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 945.0*sqrt(70.0))/(-50400.0 + 12600.0*sqrt(70.0)), (-140.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 10.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 441.0 + 315.0*sqrt(70.0))/(16800.0 + 4200.0*sqrt(70.0)),
                (-140.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 1323.0 + 215.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 210.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 285.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 945.0*sqrt(70.0))/(50400.0 + 12600.0*sqrt(70.0)), (-21.0 + 15.0*sqrt(70.0))/(-800.0 + 200.0*sqrt(70.0)), -92.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0))/8505.0 - 4.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0))/1215.0 + 32.0/225.0, (-140.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) - 441.0 - 10.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 315.0*sqrt(70.0))/(-16800.0 + 4200.0*sqrt(70.0)), (-285.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) - 210.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0) - 140.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 1323.0 + 215.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 945.0*sqrt(70.0))/(50400.0 + 12600.0*sqrt(70.0)),
                (-15.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 168.0 + 120.0*sqrt(70.0) + 60.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0))/(6400.0 + 1600.0*sqrt(70.0)), (-168.0 + 15.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 60.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 120.0*sqrt(70.0))/(-6400.0 + 1600.0*sqrt(70.0)), 32.0/225.0, (-60.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) - 15.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) - 168.0 + 120.0*sqrt(70.0))/(-6400.0 + 1600.0*sqrt(70.0)), (-60.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 168.0 + 15.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 120.0*sqrt(70.0))/(6400.0 + 1600.0*sqrt(70.0)),
                (-215.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 1323.0 + 140.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 210.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 285.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 945.0*sqrt(70.0))/(50400.0 + 12600.0*sqrt(70.0)), (-441.0 + 10.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 140.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 315.0*sqrt(70.0))/(-16800.0 + 4200.0*sqrt(70.0)), 4.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0))/1215.0 + 92.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0))/8505.0 + 32.0/225.0, (-21.0 + 15.0*sqrt(70.0))/(-800.0 + 200.0*sqrt(70.0)), (-285.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) - 210.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0) - 215.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 1323.0 + 140.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 945.0*sqrt(70.0))/(50400.0 + 12600.0*sqrt(70.0)),
                (-10.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 441.0 + 315.0*sqrt(70.0) + 140.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0))/(16800.0 + 4200.0*sqrt(70.0)), (-285.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) - 1323.0 + 210.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 140.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 215.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 945.0*sqrt(70.0))/(-50400.0 + 12600.0*sqrt(70.0)), -4.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0)/1215.0 + 32.0/225.0 + 92.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0)/8505.0, (-210.0*sqrt(10.0)*sqrt(35.0 - 2.0*sqrt(70.0)) - 1323.0 + 140.0*sqrt(10.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 285.0*sqrt(7.0)*sqrt(35.0 - 2.0*sqrt(70.0)) + 215.0*sqrt(7.0)*sqrt(2.0*sqrt(70.0) + 35.0) + 945.0*sqrt(70.0))/(-50400.0 + 12600.0*sqrt(70.0)), (21.0 + 15.0*sqrt(70.0))/(800.0 + 200.0*sqrt(70.0)),
                (21.0 + 15.0*sqrt(70.0))/(400.0 + 100.0*sqrt(70.0)), (-21.0 + 15.0*sqrt(70.0))/(-400.0 + 100.0*sqrt(70.0)), 64.0/225.0, (-21.0 + 15.0*sqrt(70.0))/(-400.0 + 100.0*sqrt(70.0)), (21.0 + 15.0*sqrt(70.0))/(400.0 + 100.0*sqrt(70.0)),
                1.0/2.0 - sqrt(sqrt(70.0)/126.0 + 5.0/36.0), 1.0/2.0 - sqrt(5.0/36.0 - sqrt(70.0)/126.0), 1.0/2.0, sqrt(5.0/36.0 - sqrt(70.0)/126.0) + 1.0/2.0, sqrt(sqrt(70.0)/126.0 + 5.0/36.0) + 1.0/2.0
            };
            break;
        default:
            std::cerr << "Invalid state number" << std::endl;
            break;
    }
}