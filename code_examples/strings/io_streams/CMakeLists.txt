# configure cmake version
cmake_minimum_required(VERSION 3.31 FATAL_ERROR)

# cmake include modules
include(FetchContent)

# project
project(Examples.Concurrency
        LANGUAGES "CXX")

# create target
set(TARGET io_streams)
add_executable(${TARGET} src/main.cpp)