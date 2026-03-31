#ifndef EULER_METHOD_H
#define EULER_METHOD_H
#include "TimeIntegrator.h"

class Euler_Method : public TimeIntegrator
{
public:
    explicit Euler_Method(int order):TimeIntegrator(order){initialize();}
    ~Euler_Method()=default;
    void solve(
        const std::vector<double>& initial_value, 
        const VectorFunction*& function,
        const int& stepnumber,
        const double& period_begin,
        const double& period_end) override;
private:
    void initialize() override;
    void step(const VectorFunction*& function,
    std::vector<std::vector<double>>& result,
    const double& stepsize,
    const double& period_begin);
};
#endif