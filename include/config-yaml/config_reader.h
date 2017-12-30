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
    /**
     * @brief Config reader class
     */
    class ConfigReader
    {
        public:
            /**
             * @brief Constructor
             *
             * @param[in] filename
             */
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


            /**
             * @brief Default constructor
             */
            ConfigReader()
            {
            }


            /**
             * @brief Read Eigen vector
             *
             * @tparam t_Scalar
             * @tparam t_rows
             * @tparam t_flags
             *
             * @param[in]      node_name
             * @param[in]      value_name
             * @param[in, out] to_read
             */
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


            /**
             * @brief Read Eigen matrix
             *
             * @tparam t_Scalar Eigen template parameter
             * @tparam t_rows   Eigen template parameter
             * @tparam t_cols   Eigen template parameter
             * @tparam t_flags  Eigen template parameter
             *
             * @param[out] entry      data
             * @param[in]  entry_name name
             */
            template <  typename t_Scalar,
                        int t_rows,
                        int t_cols,
                        int t_flags>
                void readMatrix(const std::string& node_name,
                                const std::string& value_name,
                                Eigen::Matrix<t_Scalar, t_rows, t_cols, t_flags>& to_read)
            {
                const YAML::Node* cols_node = findNode(findNode(findNode(&root_node_, node_name), value_name), "cols");
                const YAML::Node* rows_node = findNode(findNode(findNode(&root_node_, node_name), value_name), "rows");
                const YAML::Node* data_node = findNode(findNode(findNode(&root_node_, node_name), value_name), "data");
                
                if(!(data_node->Type() == YAML::NodeType::Sequence))
                {
                    std::cerr << "Entry is not a sequence." << std::endl;
                    throw(std::runtime_error("Entry not a sequence."));
                }
                
                std::size_t num_rows;
                std::size_t num_cols;
                *cols_node >> num_cols;
                *rows_node >> num_rows;

                Eigen::VectorXd content;
                if(Eigen::Dynamic == t_rows)
                {
                    content.resize(data_node->size());
                }
                
                for(std::size_t i = 0; i < content.size(); ++i)
                {
                    (*data_node)[i] >> content(i);
                }
                
                if(!(content.rows() == num_rows * num_cols))
                {
                    std::cerr << "Wrong matrix shape." << std::endl;
                    throw(std::runtime_error("Wrong matrix shape."));
                }
                
                Eigen::Map< Eigen::Matrix<  double,
                                            Eigen::Dynamic,
                                            Eigen::Dynamic,
                                            Eigen::RowMajor> >  map(content.data(), num_rows, num_cols);
                to_read = map;
            }


            /**
             * @brief Read scalar
             *
             * @tparam t
             *
             * @param[in]      node_name
             * @param[in]      value_name
             * @param[in, out] to_read
             */
            template<typename t>
                void readScalar(const std::string& node_name,
                                const std::string& value_name,
                                t&                 to_read)
            {
                const YAML::Node* node = findNode(findNode(&root_node_, node_name), value_name);
                *node >> to_read; 
            }
            
            
            /**
             * @brief Read vector
             *
             * @tparam t
             *
             * @param[in]      node_name
             * @param[in]      value_name
             * @param[in, out] to_read
             */
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


            /**
             * @brief Read enum
             *
             * @tparam t
             *
             * @param[in]      node_name
             * @param[in]      value_name
             * @param[in, out] to_read
             */
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
            /**
             * @brief Find node
             *
             * @param[in] current_node
             * @param[in] node_name
             */
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
