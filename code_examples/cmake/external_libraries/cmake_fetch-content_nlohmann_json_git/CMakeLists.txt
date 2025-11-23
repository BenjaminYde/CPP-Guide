# define project
cmake_minimum_required(VERSION 3.31 FATAL_ERROR)
project(MyProject CXX)

# variables
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# add executable
add_executable(MyTarget ${SOURCE_DIR}/main.cpp)

#include cmake modules
include(FetchContent)

# package - declare: nlohmann_json
FetchContent_Declare(external_nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.12.0
)

# package - make available
FetchContent_MakeAvailable(external_nlohmann_json)

# package - link libraries
target_link_libraries(MyTarget PRIVATE nlohmann_json::nlohmann_json)