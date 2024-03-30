#include"../include/JsonRead.h"
#include"../include/FunctionExtension.h"
#include <filesystem>
void Normoutput(std::string filename,
const std::vector<double>& Norm1,
const std::vector<double>& Norm2,
const std::vector<double>& Norm3,
const std::vector<double>& Norm4);
int main()
{
    //std::cout<<std::filesystem::current_path()<<std::endl;
    RealFunction1 value_function1;
    RHSFunction1 rhs_function1;
    MixedFunction1 mixed_function1;
    MixedTypeFunction1 type_function1;
    CMixedFunction1 cmixed_function1;
    CMixedTypeFunction1 ctype_function1;
    Function* value1 = &value_function1;
    Function* rhs1 = &rhs_function1;
    Function* mixed1 = &mixed_function1;
    Function* type1 = &type_function1;
    Function* cmixed1 = &cmixed_function1;
    Function* ctype1 = & ctype_function1;
    std::vector<double> n8f1d = JsonRead("./input/f1/n8f1d.json",rhs1,value1,value1,nullptr,nullptr,nullptr);
    std::vector<double> n8f1dcd = JsonRead("./input/f1/n8f1dcd.json",rhs1,value1,value1,nullptr,value1,nullptr);
    std::vector<double> n8f1dcm = JsonRead("./input/f1/n8f1dcm.json",rhs1,value1,value1,nullptr,cmixed1,ctype1);
    std::vector<double> n8f1m = JsonRead("./input/f1/n8f1m.json",rhs1,value1,mixed1,type1,nullptr,nullptr);
    std::vector<double> n8f1mcd = JsonRead("./input/f1/n8f1mcd.json",rhs1,value1,mixed1,type1,value1,nullptr);
    std::vector<double> n8f1mcm = JsonRead("./input/f1/n8f1mcm.json",rhs1,value1,mixed1,type1,cmixed1,ctype1);
    std::vector<double> n16f1d = JsonRead("./input/f1/n16f1d.json",rhs1,value1,value1,nullptr,nullptr,nullptr);
    std::vector<double> n16f1dcd = JsonRead("./input/f1/n16f1dcd.json",rhs1,value1,value1,nullptr,value1,nullptr);
    std::vector<double> n16f1dcm = JsonRead("./input/f1/n16f1dcm.json",rhs1,value1,value1,nullptr,cmixed1,ctype1);
    std::vector<double> n16f1m = JsonRead("./input/f1/n16f1m.json",rhs1,value1,mixed1,type1,nullptr,nullptr);
    std::vector<double> n16f1mcd = JsonRead("./input/f1/n16f1mcd.json",rhs1,value1,mixed1,type1,value1,nullptr);
    std::vector<double> n16f1mcm = JsonRead("./input/f1/n16f1mcm.json",rhs1,value1,mixed1,type1,cmixed1,ctype1);
    std::vector<double> n32f1d = JsonRead("./input/f1/n32f1d.json",rhs1,value1,value1,nullptr,nullptr,nullptr);
    std::vector<double> n32f1dcd = JsonRead("./input/f1/n32f1dcd.json",rhs1,value1,value1,nullptr,value1,nullptr);
    std::vector<double> n32f1dcm = JsonRead("./input/f1/n32f1dcm.json",rhs1,value1,value1,nullptr,cmixed1,ctype1);
    std::vector<double> n32f1m = JsonRead("./input/f1/n32f1m.json",rhs1,value1,mixed1,type1,nullptr,nullptr);
    std::vector<double> n32f1mcd = JsonRead("./input/f1/n32f1mcd.json",rhs1,value1,mixed1,type1,value1,nullptr);
    std::vector<double> n32f1mcm = JsonRead("./input/f1/n32f1mcm.json",rhs1,value1,mixed1,type1,cmixed1,ctype1);
    std::vector<double> n64f1d = JsonRead("./input/f1/n64f1d.json",rhs1,value1,value1,nullptr,nullptr,nullptr);
    std::vector<double> n64f1dcd = JsonRead("./input/f1/n64f1dcd.json",rhs1,value1,value1,nullptr,value1,nullptr);
    std::vector<double> n64f1dcm = JsonRead("./input/f1/n64f1dcm.json",rhs1,value1,value1,nullptr,cmixed1,ctype1);
    std::vector<double> n64f1m = JsonRead("./input/f1/n64f1m.json",rhs1,value1,mixed1,type1,nullptr,nullptr);
    std::vector<double> n64f1mcd = JsonRead("./input/f1/n64f1mcd.json",rhs1,value1,mixed1,type1,value1,nullptr);
    std::vector<double> n64f1mcm = JsonRead("./input/f1/n64f1mcm.json",rhs1,value1,mixed1,type1,cmixed1,ctype1);
    Normoutput("f1d",n8f1d,n16f1d,n32f1d,n64f1d);
    Normoutput("f1dcd",n8f1dcd,n16f1dcd,n32f1dcd,n64f1dcd);
    Normoutput("f1dcm",n8f1dcm,n16f1dcm,n32f1dcm,n64f1dcm);
    Normoutput("f1m",n8f1m,n16f1m,n32f1m,n64f1m);
    Normoutput("f1mcd",n8f1mcd,n16f1mcd,n32f1mcd,n64f1mcd);
    Normoutput("f1mcm",n8f1mcm,n16f1mcm,n32f1mcm,n64f1mcm);
}

void Normoutput(std::string filename,
const std::vector<double>& Norm1,
const std::vector<double>& Norm2,
const std::vector<double>& Norm3,
const std::vector<double>& Norm4)
{
    std::string outname = "./output/"+filename+".txt";
    std::ofstream ofs(outname);
    ofs<<Norm1[0]<<","<<Norm1[1]<<","<<Norm1[2]<<","<<Norm1[3]<<std::endl;
    ofs<<Norm2[0]<<","<<Norm2[1]<<","<<Norm2[2]<<","<<Norm2[3]<<std::endl;
    ofs<<Norm3[0]<<","<<Norm3[1]<<","<<Norm3[2]<<","<<Norm3[3]<<std::endl;
    ofs<<Norm4[0]<<","<<Norm4[1]<<","<<Norm4[2]<<","<<Norm4[3]<<std::endl;
    ofs.close();
}
