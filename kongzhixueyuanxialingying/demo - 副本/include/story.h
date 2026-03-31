#ifndef STORY_H
#define STORY_H
#include "solver.h"
class AutoSolverRegister {
public:
    AutoSolverRegister() {
        SolverFactory::getInstance().registerSolver("RK4", std::make_shared<RungeKutta4Solver>());
        SolverFactory::getInstance().registerSolver("RK5", std::make_shared<RungeKutta5Solver>());
        SolverFactory::getInstance().registerSolver("ERK", std::make_shared<AdaptiveRungeKuttaSolver>());
    }
};
static AutoSolverRegister autoSolverRegisterInstance;
Vec ODE(const Vec& x, double t, const Vec& u, double h) {
    Vec dxdt(x.size());
    double uval = u[floor(t/h)];
    dxdt[0] = uval*(10*x[1]-x[0]);
    dxdt[1] = uval*(x[0]-10*x[1])-(1.0-uval)*x[1];
    return dxdt;
}
double Val(const Vec& u) {
    Vec x {1.0,0.0};
    int partitions = u.size();
    std::shared_ptr<Solver> solver = SolverFactory::getInstance().getSolver("ERK");
    Vec res = solver->solve(
        ODE,x,u,10.0,partitions
    );
    return 1.0-res[0]-res[1];
}
// double Val(const Vec& u) {
//     return -(u[1]-0.8)*(u[1]-0.8)-(u[0]-0.2)*(u[0]-0.2)+1;
// }
#endif //STORY_H