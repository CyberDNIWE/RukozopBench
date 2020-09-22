cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME RukozopBench)
project(${PROJECT_NAME} CXX)

#Add librarys as interface (header only, visible only to user)
add_library(${PROJECT_NAME} INTERFACE)

#Add include directory
target_include_directories(${PROJECT_NAME} INTERFACE ".")