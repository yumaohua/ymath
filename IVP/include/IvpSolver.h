#ifndef IVP_SOLVER_H
#define IVP_SOLVER_H
#include "Adams_Bashforth_Methods.h"
#include "Adams_Moulton_Methods.h"
#include "Backward_Differentiation_Formula.h"
#include "ClassicalRK.h"
#include "ESDIRK.h"
#include "GLRK.h"
#include "FehlbergRK.h"
#include "Dormand_PrinceRK.h"
#include "Euler_Method.h"
static void registerAdamsBashforthMethods(void)__attribute__((constructor));
void registerAdamsBashforthMethods(){
    auto& factory = TimeIntegratorFactory::getInstance();
    factory.registerTimeIntegrator("Adams-Bashforth",
        [](int p)->std::unique_ptr<TimeIntegrator>{return std::make_unique<Adams_Bashforth_Methods>(p);});
}
static void registerAdamsMoultonMethods(void)__attribute__((constructor));
void registerAdamsMoultonMethods(){
    auto& factory = TimeIntegratorFactory::getInstance();
    factory.registerTimeIntegrator("Adams-Moulton",
        [](int p)->std::unique_ptr<TimeIntegrator>{return std::make_unique<Adams_Moulton_Methods>(p);});
}
static void registerBackwardDifferentiationFormula(void)__attribute__((constructor));
void registerBackwardDifferentiationFormula(){
    auto& factory = TimeIntegratorFactory::getInstance();
    factory.registerTimeIntegrator("Backward_Differentiation",
        [](int p)->std::unique_ptr<TimeIntegrator>{return std::make_unique<Backward_Differentiation_Formula>(p);});
}
static void registerClassicalRK(void)__attribute__((constructor));
void registerClassicalRK(){
    auto& factory = TimeIntegratorFactory::getInstance();
    factory.registerTimeIntegrator("ClassicalRK",
        [](int p)->std::unique_ptr<TimeIntegrator>{return std::make_unique<ClassicalRK>(p);});
}
static void registerESDIRK(void)__attribute__((constructor));
void registerESDIRK(){
    auto& factory = TimeIntegratorFactory::getInstance();
    factory.registerTimeIntegrator("ESDIRK",
        [](int p)->std::unique_ptr<TimeIntegrator>{return std::make_unique<ESDIRK>(p);});
}
static void registerGLRK(void)__attribute__((constructor));
void registerGLRK(){
    auto& factory = TimeIntegratorFactory::getInstance();
    factory.registerTimeIntegrator("GLRK",
        [](int p)->std::unique_ptr<TimeIntegrator>{return std::make_unique<GLRK>(p);});
}
static void registerFehlbergRK(void)__attribute__((constructor));
void registerFehlbergRK(){
    auto& factory = TimeIntegratorFactory::getInstance();
    factory.registerTimeIntegrator("FehlbergRK",
        [](int p)->std::unique_ptr<TimeIntegrator>{return std::make_unique<FehlbergRK>(p);});
}
static void registerDormand_PrinceRK(void)__attribute__((constructor));
void registerDormand_PrinceRK(){
    auto& factory = TimeIntegratorFactory::getInstance();
    factory.registerTimeIntegrator("Dormand-PrinceRK",
        [](int p)->std::unique_ptr<TimeIntegrator>{return std::make_unique<Dormand_PrinceRK>(p);});
}
static void registerEulerMethod(void)__attribute__((constructor));
void registerEulerMethod(){
    auto& factory = TimeIntegratorFactory::getInstance();
    factory.registerTimeIntegrator("Euler",
        [](int p)->std::unique_ptr<TimeIntegrator>{return std::make_unique<Euler_Method>(p);});
}
#endif