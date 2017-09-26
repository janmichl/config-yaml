/**
    @file
    @author Jan Michalczyk

    @brief Config file reader
*/

#pragma once

#include <exception>
#include <string>
#include <vector>
#include <fstream>

#include <Eigen/Core>

#include "yaml-cpp/yaml.h"

namespace yaml_config
{
    class ConfigReader
    {
        public:
            ConfigReader(const std::string& filename)
            {
                ifs_.open(filename.c_str());
                if(!ifs_)
                {
                    std::cerr << "Cannot find file." << std::endl;
                    throw(std::runtime_error("File not found."));
                }

                parser_.Load(ifs_),
                parser_.GetNextDocument(root_node_);
            }


            ConfigReader()
            {
            }


            template <typename t_Scalar,
                      int t_rows,
                      int t_flags>
                void readVector(const std::string&                           node_name,
                                const std::string&                           value_name,
                                Eigen::Matrix<t_Scalar, t_rows, 1, t_flags>& to_read)
            {
                    const YAML::Node* node = findNode(findNode(&root_node_, node_name), value_name);
                    if(!(node->Type() == YAML::NodeType::Sequence))
                    {
                        std::cerr << "Entry is not a sequence." << std::endl;
                        throw(std::runtime_error("Entry not a sequence."));
                    }
                    
                    if(Eigen::Dynamic == t_rows)
                    {
                        to_read.resize(node->size());
                    }
                    for(std::size_t i = 0; i < to_read.size(); ++i)
                    {
                        (*node)[i] >> to_read(i);
                    }
            }


            template<typename t>
                void readScalar(const std::string& node_name,
                                const std::string& value_name,
                                t&                 to_read)
            {
                const YAML::Node* node = findNode(findNode(&root_node_, node_name), value_name);
                *node >> to_read; 
            }
            
            
            template<typename t>
                void readVector(const std::string& node_name,
                                const std::string& value_name,
                                std::vector<t>&    to_read)
            {
                const YAML::Node* node = findNode(findNode(&root_node_, node_name), value_name);
                if(!(node->Type() == YAML::NodeType::Sequence))
                {
                    std::cerr << "Entry is not a sequence." << std::endl;
                    throw(std::runtime_error("Entry not a sequence."));
                }
                
                to_read.resize(node->size());
                for(std::size_t i = 0; i < to_read.size(); ++i)
                {
                    (*node)[i] >> to_read[i];
                }
            }


            template<typename t>
                void readEnum(const std::string& node_name, 
                              const std::string& value_name,
                              t&                 to_read)
            {
                const YAML::Node* node = findNode(findNode(&root_node_, node_name), value_name);

                int enum_value = 0;
                *node >> enum_value;
                to_read = static_cast<t>(enum_value);
            }


        private:
            const YAML::Node* findNode(const YAML::Node* current_node, const std::string& node_name)
            {
                return(current_node->FindValue(node_name));
            }


        private:
            YAML::Node    root_node_;
            YAML::Parser  parser_;
            std::ifstream ifs_;
    };
}//yaml_config
