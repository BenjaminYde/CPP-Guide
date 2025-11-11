# CMake Variables

CMake variables are placeholders used to store information during the configuration process of a build system. They can represent paths, compiler flags, settings, or user-defined values. These variables enable dynamic and reusable configurations, allowing you to write flexible and robust build scripts.

### Key points

- Variables are case-sensitive (MY_VAR and My_Var are distinct).
- Variables can have different scopes, meaning their visibility can vary depending on where they are defined and used.
- CMake supports various types of variables for specific needs, including strings, lists, and booleans.

## Types of Variables

### 1. Standard Variables

Standard variables exist only during the current configuration process. They are transient and are not written to the `CMakeCache.txt` file unless explicitly marked as cache variables. These include:

#### Predefined variables

- Predefined variables provided by CMake (e.g., `CMAKE_CXX_STANDARD`, `CMAKE_BUILD_TYPE`).

Examples:

- `CMAKE_CXX_STANDARD`: Specifies the C++ standard (e.g., 17, 20).
- `CMAKE_CXX_STANDARD_REQUIRED`: This makes the standard you just set a strict requirement. If this is ON, CMake will stop with an error if the compiler doesn't support C++20. If you left this `OFF` (or didn't set it), CMake would try C++17, but if it wasn't available, it might silently fall back to an older standard (like C++11), which is usually not what you want.
- `CMAKE_CXX_EXTENSIONS`: This disables compiler-specific extensions to the C++ standard. Many compilers (like GCC and Clang) add their own non-standard features, which are enabled by default. Setting this to `OFF` ensures CMake uses the flag for the pure, official standard. This makes your code more portable and helps you avoid relying on non-standard features by mistake.
- `CMAKE_BUILD_TYPE`: Determines the build type (e.g., Debug, Release, RelWithDebInfo).
- `CMAKE_INSTALL_PREFIX`: Defines the directory where files will be installed.

Usage:

```cmake
# set compiler configurations
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_VERSION 20.1.8)

# enforce modern C++ standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

#### User-defined variables

User-defined variables used for internal logic or intermediate values.

These variables simplify project configuration, ensuring consistency across different platforms and build environments.

```cmake
set(MY_VAR "temporary_value")  # A standard variable
message(STATUS "MY_VAR: ${MY_VAR}")
```

### 2. Cache Variables

The CMake Cache is stored in the `CMakeCache.txt` file, which is automatically generated in the build directory during the configuration process. CMake uses this file to store a set of global cache variables, whose values persist across multiple runs within a project build tree. It records variables that CMake needs to configure and generate the build system files, such as:

- Paths to compilers and libraries.
- Feature flags.
- User-defined settings.

Example of entries in `CMakeCache.txt`:

```
CMAKE_C_COMPILER:FILEPATH=/usr/bin/clang
CMAKE_BUILD_TYPE:STRING=Release
ENABLE_FEATURE_X:BOOL=ON
```

#### Key features:

- Cache variables are created using the `CACHE` keyword in the `set(...)` command or or by passing them via the command line.
- They are saved in `CMakeCache.txt` and reused in subsequent configurations unless overridden or explicitly cleared.

#### Behavior:

- If a cache variable exists, its value is prioritized over a standard variable with the same name.
- Cache variables are visible in the `cmake-gui`.
- To update an existing cache variable, use the `FORCE` keyword.

```cmake
set(MY_CACHE_VAR "default_value" CACHE STRING "A persistent cache variable")
```

or

Command-line usage:

```sh
cmake -DMY_CACHE_VAR=custom_value ..
```

#### Avoid Repeated Command-Line Arguments:

- If you set a cache variable using the command line (e.g., `cmake -DVAR=value`), CMake saves this value in `CMakeCache.txt`.
- On subsequent runs, the saved value will automatically be used, and you don’t need to pass  the `-D` flag again.


Cache variables allow users to customize the build process without modifying the CMakeLists.txt file.

#### Decision Flow: Cache or Not?

Ask the following questions to decide whether a variable should be cached:

- Does the user need to customize this variable?
  - Yes → Cache it.
  - No → Use a standard variable.
- Should the variable persist across multiple runs?
  - Yes → Cache it.
  - No → Use a standard variable.
- Is this variable purely for internal logic within the script?
  - Yes → Use a standard variable.


###  3. Environment Variables

Environment variables are inherited from the system or shell environment. These are accessed in CMake using the $ENV{VAR_NAME} syntax.

```cmake
set(PATH_VAR $ENV{PATH})
message(STATUS "System PATH: ${PATH_VAR}")
```

Use cases:

- Passing system-level configuration into the build.
- Managing compiler or library paths that vary across machines.

## Variable Scopes

Understanding the scope of a variable is crucial for managing its visibility and avoiding unintended side effects.

### 1. Directory Scope

Variables set in a specific directory are only accessible within that directory and its subdirectories. They do not propagate to parent directories unless explicitly passed.

Example: 

`CMakeLists.txt`:
```cmake
set(DIR_VAR "Value for the parent directory")
add_subdirectory(subdir)
```

`subdir/CMakeLists.txt`:
```cmake
# Accessing the variable works here
message(STATUS "DIR_VAR in subdir: ${DIR_VAR}")

