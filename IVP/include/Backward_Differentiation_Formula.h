#ifndef BACKWARD_DIFFERENTIATION_FORMULA_H
#define BACKWARD_DIFFERENTIATION_FORMULA_H
#include "TimeIntegrator.h"
class Backward_Differentiation_Formula: public TimeIntegrator {
public:
    explicit Backward_Differentiation_Formula(int order):TimeIntegrator(order){}
    ~Backward_Differentiation_Formula()=default;

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
#endif