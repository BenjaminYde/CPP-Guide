# define project
cmake_minimum_required(VERSION 3.31 FATAL_ERROR)
project(MyProject CXX)

# variables
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(TARGET_NAME "cppguide_examples_qt_imageblender")

# include cmake modules
include(FetchContent)

# find qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# create cexecutable
add_executable(${TARGET_NAME} src/main.cpp)

# link libraries
target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Widgets)

# set platform properties
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(cmake/windows/executable.cmake)
endif()