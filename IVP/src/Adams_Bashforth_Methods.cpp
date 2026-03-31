#include "Adams_Bashforth_Methods.h"
void Adams_Bashforth_Methods::solve(
    const std::vector<double>& initial_value, 
    const VectorFunction*& function,
    const int& stepnumber,
    const double& period_begin,
    const double& period_end) {
        initialize();
        std::vector<std::vector<double>> initial_values;
        initial_values.push_back(initial_value);
        double stepsize = (period_end-period_begin)*1.0/stepnumber;
        for(int i=0;i<order_-1;i++){
            initial_values.push_back(CRKstep(initial_values[i],period_begin+i*stepsize,stepsize,function));
        }
        doSolve(
        initial_values,
        function,
        stepnumber,
        period_begin,
        period_end);
}


void Adams_Bashforth_Methods::doSolve(
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

void Adams_Bashforth_Methods::step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    const double& stepsize,
    const double& period_begin) {
        int timeindex = result.size();
        std::vector<double> last{result[timeindex-1]};
        for(int i=0;i<order_;i++){
            last = last + stepsize*parameters_[i]*(*function)(result[timeindex-order_+i],period_begin+(timeindex-order_+i)*stepsize);
        }
        result.push_back(last);
}
void Adams_Bashforth_Methods::initialize() {
    result_.resize(0);
    result_.reserve(0);
    parameters_.resize(order_,0.0);
    switch (order_) {
        case 1:
            parameters_[0] = 1.0;
            break;
        case 2:
            parameters_[0] = -0.5;
            parameters_[1] = 1.5;
            break;
        case 3:
            parameters_[0] = 5.0/12.0;
            parameters_[1] = -16.0/12.0;
            parameters_[2] = 23.0/12.0;
            break;
        case 4:
            parameters_[0] = -9.0/24.0;
            parameters_[1] = 37.0/24.0;
            parameters_[2] = -59.0/24.0;
            parameters_[3] = 55.0/24.0;
            break;
        default:
            std::cerr << "Invalid order" << std::endl;
            break;
    }
}