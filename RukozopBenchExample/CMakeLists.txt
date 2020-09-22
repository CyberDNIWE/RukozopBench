cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(PROJECT_NAME RukozopBenchExample)
project(${PROJECT_NAME} CXX)

# Set build path for required libraries to be same as executables
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add library dependencies by simply adding subdirectory
add_subdirectory(../RukozopBench ../RukozopBench)

# Add dependencies libs names to variable for convenience
set(DEPENDENT_LIBS RukozopBench)

# Add "main" to sources
set(SOURCES RukozopBenchExample.cpp)

# Generate executable from the sources
add_executable(${PROJECT_NAME} ${SOURCES})

# Link with dependent libraries 
target_link_libraries(${PROJECT_NAME} PRIVATE ${DEPENDENT_LIBS})