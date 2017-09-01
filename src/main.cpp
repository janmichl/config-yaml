/**
    @file
    @author Jan Michalczyk

    @brief
*/

#include <iostream>

#include "configurable_class_derived.h"

int main()
{
    try
    {
        yaml_config::ConfigurableClassDerived configurable_class_derived("config.yaml"); 
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        exit(-1);
    }

    return(0);	
}
