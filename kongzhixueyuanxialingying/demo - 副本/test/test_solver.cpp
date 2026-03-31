#include "solver.h"
#include <iostream>
// 示例常微分方程组
Vec exampleODE(const Vec& y, double t, const Vec& u, double h) {
    Vec dydt(y.size());
    dydt[0] = y[1]; // dy1/dt = y2
    dydt[1] = -y[0]; // dy2/dt = -y1
    return dydt;
}

void run_solver_test() {
    std::cout<<"test_solver_begin:================================================"<<std::endl;
    SolverFactory& factory = SolverFactory::getInstance();
    factory.registerSolver("RK4", std::make_shared<RungeKutta4Solver>());
    
    auto solver = factory.getSolver("RK4");
    Vec y0 = {1.0, 0.0};
    double t_end = 10.0;
    int partitions = 100;
    
    Vec y_final = solver->solve(exampleODE, y0, {}, t_end, partitions);
    
    std::cout << "Final values: ";
    for (double value : y_final) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout<<"test_solver_end:================================================"<<std::endl;
}
