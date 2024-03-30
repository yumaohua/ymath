#include "../include/Assistance.h"
bool TestPrepare(const std::string& name,const std::vector<bool>& component_prepare)
{
        bool prepare=true;
        for(auto ifprepare:component_prepare){
            if(!ifprepare){
                prepare=false;
            }
        }
        if(!prepare)
        {
            std::cout<<name<<": ";
            for(auto ifprepare:component_prepare)
            {
                std::cout<<ifprepare<<", ";
            }
            std::cout<<std::endl;
        }
        return prepare;
}

std::string VS(int i, int j)
{
    return std::to_string(i)+","+std::to_string(j);
}
std::vector<int> SV(std::string s)
{
    auto it=s.find(",");
    std::string i = s.substr(0,it);
    std::string j = s.substr(it+1,s.length()-it-1);
    return std::vector<int>{atoi(i.c_str()),atoi(j.c_str())};
}