#include <iostream>
#include <string>
#include "string_function.h"
int main()
{
    std::vector<std::string> names{"@x"};
    std::vector<double> values{-4};
    std::string expression{"3+(@x)"}; 
    ExpressionTree tree{expression,names,values};
    std::cout<<tree.calculate()<<std::endl;
    std::vector<double> values2{-1};
    tree.passvariablevalues(values2);
    std::cout<<tree.calculate()<<std::endl;
    // std::vector<std::string> v;
    // v.push_back("not");
    // v.push_back("@a");
    // v.push_back("@b");
    // std::string c{"@c"};
    // for(auto s:v)
    // {
    //     if(c.compare(s)!=0)
    //     {
    //         std::cout<<c<<std::endl;
    //     }
    // }

}