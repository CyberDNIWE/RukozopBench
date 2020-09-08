cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME RukozopBenchExample)
project(${PROJECT_NAME} CXX)

# Set build path for required libraries to be same as executables
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SOURCES RukozopBenchExample.cpp)
#Generate executable from the sources
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE "../RukozopBench")