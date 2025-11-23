# configure cmake version
cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

# cmake include modules
include(FetchContent)

# project
project(RuleOfFive)

# create target
set(TARGET MyTarget)
add_executable(${TARGET} src/main.cpp)