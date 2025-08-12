# CMake Introduction

## World Without CMake

Managing C++ projects without CMake leads to several challenges:

- **Platform-Specific Build Scripts**: Separate scripts like `Makefile`, `.vcxproj`, or `Xcode` files must be manually written and maintained for each platform, creating duplication and inconsistency.
- **Manual Dependency Management**: Handling include paths, library linking, and dependencies becomes error-prone.
- **Scaling Issues**: Larger projects with multiple modules and cross-platform support become unmanageable, with teams wasting time replicating complex setups.
- **Inconsistent Builds**: Small differences in environments or configurations can lead to "works on my machine" problems.
- **Lack of Standardization**: No unified way to enforce modern C++ standards, abstract compiler details, or automate build steps.

Without CMake, developers spend more time fighting the build system than writing code.

### Manual Build Example Without CMake

Let’s consider a simple project that consists of a main program and a library:

Project structure: 

```yml
/MyProject
├── src
│   ├── main.cpp
│   ├── MyLib.cpp
├── include
│   └── MyLib.h
```

`main.cpp`:

```c++
#include "MyLib.h"
#include <iostream>

int main() {
    std::cout << "Result: " << add(5, 3) << std::endl;
    return 0;
}
```

`MyLib.cpp:`

```c++
#include "MyLib.h"

int add(int a, int b) {
    return a + b;
}
```

`MyLib.h`:

```c++
#ifndef MYLIB_H
#define MYLIB_H

int add(int a, int b);

#endif
```

#### Step 1: Compile the Library

```sh
g++ -c src/MyLib.cpp -o build/MyLib.o -Iinclude
```

`-c`: Compile without linking.
`-o`: Output file.
`-Iinclude`: Adds the include directory to the compiler's search path for headers.

#### Step 2: Compile the Main Program

This step compiles main.cpp into an object file (main.o) but does not link it with the library yet.

```sh
g++ -c src/main.cpp -o build/main.o -Iinclude
```

#### Step 3: Link Everything

In this step, all compiled object files (`main.o` and `MyLib.o`) are linked together to produce the final executable (`MyApp`).

```sh
g++ build/main.o build/MyLib.o -o build/MyApp
```

### Conclusion

As the project grows, the above approach becomes unmanageable:

- You'd need to compile and link dozens of files manually.
- Dependencies would need careful tracking to ensure correct linkage.
- Compiler and linker flags might vary across platforms.


This is where CMake shines: it automates and abstracts this process, making builds consistent, scalable, and platform-independent. Instead of managing everything manually, you focus on defining what to build and let CMake handle the rest!

## What is CMake?

CMake is a powerful, cross-platform build system (generator and builder) designed to simplify complex build workflows for C++ projects. Unlike traditional build systems, CMake does not directly build code but generates platform-specific build scripts (e.g., Makefiles, Ninja files, Visual Studio solutions). Its flexibility and scalability make it the de facto standard for modern C++ development.

### Key Features: 

- **Multi-Platform Support**: Generate build files for Linux, Windows, macOS, and embedded platforms with a single configuration.
- **Target-Based Design**: Focuses on modern C++ principles by managing dependencies and properties at the target level, promoting modularity and reusability.
- **Extensibility**: Custom commands, macros, and modules allow for deep customization of the build process.

## Key Stages in the CMake Workflow

As mentioned, CMake acts as an orchestrator that coordinates the build process by generating and managing the necessary instructions that external tools need for creating our executable. This workflow consists of three main stages: Configuration, Generation, and Build stage:

### Configuration Stage

This stage sets the foundation for the entire build process. During configuration, CMake examines the project’s directory structure, called the **source tree**, and prepares an output directory, referred to as the **build tree**.

#### Key Features:

- **Environment Analysis**: CMake inspects the build environment, identifying available compilers, linkers, and toolchains. For example, it ensures that a minimal test program can compile successfully.

- **Project Configuration Parsing**: The `CMakeLists.txt` file, which contains the project's targets, dependencies, and build rules, is executed. This defines how the project is structured and what components need to be built.

- **Cache Management**: A `CMakeCache.txt` file is created or updated, storing details such as compiler paths and configuration variables. This caching mechanism speeds up subsequent builds.

Command to configure your cmake project:

```sh
cmake -S <source_dir> -B <build_dir>
```

- `-S` specifies the source directory.
- `-B` specifies the output directory for generated files.

Example:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

### Generation Stage

In the generation stage, CMake transforms the collected information from the configuration stage into files that can be used by a **buildsystem** that is used by the project. This buildsystem can take the form of Makefiles, Ninja files, or IDE project files such as Visual Studio solutions.

The generation stage runs immediately after configuration. In many workflows, the two stages are combined and referred to collectively as "configuring the buildsystem".
#### Platform-Specific Outputs:

- **On Linux**: Generates Makefile or Ninja build scripts.
- **On Windows**: Generates Visual Studio project/solution files (.vcxproj and .sln).
- **On macOS**: Supports Xcode project generation.

### Build Stage

In the final stage, the buildsystem files generated by CMake is used to produce the requested artifacts, such as executables or libraries. The actual work is carried out by the build tool (e.g., make, ninja, or an IDE), not CMake itself.

Command to build your cmake project:

```sh
cmake --build <build_dir> --target <target> --config <config>
```

Example:

```sh
cmake --build build --target MyApp --config Release
```

## CMake Example:

Project Folder Structure:

```yml
/MyProject
├── CMakeLists.txt
├── include
│   └── MyLib
│       └── MyLib.h
├── src
│   ├── main.cpp
│   ├── MyLib.cpp
│   ├── MyLibHelper.cpp
└── build (created after configuration)
```

`CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.31)
project(MyProject 
    VERSION 1.0 
    LANGUAGES CXX)

# set compiler configurations
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_VERSION 19.1.5)

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

### Explanation

- **Library**: `MyLib` is defined as a static library with two .cpp files.
- **Include Directory**: The include directory contains the public header for the library.
- **Main Application**: The `MyApp` executable is linked to the `MyLib` library.
- **Modularity**: The library and executable are organized into src and include folders, following common C++ project conventions.