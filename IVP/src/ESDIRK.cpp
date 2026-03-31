#include "ESDIRK.h"
#include "VectorFunction.h"
void ESDIRK::solve(
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


void ESDIRK::doSolve(
    const std::vector<std::vector<double>>& initial_values, 
    const VectorFunction*& function,
    const int& stepnumber,
    const double& period_begin,
    const double& period_end) {

        double stepsize = (period_end-period_begin)*1.0/stepnumber;
        result_=initial_values;
        for (int i = initial_values.size()-1; i < stepnumber; i++) {
            step(function,result_,stepsize,period_begin);
        }
}

void ESDIRK::step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    const double& stepsize,
    const double& period_begin) {
        int timeindex = result.size();
        std::vector<double> next{result[timeindex-1]};
        std::vector<double> y1;
        y1.resize(next.size(),0.0);
        std::vector<double> y2{y1};
        std::vector<double> y3{y1};
        std::vector<double> y4{y1};
        std::vector<double> y5{y1};
        std::vector<double> y6{y1};
        next = ESDIRKstep(next, period_begin+(timeindex-1)*stepsize, stepsize, function,y1,y2,y3,y4,y5,y6);
        result.push_back(next);
}

std::vector<double> ESDIRKstep(const std::vector<double>& last,const double& time,const double& stepsize, const VectorFunction*& func,
std::vector<double> & y1,std::vector<double> & y2,std::vector<double> & y3,std::vector<double> & y4,std::vector<double> & y5,std::vector<double> & y6) {
    std::vector<double> maxnormvector;
    maxnormvector.resize(6);
    int count = 0;
    auto tolerance = std::numeric_limits<double>::epsilon();
    while(true){
        std::vector<double> nexty1{(*func)(last,time)};
        std::vector<double> nexty2{(*func)(last+stepsize*(1.0/4.0*y1+1.0/4.0*y2),time+0.5*stepsize)};
        std::vector<double> nexty3{(*func)(last+stepsize*(8611.0/62500.0*y1-1743.0/31250.0*y2+0.25*y3),time+83.0/250.0*stepsize)};
        std::vector<double> nexty4{(*func)(last+stepsize*(5012029.0/34652500.0*y1-654441.0/2922500.0*y2+174375.0/388108.0*y3+1.0/4.0*y4),time+31.0/50.0*stepsize)};
        std::vector<double> nexty5{(*func)(last+stepsize*(15267082809.0/155376265600.0*y1-71443401.0/120774400.0*y2+730878875.0/902184768.0*y3+2285395.0/8070912.0*y4+0.25*y5),time+17.0/20.0*stepsize)};
        std::vector<double> nexty6{(*func)(last+stepsize*(82889.0/524892.0*y1+15625.0/83664.0*y3+69875.0/102672.0*y4-2260.0/8211.0*y5+0.25*y6),time+stepsize)};
        maxnormvector[0]=maxNorm(nexty1-y1);
        maxnormvector[1]=maxNorm(nexty2-y2);
        maxnormvector[2]=maxNorm(nexty3-y3);
        maxnormvector[3]=maxNorm(nexty4-y4);
        maxnormvector[4]=maxNorm(nexty5-y5);
        maxnormvector[5]=maxNorm(nexty6-y6);
        if (maxNorm(maxnormvector) < tolerance) {
            break;
        }
        y1 = nexty1;
        y2 = nexty2;
        y3 = nexty3;
        y4 = nexty4;
        y5 = nexty5;
        y6 = nexty6;
        count++;
        if (count > 10000) {
            std::cerr << "too many fix point iteration times" << std::endl;
            break;
        }
    }
    std::vector<double> next {last+stepsize*(82889.0/524892.0*y1+15625.0/83664.0*y3+69875.0/102672.0*y4-2260.0/8211.0*y5+0.25*y6)};
    return next;
}

void ESDIRK::initialize() {
    result_.resize(0);
    result_.reserve(0);
    switch (order_) {
        case 4:
            break;
        default:
            std::cerr << "Invalid order" << std::endl;
            break;
    }
}