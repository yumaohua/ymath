#ifndef GLRK_H
#define GLRK_H
#include "TimeIntegrator.h"
class GLRK: public TimeIntegrator {
public:
    explicit GLRK(int order):TimeIntegrator(order){}
    ~GLRK()=default;

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
    std::vector<std::vector<double>>& y,
    const double& period_begin);
};
std::vector<double> GLRKstep(const std::vector<double>& last,const double& time,const double& stepsize, const VectorFunction*& func, std::vector<std::vector<double>>& y, const std::vector<double>& para);
#endif