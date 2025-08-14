# CMake Targets

A target in CMake represents a logical unit in your project, such as an executable, a library, or a custom command. Targets encapsulate properties, dependencies, and build rules, allowing you to manage your project modularly.

Here is a minimal example of a `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.31)
project(MyProject 
    VERSION 1.0 
    LANGUAGES CXX)

# set compiler configurations
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_VERSION 20.1.8)

# enforce modern C++ standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add executable
add_executable(MyApp 
    src/main.cpp)

# add include directory
include_directories(include)

# add library
add_library(MyLib 
    src/MyLib.cpp 
    src/MyLibHelper.cpp)

# link library to executable
target_link_libraries(MyApp PRIVATE MyLib)
```

Here 2 target commands are used:

- `add_executable()`: defines the main executable app named `MyApp`.
- `target_link_libraries()`: links the library to the `MyApp` executable.

## Target Types (overview)

### 1. Executable Targets:

Created using `add_executable()`.

For example, define an executable target from specified source files:

```sh
add_executable(MyApp main.cpp)
```

### 2. Library Targets

Created using `add_library()`.

For example, create a library target, which can be static, shared, or module:

```sh
add_library(MyLib STATIC lib.cpp)
```

### 3. Custom Targets

Defines a target that performs custom commands, which is always considered out-of-date and can be used to run arbitrary build steps.

Created using `add_custom_target()` for custom build actions.

```sh
add_custom_target(run_tests
    COMMAND ${CMAKE_COMMAND} -E echo "Running tests..."
    COMMAND ${CMAKE_COMMAND} -E touch run_tests.stamp
    COMMENT "Executing custom target: run_tests"
)
```

## Target Commands (overview)

### 1. `target_sources()`

Associates source files with a target, allowing for dynamic source file specification.

```sh
add_library(MyLib STATIC)
target_sources(MyLib
    PRIVATE
        lib.cpp
    PUBLIC
        lib.h
)
```

### 2. `target_include_directories()`

Sets include directories for a target, specifying where the compiler should look for header files.

```sh
add_library(MyLib STATIC lib.cpp)
target_include_directories(MyLib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)
```

This command specifies that the compiler should search for headers in `${CMAKE_SOURCE_DIR}/`include when compiling MyLib and in `${CMAKE_SOURCE_DIR}/src` only for the target itself.

### 3. `target_link_libraries()`

Specifies libraries that a target depends on.

```sh
add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE MyLib)
```

### 4. `target_compile_definitions()`

Adds preprocessor definitions to a target, which can be used to conditionally compile code.

```sh
add_library(MyLib STATIC lib.cpp)
target_compile_definitions(MyLib
    PRIVATE
        MYLIB_INTERNAL=1
    PUBLIC
        MYLIB_API
)
```

This defines `MYLIB_INTERNAL` for the compilation of `MyLib` and `MYLIB_API` for both `MyLib` and any targets that link against it.

### 5. `target_compile_options()`

Specifies additional compiler options for a target.

```sh
add_executable(MyApp main.cpp)
target_compile_options(MyApp
    PRIVATE
        -Wall
        -Wextra
)
```

This adds the `-Wall` and `-Wextra` compiler flags when compiling MyApp.

### 6. `add_dependencies()`

Specifies that a target depends on other targets, ensuring the dependent targets are built first.

```sh
add_executable(MyApp main.cpp)
add_custom_target(generate_code
    COMMAND ${CMAKE_COMMAND} -E echo "Generating code..."
)
add_dependencies(MyApp generate_code)
```

Here, `MyApp` depends on generate_code, so the generate_code target will be built before MyApp.