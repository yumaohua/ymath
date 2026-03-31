#include "Adams_Moulton_Methods.h"
#include "VectorFunction.h"
void Adams_Moulton_Methods::solve(
    const std::vector<double>& initial_value, 
    const VectorFunction*& function,
    const int& stepnumber,
    const double& period_begin,
    const double& period_end) {
        initialize();
        std::vector<std::vector<double>> initial_values;
        initial_values.push_back(initial_value);
        double stepsize = (period_end-period_begin)*1.0/stepnumber;
        for(int i=0;i<order_-2;i++){
            initial_values.push_back(CRKstep(initial_values[i],period_begin+i*stepsize,stepsize,function));
        }
        doSolve(
        initial_values,
        function,
        stepnumber,
        period_begin,
        period_end);
}


void Adams_Moulton_Methods::doSolve(
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

void Adams_Moulton_Methods::step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    const double& stepsize,
    const double& period_begin) {
        int timeindex = result.size();
        std::vector<double> last{result[timeindex-1]};
        std::vector<double> next{last};
        for(int i=0;i<order_-1;i++){
            last = last + stepsize*parameters_[i]*(*function)(result[timeindex-order_+1+i],period_begin+(timeindex-order_+1+i)*stepsize);
        }
        auto epsilon = std::numeric_limits<double>::epsilon();
        int count=0;
        while(true) {
            std::vector<double> delta = stepsize*parameters_[order_-1]*(*function)(next,period_begin+timeindex*stepsize);
            if(maxNorm(next-last-delta)<epsilon){
                break;
            }
            if(count==10000) {
                std::cerr << "Too many iterations in fix point iteration" << std::endl;
                break;
            }
            next = last + delta;
            count++;
        }
        result.push_back(next);
}
void Adams_Moulton_Methods::initialize() {
    result_.resize(0);
    result_.reserve(0);
    parameters_.resize(order_,0.0);
    switch (order_) {
        case 2:
            parameters_[0] = 0.5;
            parameters_[1] = 0.5;
            break;
        case 3:
            parameters_[0] = -1.0/12.0;
            parameters_[1] = 8.0/12.0;
            parameters_[2] = 5.0/12.0;
            break;
        case 4:
            parameters_[0] = 1.0/24.0;
            parameters_[1] = -5.0/24.0;
            parameters_[2] = 19.0/24.0;
            parameters_[3] = 9.0/24.0;
            break;
        case 5:
            parameters_[0] = -19.0/720.0;
            parameters_[1] = 106.0/720.0;
            parameters_[2] = -264.0/720.0;
            parameters_[3] = 646.0/720.0;
            parameters_[4] = 251.0/720.0;
            break;
        default:
            std::cerr << "Invalid order" << std::endl;
            break;
    }
}