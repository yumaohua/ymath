#ifndef JSONREAD
#define JSONREAD
#include "./Solver.h"
#include "./FunctionExtension.h"
#include "../libs/configor/json.hpp"
#include <filesystem>
using namespace configor;
std::vector<double> JsonRead(
    std::string filename,
    Function* rhs_function,
    Function* result,
    Function* value_func,
    Function* type_func,
    Function* cvalue_func,
    Function* ctype_func=nullptr){
        Solver solver;
        std::ifstream ifs(filename);
        std::cout<<ifs.is_open()<<std::endl;
        json::value j;
        ifs>>json::wrap(j);
        int grid_number = j["solver"]["grid_number"];
        std::string output_name=j["solver"]["output_name"];
        bool if_subtract_a_cycle = j["domain"]["if_subtract_a_cycle"];
        std::string type = j["domain"]["boundary_condition"]["type"];
        double radius = j["domain"]["cycle"]["radius"];
        double x = j["domain"]["cycle"]["center"][0];
        double y = j["domain"]["cycle"]["center"][1];
        std::vector<double> center{x,y};//{j["domain"]["cycle"]["center"]};
        std::string ctype = j["domain"]["cycle"]["boundary_condition"]["type"];
        solver.Initialize(
            rhs_function,
            grid_number,
            output_name,
            if_subtract_a_cycle,
            type,
            value_func,
            type_func,
            radius,
            center,
            ctype,
            cvalue_func,
            ctype_func
        );
        solver.Define();
        solver.Solve();
        solver.SetError(result);
        solver.Output();
        //solver.maxerrorplace();
        std::cout<<output_name+":"<<solver.ErrorMaxNorm()<<","<<solver.ErrorTwoNorm()<<","<<solver.ErrorOneNorm()<<std::endl;
        return std::vector<double> {(double)grid_number,solver.ErrorMaxNorm(),solver.ErrorTwoNorm(),solver.ErrorOneNorm()};
    }
#endif