#include "../include/Domain.h"
bool Domain::Prepare(){
    std::vector<bool> component_prepare;
    return TestPrepare("Domain",component_prepare);
}