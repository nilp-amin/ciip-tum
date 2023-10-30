cmake_minimum_required(VERSION 3.14)
project(hw02)

# set default c++ standard for compile targets
set(CMAKE_CXX_STANDARD 20)

# require c++20, don't allow falling back to older version of standard
set(CXX_STANDARD_REQUIRED ON)

# disable c++ extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# build hw02 executables
add_executable(${PROJECT_NAME} combinatorics.cpp run.cpp) 