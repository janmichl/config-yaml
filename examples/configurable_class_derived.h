/**
    @file
    @author Jan Michalczyk

    @brief
*/

#pragma once

#include <string>
#include <vector>

#include "config-yaml/config_reader.h"

#include "configurable_class_base.h"

namespace yaml_config
{
    /**
     * @brief Enum in configuration
     */
    enum Enum
    {
        UNDEFINED = 0,
        FIRST     = 1,
        SECOND    = 2
    };

    /**
     * @brief Configurable class
     */
    class ConfigurableClassDerived : public ConfigurableClassBase
    {
        public:
            /**
             * @brief Constructor
             *
             * @param[in] config_file
             */
            ConfigurableClassDerived(const char* config_file) : ConfigurableClassBase(config_file), config_reader(config_file)
            {
                readParameters("ConfigurableClassDerived");
            }
        

        private:
            /**
             * @brief Read parameters
             *
             * @param[in] node_name
             */
            void readParameters(const std::string& node_name)
            {      
                config_reader.readScalar(node_name, "string1", string1);
                std::cout << string1 << std::endl;
                config_reader.readScalar(node_name, "string2", string2);
                std::cout << string2 << std::endl;
                config_reader.readScalar(node_name, "string3", string3);
                std::cout << string3 << std::endl;
                config_reader.readScalar(node_name, "string4", string4);
                std::cout << string4 << std::endl;
                config_reader.readVector(node_name, "vector1", vector1);
                std::cout << vector1 << std::endl;
                config_reader.readVector(node_name, "vector2", vector2);
                std::cout << vector2 << std::endl;

                config_reader.readVector(node_name, "vector3", vector3);
                for(std::size_t i = 0; i < vector3.size(); ++i)
                {
                    std::cout << vector3[i] << std::endl;
                }
                
                config_reader.readEnum(node_name, "enum1", enum1);
                std::cout << enum1 << std::endl;
            }


        private:
            yaml_config::ConfigReader config_reader;
            
            std::string string1;
            std::string string2;
            std::string string3;
            std::string string4;

            Eigen::Vector2d vector1;
            Eigen::Vector2d vector2;

            std::vector<std::string> vector3;

            Enum enum1;
    };
}//yaml_config
