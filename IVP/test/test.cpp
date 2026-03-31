#include "Dormand_PrinceRK.h"
#include "UnitTimer.h"
#include "VectorFunction.h"
#include "story.h"
#include "OutPutCapture.h"

void testsolver1 (const int& stepnumber, const std::string& solver, const int& order_min, const int& order_max);
void testsolver (const std::string& solver,const int& order_min,const int& order_max);
int main() {
    // outputcapture("../log/Adams-Bashforth.txt", testsolver, "Adams-Bashforth", 1, 4);
    // outputcapture("../log/Adams-Moulton.txt", testsolver, "Adams-Moulton", 2, 5);
    // outputcapture("../log/Backward_Differentiation.txt", testsolver, "Backward_Differentiation", 1, 4);
    // outputcapture("../log/ClassicalRK.txt", testsolver, "ClassicalRK", 4, 4);
    // outputcapture("../log/ESDIRK.txt", testsolver, "ESDIRK", 4, 4);
    // outputcapture("../log/GLRK.txt", testsolver, "GLRK", 2, 5);
    // outputcapture("../log/FehlbergRK.txt", testsolver, "FehlbergRK", 4, 4);
    // outputcapture("../log/Dormand-PrinceRK.txt", testsolver, "Dormand-PrinceRK", 5, 5);
    // outputcapture("../log/Euler.txt", testsolver, "Euler", 1, 1);
    // outputcapture("../log/Euler1.txt",testsolver1,24000,"Euler",(int)1,(int)1);
    // outputcapture("../log/ClassicalRK1.txt",testsolver1,6000,"ClassicalRK",(int)4,(int)4);
    auto solver1 = Dormand_PrinceRK(5);
    auto a1=std::vector<double>{0.994,0,0,0,-2.0015851063790825224,0};
    auto t1=17.06521656015796;
    auto a2=std::vector<double>{0.879779227778,0,0,0,-0.379677780949,0};
    auto t2=19.140540691377;
    StoryFunction storyfunc;
    const VectorFunction* func = &storyfunc;
    auto& timer = UnitTimer::getInstance();
    timer.reset();
    for(double Eabs=0.00000001; Eabs<1; Eabs+=0.00000001){
        bool flag = false;
        for(double Erel=0.00000001; Erel<1; Erel+=0.00000001){
            solver1.Eabs=Eabs;
            solver1.Erel=Erel;
            solver1.solve(a1, func, 1000, 0, t1);
            if(solver1.stepnum==100) {
                std::cout << "DP190 : "<<"Eabs = "<<Eabs<<", Erel = "<<Erel<<std::endl;
                flag = true;
                timer.begin("DP190");
                solver1.solve(a1, func, 1000, 0, t1);
                timer.end("DP190");
                solver1.output("../output/track1/Dormand_PrinceRK/190/5/Dormand-PrinceRK(stepnumber=100).txt");
                Output(std::vector<std::vector<double>>{{maxNorm(a1-solver1.finalResult()),timer.getTime("DP190")}},"../output/acc1/Dormand-PrinceRK_190.txt");
                break;
            }
        }
        if(flag){break;}
    }    
    for(double Eabs=0.00000001; Eabs<1; Eabs+=0.00000001){
        bool flag = false;
        for(double Erel=0.00000001; Erel<1; Erel+=0.00000001){
            solver1.Eabs=Eabs;
            solver1.Erel=Erel;
            solver1.solve(a2, func, 1000, 0, t2);
            if(solver1.stepnum==100) {
                std::cout << "DP191 : "<<"Eabs = "<<Eabs<<", Erel = "<<Erel<<std::endl;
                flag = true;
                timer.begin("DP191");
                solver1.solve(a2, func, 1000, 0, t2);
                timer.end("DP191");
                solver1.output("../output/track1/Dormand_PrinceRK/191/5/Dormand-PrinceRK(stepnumber=100).txt");
                auto final = solver1.finalResult();
                solver1.solve(a2,func,1000,0.0,t2/2.0);
                auto final1 = solver1.finalResult();
                solver1.solve(final1,func,1000,t2/2.0,t2);
                final1 = solver1.finalResult();
                Output(std::vector<std::vector<double>>{{maxNorm(std::pow(2,5)/(1-std::pow(2,5))*(final1-final)),timer.getTime("DP191")}},"../output/acc1/Dormand-PrinceRK_191.txt");
                break;
            }
        }
        if(flag){break;}
    }
    timer.report();
    std::cout<<"all finish"<<std::endl;
}
void testsolver (const std::string& solver, const int& order_min, const int& order_max) {
    StoryFunction storyfunc;
    const VectorFunction* func = &storyfunc;
    std::string base_path = "../output/track";
    UnitTimer& timer=UnitTimer::getInstance();
    timer.reset();
    std::vector<std::vector<double>> results_190;
    std::vector<std::vector<double>> results_191;
    std::vector<std::vector<double>> mincpu_190;
    std::vector<std::vector<double>> mincpu_191;
    for(int order = order_min; order<order_max+1; order++) {
        auto Func190 = [&](int stepnumber) -> std::vector<double> {
            return storySolve(solver, 190, stepnumber, order, func, base_path, timer);
        };        
        auto Func191 = [&](int stepnumber) -> std::vector<double> {
            return storySolve(solver, 191, stepnumber, order, func, base_path, timer);
        };
        mincpu_190.push_back(findMinStepNumber(0.001, 1000000, Func190));
        mincpu_191.push_back(findMinStepNumber(0.001, 1000000, Func191));
        for(int stepnumber = 1000; stepnumber < 1000001; stepnumber*=10) {
            results_190.push_back(storySolve(solver, 190, stepnumber, order, func, base_path, timer,true));
            results_191.push_back(storySolve(solver, 191, stepnumber, order, func, base_path, timer,true));
        }
    }
    timer.report();
    Output(results_190,"../output/acc/"+solver+"_190.txt");
    Output(results_191,"../output/acc/"+solver+"_191.txt");
    Output(mincpu_190,"../output/mincpu/"+solver+"_190.txt");
    Output(mincpu_191,"../output/mincpu/"+solver+"_191.txt");
}

void testsolver1 (const int& stepnumber, const std::string& solver, const int& order_min, const int& order_max) {
    StoryFunction storyfunc;
    const VectorFunction* func = &storyfunc;
    std::string base_path = "../output/track1";
    UnitTimer& timer=UnitTimer::getInstance();
    timer.reset();
    std::vector<std::vector<double>> results_190;
    std::vector<std::vector<double>> results_191;
    for(int order = order_min; order<order_max+1; order++) {
        results_190.push_back(storySolve(solver, 190, stepnumber, order, func, base_path, timer,true));
        results_191.push_back(storySolve(solver, 191, stepnumber, order, func, base_path, timer,true));
    }
    timer.report();
    Output(results_190,"../output/acc1/"+solver+"_190.txt");
    Output(results_191,"../output/acc1/"+solver+"_191.txt");
}