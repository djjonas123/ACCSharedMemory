cmake_minimum_required(VERSION 3.16)
project(ACCSharedMemory)

set(CMAKE_CXX_STANDARD 17)

find_package(Python 3.9 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(ACCSharedMemory SM.cpp)

target_compile_definitions(ACCSharedMemory
        PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

#used for local testing
#add_executable(ACCSharedMemory SM.cpp stdafx.cpp)
