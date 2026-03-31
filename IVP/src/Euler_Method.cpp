#include "Euler_Method.h"

void Euler_Method::step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    const double& stepsize,
    const double& period_begin) {
        std::vector<double> last = result[result.size() - 1];
        result.push_back(last+stepsize*((*function)(last,period_begin+(result.size()-1)*stepsize)));
}

void Euler_Method::initialize() {
    result_.resize(0);
    result_.reserve(0);
}

void Euler_Method::solve(
    const std::vector<double>& initial_value, 
    const VectorFunction*& function,
    const int& stepnumber,
    const double& period_begin,
    const double& period_end) {
        double stepsize = (period_end - period_begin)*1.0/stepnumber;
        result_.push_back(initial_value);
        for (int i = 0; i < stepnumber; i++) {
            step(function, result_, stepsize,period_begin);
        }
}