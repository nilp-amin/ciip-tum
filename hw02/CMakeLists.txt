cmake_minimum_required(VERSION 3.14)
project(hw02)

# set default c++ standard for compile targets
set(CMAKE_CXX_STANDARD 20)

# require c++20, don't allow falling back to older version of standard
set(CXX_STANDARD_REQUIRED ON)

# disable c++ extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# build the combinatornics library
add_library(${PROJECT_NAME} combinatorics.cpp combinatorics.h)

target_include_directories(${PROJECT_NAME} PUBLIC .)

# build hw02 executable
add_executable(${PROJECT_NAME}_run run.cpp) 
target_link_libraries(${PROJECT_NAME}_run ${PROJECT_NAME})