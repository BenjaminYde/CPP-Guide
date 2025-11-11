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
target_include_directories(MyApp PRIVATE include)

# add library
add_library(MyLib 
    src/MyLib.cpp)

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

```cmake
add_executable(MyApp main.cpp)
```

### 2. Library Targets

Created using `add_library()`.

For example, create a library target, which can be static, shared, or module:

```cmake
add_library(MyLib STATIC lib.cpp)
```

### 3. Custom Targets

Defines a target that performs custom commands, which is always considered out-of-date and can be used to run arbitrary build steps.

Created using `add_custom_target()` for custom build actions.

```cmake
add_custom_target(run_tests
    COMMAND ${CMAKE_COMMAND} -E echo "Running tests..."
    COMMAND ${CMAKE_COMMAND} -E touch run_tests.stamp
    COMMENT "Executing custom target: run_tests"
)
```

## Target Commands (overview)

### `set_target_properties()`

Instead of setting global properties for all targets, you can set properties for the targets instead:

Global:

```cmake
# It affects all targets below it, and doesn't propagate.
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DMY_DEFINE)
include_directories(include)

add_executable(App1 "app1.cpp") # Gets C++17, -DMY_DEFINE, include
add_executable(App2 "app2.cpp") # Also gets C++17, -DMY_DEFINE, include
```

Target based:

```cmake
# app 1
add_executable(App1 "app1.cpp")
set_target_properties(App1 PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)
target_compile_definitions(App1 PRIVATE "MY_DEFINE")
target_include_directories(App1 PRIVATE include)

# app 2
add_executable(App2 "app2.cpp") # Is completely unaffected by App1's properties
```

### `get_target_properties()`

Retrieves the value of a property from a target and stores it in a CMake variable.

```cmake
add_library(MyLib lib.cpp)
set_target_properties(MyLib PROPERTIES CXX_STANDARD 17)

# ... later in the script ...
get_target_property(MyLibStandard MyLib CXX_STANDARD)
message(STATUS "MyLib C++ Standard is: ${MyLibStandard}")
```

You can also set multiple properties at once:

```cmake
add_library(MyLib lib.cpp)
set_target_properties(MyLib PROPERTIES
    OUTPUT_NAME "MyAwesomeLib"
    CXX_STANDARD 17
    POSITION_INDEPENDENT_CODE TRUE
)
```

### `target_sources()`

Associates source files with a target, allowing for dynamic source file specification.

```cmake
add_library(MyLib STATIC)
target_sources(MyLib
    PRIVATE
        lib.cpp
    PUBLIC
        lib.h
)
```

### `target_include_directories()`

Sets include directories for a target, specifying where the compiler should look for header files.

```cmake
add_library(MyLib STATIC lib.cpp)
target_include_directories(MyLib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)
```

This command specifies that the compiler should search for headers in `${CMAKE_SOURCE_DIR}/include` when compiling `MyLib` and in `${CMAKE_SOURCE_DIR}/src` only for the target itself.

### `target_link_libraries()`

Specifies libraries that a target depends on.

```cmake
add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE MyLib)
```

### `target_link_directories()`

Adds directories where the linker should search for libraries (e.g., using the `-L` flag).

```cmake
add_executable(MyApp main.cpp)
target_link_directories(MyApp
    PRIVATE
        /opt/custom/lib
)
target_link_libraries(MyApp PRIVATE some_custom_lib)
```

This tells the linker to search in `/opt/custom/lib` to find `libsome_custom_lib.a` (or `.so`). 

> [!Warning]
> In the example above you see there there is no target for the custom library so it is not really a modern approach. Modern practice prefers linking to IMPORTED targets, which already know their own paths. See the example below.

```cmake
add_executable(MyApp main.cpp)

# For example searching for something like "/opt/custom/lib/libsome_custom_lib.so"
find_library(
    CUSTOM_LIB_LIBRARY     
    NAMES "some_custom_lib"
    PATHS /opt/custom/lib
)

add_library(Custom::Lib SHARED IMPORTED)

set_target_properties(Custom::Lib PROPERTIES
    IMPORTED_LOCATION "${CUSTOM_LIB_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "/opt/custom/include"
)

target_link_libraries(MyApp PRIVATE Custom::Lib)
```

### `target_compile_definitions()`

Adds preprocessor definitions to a target, which can be used to conditionally compile code.

```cmake
add_library(MyLib STATIC lib.cpp)
target_compile_definitions(MyLib
    PRIVATE
        MYLIB_INTERNAL=1
    PUBLIC
        MYLIB_API
)
```

This defines `MYLIB_INTERNAL` for the compilation of `MyLib` and `MYLIB_API` for both `MyLib` and any targets that link against it.

### `target_compile_options()`

Specifies additional compiler options for a target.

```cmake
add_executable(MyApp main.cpp)
target_compile_options(MyApp
    PRIVATE
        -Wall
        -Wextra
)
```

This adds the `-Wall` and `-Wextra` compiler flags when compiling MyApp.        
Find more docs here about compiler flags for Clang:
- https://clang.llvm.org/docs/ClangCommandLineReference.html
- https://clang.llvm.org/docs/DiagnosticsReference.html

You can also do this: `man clang++` or `clang++ --help`​

### `add_dependencies()`

Specifies that a target depends on other targets, ensuring the dependent targets are built first.

```cmake
add_executable(MyApp main.cpp)
add_custom_target(generate_code
    COMMAND ${CMAKE_COMMAND} -E echo "Generating code..."
)
add_dependencies(MyApp generate_code)
```

Here, `MyApp` depends on generate_code, so the generate_code target will be built before `MyApp`.