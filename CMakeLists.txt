# General parameters
cmake_minimum_required(VERSION 2.6.3)
project(yaml-tests CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
option(COLLECT_FILES    "Copy libraries to <build>/lib, binaries to <build>/bin, headers to <build>/include." ON)
option(COMPILE_EXAMPLES "Compile examples."                                                                  OFF)

###########
## Settings
###########

##
## default options
##
set(CMAKE_VERBOSE_MAKEFILE ON)

if(COLLECT_FILES)
    set(RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
    set(LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
    set(ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif(COLLECT_FILES)

##
## Mandatory dependencies
##

# Search for Eigen
find_package(Eigen3 REQUIRED)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

###############
## Dependencies
###############

add_custom_target(TGT_config-yaml_build_libraries)
add_custom_target(TGT_config-yaml_copy_headers)

# yaml-cpp 
set(YAML-CPP_DIR "${PROJECT_SOURCE_DIR}/contrib/yaml-cpp")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_BUILD_TYPE      Release)

add_subdirectory("${YAML-CPP_DIR}" EXCLUDE_FROM_ALL)

set_property(TARGET yaml-cpp APPEND_STRING PROPERTY COMPILE_FLAGS " -fPIC ")

add_dependencies(TGT_config-yaml_build_libraries yaml-cpp)

if(COLLECT_FILES)
    #yaml-cpp
    add_custom_target(TGT_copy_yaml-cpp_headers COMMAND ${CMAKE_COMMAND} -E
                      copy_directory "${YAML-CPP_DIR}/include" "${PROJECT_BINARY_DIR}/include/yaml-cpp")
    include_directories(SYSTEM "${PROJECT_BINARY_DIR}/include/yaml-cpp")
    add_dependencies(TGT_config-yaml_copy_headers TGT_copy_yaml-cpp_headers)

    #config-yaml
    add_custom_target(TGT_copy_config-yaml_headers COMMAND ${CMAKE_COMMAND} -E
                      copy_directory "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include/config-yaml")
    include_directories(SYSTEM "${PROJECT_BINARY_DIR}/include/config-yaml")
    add_dependencies(TGT_config-yaml_copy_headers TGT_copy_config-yaml_headers)
else(COLLECT_FILES)
    include_directories(SYSTEM "${YAML-CPP_DIR}/include/")
    include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/include/")
endif(COLLECT_FILES)

############
## Variables
############

set(CMAKE_BUILD_TYPE      ${YAML-CPP_BUILD_TYPE})
set(CMAKE_CXX_FLAGS_DEBUG ${YAML-CPP_CXX_FLAGS_DEBUG})

#######
## main 
#######
if(COMPILE_EXAMPLES)
    set(TGT_NAME main)
    add_executable(${TGT_NAME} ${CMAKE_SOURCE_DIR}/examples/main.cpp)

    target_link_libraries(${TGT_NAME} yaml-cpp)

    add_dependencies(${TGT_NAME} TGT_config-yaml_copy_headers)
    add_dependencies(${TGT_NAME} TGT_config-yaml_build_libraries)

    set(CONFIG-YAML_CONFIG_DIR "${PROJECT_SOURCE_DIR}/config")
    set(REF_FILENAME "${CONFIG-YAML_CONFIG_DIR}/config.yaml") 
    file(COPY ${REF_FILENAME} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif(COMPILE_EXAMPLES)
