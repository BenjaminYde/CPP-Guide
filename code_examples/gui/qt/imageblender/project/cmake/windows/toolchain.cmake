set(CMAKE_SYSTEM_NAME Windows)

# --- 1. Set Clang as the Compiler ---
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# --- 2. Tell Clang to Target Windows (MinGW) ---
# This is the magic flag that makes Clang use the MinGW headers/libs
set(CMAKE_C_COMPILER_TARGET x86_64-w64-mingw32)
set(CMAKE_CXX_COMPILER_TARGET x86_64-w64-mingw32)

# --- 3. Resource Compiler ---
# We use MinGW's windres to compile .rc files (icons, metadata)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# --- 4. Paths and Qt Setup ---
# Default to 6.8.0 if not passed via command line
if(NOT DEFINED QT_VERSION)
    set(QT_VERSION "6.8.0")
endif()

# Tell CMake where to look for libraries (MinGW sysroot + Qt)
set(CMAKE_FIND_ROOT_PATH 
    /usr/x86_64-w64-mingw32 
    /opt/qt/${QT_VERSION}/mingw_64
)

# Cross-compilation standard settings
# (Don't run Windows programs found in these paths, only link libraries)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Help CMake find Qt modules
set(QT_DIR /opt/qt/${QT_VERSION}/mingw_64/lib/cmake/Qt6)
set(Qt6_DIR /opt/qt/${QT_VERSION}/mingw_64/lib/cmake/Qt6)