# Defining a local variable
set(SUBDIR_VAR "Value for subdirectory")

# This variable is local to this directory and subdirectories only
# So when changing the value, it will not propagate to the previous directory
```


### 2. Parent-Child Scope

Variables can be explicitly propagated from a child directory back to its parent using the `PARENT_SCOPE` modifier. This is useful for sharing values upward when subdirectories calculate or define values that need to influence the parent.

`CMakeLists.txt`:

```cmake
# Parent CMakeLists.txt
add_subdirectory(subdir)

# The value of SHARED_VAR is now visible here
message(STATUS "SHARED_VAR in parent: ${SHARED_VAR}")
```

`subdir/CMakeLists.txt`:
```cmake
# Define a variable and propagate it to the parent
set(SHARED_VAR "shared_value_from_child" PARENT_SCOPE)
```

### 3. Global Scope

Variables defined in the top-level `CMakeLists.txt` file (or explicitly marked as GLOBAL) are accessible throughout the entire project, including all subdirectories.

## Variable Types

### 1. Strings

Strings are the most common variable type in CMake. They store single-line text values and are used for paths, flags, names, or any other textual data. Strings are unquoted in CMake unless they contain special characters.

- CMake treats variables as strings by default.
- Double quotes are needed to preserve spaces or special characters.

Example:

```cmake
# Define a string variable
set(PROJECT_VERSION "1.2.3")
message(STATUS "Project version: ${PROJECT_VERSION}")

# String concatenation
set(FULL_PATH "${CMAKE_SOURCE_DIR}/include")
message(STATUS "Full path: ${FULL_PATH}")
```

Output:
```
Project version: 1.2.3
Full path: /path/to/source/include
```

### 2. Booleans

CMake treats certain keywords as boolean values to enable conditional logic. The values `ON`, `OFF`, `TRUE`, `FALSE`, `1`, and `0` are considered boolean.

Example:

```cmake
# Define a boolean variable
set(ENABLE_FEATURE ON)

# Check the value
if(ENABLE_FEATURE)
    message(STATUS "Feature is enabled.")
else()
    message(STATUS "Feature is disabled.")
endif()
```

Or via commandline:

```
cmake -DENABLE_FEATURE=ON ..
```

### 3. Paths

Paths are a specialized form of strings that represent file or directory locations. CMake uses paths extensively for compiler settings, installation locations, and dependency discovery.

Paths are represented as strings but often manipulated using built-in CMake commands like `file()` or `find_package()`.

Example:

```cmake
# Set the installation prefix
set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation directory")

# Print the installation path
message(STATUS "Install path: ${CMAKE_INSTALL_PREFIX}")
```

### 4. Lists

#### Defining a List with a Single Semicolon-Delimited String

If all elements of the list are known in advance, you can define the list as a single string with semicolon-separated values:

```cmake
set(myList "a;list;of;five;elements")
message(STATUS "myList: ${myList}")
```

- Here, myList contains 5 elements: `a`, `list`, `of`, `five`, `elements`.
- Semicolons (;) are the native delimiters for lists in CMake.

#### Defining a List Using Separate Arguments

You can also provide each list item as a separate argument to the `set()` command:

```cmake
set(myList a list of five elements)
message(STATUS "myList: ${myList}")
```

#### Mixing Arguments and Semicolon-Delimited Strings

You can mix single arguments and semicolon-delimited strings when defining a list:

```cmake
set(myList a list "of;five;elements")
message(STATUS "myList: ${myList}")
```

#### Common List Functions

Here are some common commands for working with lists in CMake, with short explanations and examples.

Create a List: 

```cmake
set(myList a list of elements)
set(myList "a;list;of;elements")
```

Append Items to a List:

```cmake
list(APPEND myList "new_item" "another_item")
message(STATUS "Updated list: ${myList}")
```

Insert Items to a List:

```cmake
list(INSERT myList 0 "first_item")
message(STATUS "Prepended list: ${myList}")
```

Get the Length of a List:

```cmake
list(LENGTH myList listLength)
message(STATUS "List length: ${listLength}")
```

Access List Elements by Index

```cmake
list(GET myList 0 firstElement)
message(STATUS "First element: ${firstElement}")
```

Remove Specific Items:

```cmake
list(REMOVE_ITEM myList "list" "elements")
message(STATUS "List after removal: ${myList}")
```

Find the Index of an Item:

```cmake
list(FIND myList "of" index)
message(STATUS "Index of 'of': ${index}")
```

Iterate Over a List:

```cmake
foreach(item IN LISTS myList)
    message(STATUS "Item: ${item}")
endforeach()
```