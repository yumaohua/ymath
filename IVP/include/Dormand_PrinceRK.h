#ifndef DORMAND_PRINCERK_H
#define DORMAND_PRINCERK_H
#include "TimeIntegrator.h"
class Dormand_PrinceRK: public TimeIntegrator {
public:
    explicit Dormand_PrinceRK(int order):TimeIntegrator(order){}
    ~Dormand_PrinceRK()=default;
    double Eabs=0.0000000001;
    double Erel=0.000001;
    double maxstepsize = 1;
    int stepnum = 0;
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
    double& stepsize,
    double& time,
    std::vector<std::vector<double>>& y1,
    std::vector<std::vector<double>>& y2);
};
std::vector<double> Dormand_PrinceRKstep(
    const std::vector<double>& last,
    double& time,
    double& stepsize, 
    const double& Eabs,
    const double& Erel,
    const VectorFunction*& func, 
    std::vector<std::vector<double>>& y1, 
    std::vector<std::vector<double>>& y2, 
    const std::vector<double>& para);
#endif