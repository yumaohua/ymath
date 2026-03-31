#ifndef ESDIRK_H
#define ESDIRK_H
#include "TimeIntegrator.h"
class ESDIRK: public TimeIntegrator {
public:
    explicit ESDIRK(int order):TimeIntegrator(order){}
    ~ESDIRK()=default;

    void solve(
        const std::vector<double>& initial_value, 
        const VectorFunction*& function,
        const int& stepnumber,
        const double& period_begin,
        const double& period_end) override;

private:
    std::vector<double> parameters_;
    void doSolve(
        const std::vector<std::vector<double>>& initial_values, 
        const VectorFunction*& function,
        const int& stepnumber,
        const double& period_begin,
        const double& period_end);
    void initialize() override;
    void step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    const double& stepsize,
    const double& period_begin);
};
std::vector<double> ESDIRKstep(const std::vector<double>& last,const double& time,const double& stepsize, const VectorFunction*& func,
std::vector<double> & y1,std::vector<double> & y2,std::vector<double> & y3,std::vector<double> & y4,std::vector<double> & y5,std::vector<double> & y6);
#endif