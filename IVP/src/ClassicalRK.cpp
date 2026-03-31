#include "ClassicalRK.h"
#include "VectorFunction.h"
void ClassicalRK::solve(
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


void ClassicalRK::doSolve(
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

void ClassicalRK::step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    const double& stepsize,
    const double& period_begin) {
        int timeindex = result.size();
        std::vector<double> next{result[timeindex-1]};
        next = CRKstep(next, period_begin+(timeindex-1)*stepsize, stepsize, function);
        result.push_back(next);
}
void ClassicalRK::initialize() {
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