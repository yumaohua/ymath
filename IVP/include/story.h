#ifndef STORY_H
#define STORY_H
#include "IvpSolver.h"
#include "UnitTimer.h"
#include "VectorFunction.h"
#include <string>

class StoryFunction : public VectorFunction {
public:
    int dim = 6;
    double mu = 0.012277471;
    std::vector<double> operator()(const std::vector<double>& inputU,const double& time)const override {
        std::vector<double> result;
        result.push_back(inputU[3]);
        result.push_back(inputU[4]);
        result.push_back(inputU[5]);
        result.push_back(2*inputU[4]+inputU[0]-mu*(inputU[0]+mu-1)/std::pow(inputU[1]*inputU[1]+inputU[2]*inputU[2]+(inputU[0]+mu-1)*(inputU[0]+mu-1),1.5)-(1-mu)*(inputU[0]+mu)/std::pow(inputU[1]*inputU[1]+inputU[2]*inputU[2]+(inputU[0]+mu)*(inputU[0]+mu),1.5));
        result.push_back(-2.0*inputU[3]+inputU[1]-mu*inputU[1]/std::pow(inputU[1]*inputU[1]+inputU[2]*inputU[2]+(inputU[0]+mu-1)*(inputU[0]+mu-1),1.5)-(1-mu)*inputU[1]/std::pow(inputU[1]*inputU[1]+inputU[2]*inputU[2]+(inputU[0]+mu)*(inputU[0]+mu),1.5));
        result.push_back(-mu*inputU[2]/std::pow(inputU[1]*inputU[1]+inputU[2]*inputU[2]+(inputU[0]+mu-1)*(inputU[0]+mu-1),1.5)-(1-mu)*inputU[2]/std::pow(inputU[1]*inputU[1]+inputU[2]*inputU[2]+(inputU[0]+mu)*(inputU[0]+mu),1.5));
        return result;
    }
};

std::vector<double> storySolve(
    const std::string& solver_name, 
    const int& workcase,
    const int& stepnumber,
    const int& order,
    const VectorFunction*& func,
    const std::string& base_path,
    UnitTimer& timer,
    const bool& ifoutput=false);

std::vector<double> findMinStepNumber(double tolerance, int maxStepNumber,const std::function<std::vector<double>(int)> &Func); 

#endif