#include <iostream>
#include <string>
#include "string_function.h"
int main()
{
    ExpressionTreeNode node{"((((2))*8.888))+abs(-4)"};
    node.testshow();
    std::cout<<std::endl;
    std::cout<<node.calculate()<<std::endl;
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