#include "VectorFunction.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>


std::vector<double> VectorFunction::fixPointIteration(const std::vector<double>& init_values,const double & time,int max_iteration_times)const {
    std::vector<double> result{init_values};
    if (result.size()==0){
        result.resize(dim,0.0);
    }
    double epsilon = std::numeric_limits<double>::epsilon();
    int iterations = 0;
    while(true) {
        std::vector<double> newresult {(*this)(result,time)};
        iterations++;
        if(maxNorm(newresult-result)<epsilon) {
            result=newresult;
            break;
        }
        if(iterations==max_iteration_times){
            std::cerr << "max iteration times reached" << std::endl;
            break;
        }
        result=newresult;
    }
    return result;
}
double maxNorm(const std::vector<double>& vec) {
    if (vec.empty()) return 0.0;
    double result = *std::max_element(vec.begin(), vec.end(), [](double a, double b) {
        return std::abs(a) < std::abs(b);
    });
    return std::abs(result);
}

std::vector<double> CRKstep(const std::vector<double>& last, const double& time, const double& stepsize, const VectorFunction*& func) {
    std::vector<double> y1 = (*func)(last,time);
    std::vector<double> y2 = (*func)(last+stepsize/2.0*y1,time+stepsize/2.0);
    std::vector<double> y3 = (*func)(last+stepsize/2.0*y2,time+stepsize/2.0);
    std::vector<double> y4 = (*func)(last+stepsize*y3,time+stepsize);
    std::vector<double> next = last + stepsize/6.0*(y1+2.0*y2+2.0*y3+y4);
    return next;
}

std::vector<double> operator+(const std::vector<double>& lhs, const std::vector<double>& rhs) {
    if (lhs.size() != rhs.size()) {
        std::cerr << "dimension don't match in vector plus" <<std::endl;
    }

    std::vector<double> result(lhs.size());
    for (size_t i = 0; i < lhs.size(); ++i) {
        result[i] = lhs[i] + rhs[i];
    }

    return result;
}
std::vector<double> operator-(const std::vector<double>& lhs, const std::vector<double>& rhs) {
    if (lhs.size() != rhs.size()) {
        std::cerr << "dimension don't match in vector plus" <<std::endl;
    }

    std::vector<double> result(lhs.size());
    for (size_t i = 0; i < lhs.size(); ++i) {
        result[i] = lhs[i] - rhs[i];
    }

    return result;
}

std::vector<double> operator*(double scalar, const std::vector<double>& vec) {
    std::vector<double> result(vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
        result[i] = scalar * vec[i];
    }
    return result;
}

std::vector<double> operator*(const std::vector<double>& vec, double scalar) {
    return scalar * vec;
}