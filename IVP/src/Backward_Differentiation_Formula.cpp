#include "Backward_Differentiation_Formula.h"
#include "VectorFunction.h"
void Backward_Differentiation_Formula::solve(
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


void Backward_Differentiation_Formula::doSolve(
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

void Backward_Differentiation_Formula::step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    const double& stepsize,
    const double& period_begin) {
        int timeindex = result.size();
        std::vector<double> next{result[timeindex-1]};
        std::vector<double> other;
        other.resize(next.size(),0.0);
        for(int i=0;i<order_;i++){
            other = other - parameters_[i+1]*result[timeindex-order_+i];
        }
        auto epsilon = std::numeric_limits<double>::epsilon();
        int count=0;
        while(true) {
            std::vector<double> delta = stepsize*parameters_[0]*(*function)(next,period_begin+timeindex*stepsize);
            if(maxNorm(next-other-delta)<epsilon){
                break;
            }
            if(count==10000) {
                std::cerr << "Too many iterations in fix point iteration" << std::endl;
                break;
            }
            // for(auto v:next) {
            //     std::cout<<v<<" ";
            // }
            // std::cout<<std::endl;
            next = other + delta;
            count++;
        }
        result.push_back(next);
}
void Backward_Differentiation_Formula::initialize() {
    result_.resize(0);
    result_.reserve(0);
    parameters_.resize(order_+1,0.0);
    switch (order_) {
        case 1:
            parameters_[0] = 1.0;
            parameters_[1] = -1.0;
            break;
        case 2:
            parameters_[0] = 2.0/3.0;
            parameters_[1] = 1.0/3.0;
            parameters_[2] = -4.0/3.0;
            break;
        case 3:
            parameters_[0] = 6.0/11.0;
            parameters_[1] = -2.0/11.0;
            parameters_[2] = 9.0/11.0;
            parameters_[3] = -18.0/11.0;
            break;
        case 4:
            parameters_[0] = 12.0/25.0;
            parameters_[1] = 3.0/25.0;
            parameters_[2] = -16.0/25.0;
            parameters_[3] = 36.0/25.0;
            parameters_[4] = -48.0/25.0;
            break;
        default:
            std::cerr << "Invalid order" << std::endl;
            break;
    }
}