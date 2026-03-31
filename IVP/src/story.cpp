#include "story.h"
#include "TimeIntegrator.h"
#include <memory>

std::vector<double> storySolve(
    const std::string& solver_name, 
    const int& workcase,
    const int& stepnumber,
    const int& order,
    const VectorFunction*& func,
    const std::string& base_path,
    UnitTimer& timer,
    const bool& ifoutput){
        std::string flag = "step_number="+std::to_string(stepnumber)+",order="+std::to_string(order);
        std::cout<<solver_name<<"_"<<std::to_string(workcase)<<"("<<flag<<"):"<<std::endl;
        std::unique_ptr<TimeIntegrator> solver = TimeIntegratorFactory::getInstance().createTimeIntegrator(solver_name, order);
        std::vector<double> a;
        double t;
        if(workcase == 190){
            a=std::vector<double>{0.994,0,0,0,-2.0015851063790825224,0};
            t=17.06521656015796;
        }
        else if(workcase == 191) {
            a=std::vector<double>{0.879779227778,0,0,0,-0.379677780949,0};
            t=19.140540691377;
        }
        else {
            std::cerr<<"no such homework case."<<std::endl;
            return {};
        }
        std::string taskname = solver_name+"_"+std::to_string(workcase)+"_stepnumber="+std::to_string(stepnumber)+"_order="+std::to_string(order);
        timer.begin(taskname);
        solver->solve(a,func,stepnumber,0.0,t);
        timer.end(taskname);
        std::string outputpath = base_path+"/"+solver_name+"/"+std::to_string(workcase)+"/"+std::to_string(order)+"/"+solver_name+"_"+std::to_string(workcase)+"(order="+std::to_string(order)+",stepnumber="+std::to_string(stepnumber)+").txt";
        std::vector<double> finalresult = solver->finalResult();
        if(ifoutput) {solver->output(outputpath,true);}      
        std::vector<double> error;
        if(workcase == 190) {
            std::cout<<"10.190 solution errors: ";
            error=a-finalresult;
            for(auto v: error){
                std::cout<<v<<" ";
            }
            std::cout<<std::endl;
        }
        else if(workcase == 191) {
            solver->solve(a,func,stepnumber,0.0,19.140540691377/2.0);
            std::vector<double> finalresult1 = solver->finalResult();
            solver->solve(finalresult1,func,stepnumber,19.140540691377/2.0,19.140540691377);
            finalresult1 = solver->finalResult();
            std::cout<<"10.191 solution errors: ";
            error = std::pow(2,order)/(1-std::pow(2,order))*(finalresult1-finalresult);
            for(auto v: error){
                std::cout<<v<<" ";
            }
            std::cout<<std::endl;
        }
        double maxnorm = maxNorm(error);
        std::cout<<"maxNorm of solution error:"<<maxnorm<<std::endl;
        double tasktime = timer.getTime(taskname);
        std::cout<<"cpuTime :"<< tasktime<<std::endl;
        std::cout<<"-------------------------------------------------"<<std::endl;
        return std::vector<double> {maxnorm,tasktime};
}

std::vector<double> findMinStepNumber(double tolerance, int maxStepNumber,const std::function<std::vector<double>(int)> &Func) {
    int low = 1;
    int high = maxStepNumber;
    std::vector<double> result = {-1, 0.0, 0.0};

    while (low <= high) {
        int mid = (high + low) / 2;
        std::vector<double> values = Func(mid);
        double maxNorm = values[0];
        double cpuTime = values[1];

        if (maxNorm < tolerance) {
            result[0] = mid;
            result[1] = maxNorm;
            result[2] = cpuTime;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return result;
}
