#ifndef VECTOR_FUNCTION_H
#define VECTOR_FUNCTION_H
#include <vector>
#include <functional>
#include <cmath>

class VectorFunction {
public:
    using ScalarFunction = std::function<double(double)>;
    int dim=0;
    virtual std::vector<double> operator()(const std::vector<double>& inputU,const double& time) const=0;
    std::vector<double> fixPointIteration(const std::vector<double>& init_values={},const double& time=0,int max_iteration_times=10000)const;
};

double maxNorm(const std::vector<double>& vec);
std::vector<double> CRKstep(const std::vector<double>& last, const double& time, const double& stepsize, const VectorFunction*& func);
std::vector<double> operator+(const std::vector<double>& lhs, const std::vector<double>& rhs);
std::vector<double> operator-(const std::vector<double>& lhs, const std::vector<double>& rhs);
std::vector<double> operator*(double scalar, const std::vector<double>& vec);
std::vector<double> operator*(const std::vector<double>& vec, double scalar);
#endif