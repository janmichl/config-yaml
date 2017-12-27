/**
    @file
    @author Jan Michalczyk

    @brief
*/

#pragma once

#include "config-yaml/config_reader.h"

namespace yaml_config
{
    /**
     * @brief Configurable class
     */
    class ConfigurableClassBase
    {
        public:
            /**
             * @brief Constructor
             *
             * @param[in] config_file
             */
            ConfigurableClassBase(const char* config_file) : config_reader(config_file)
            {
                readParameters("ConfigurableClassBase");
            }
        

        private:
            /**
             * @brief Read parameters
             *
             * @param[in] node_name
             */
            void readParameters(const std::string& node_name)
            {       
                config_reader.readScalar(node_name, "uint1", uint1);
                std::cout << uint1 << std::endl;
                config_reader.readScalar(node_name, "uint2", uint2);
                std::cout << uint2 << std::endl;
                config_reader.readScalar(node_name, "uint3", uint3);
                std::cout << uint3 << std::endl;
                config_reader.readScalar(node_name, "uint4", uint4);
                std::cout << uint4 << std::endl;
                config_reader.readScalar(node_name, "uint5", uint5);
                std::cout << uint5 << std::endl;
                config_reader.readScalar(node_name, "uint6", uint6);
                std::cout << uint6 << std::endl;
            }


        private:
            yaml_config::ConfigReader config_reader;
            
            unsigned int uint1;
            unsigned int uint2;
            unsigned int uint3;
            unsigned int uint4;
            unsigned int uint5;
            unsigned int uint6;
    };
}//yaml_config
