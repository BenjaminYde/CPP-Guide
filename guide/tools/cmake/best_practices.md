# CMake Best Practices

## Out-of-Source Builds

Keep the source tree clean by building in a separate directory (makes it easier to clean up builds):

```sh
cmake -S . -B build
```

## Pin the Minimum CMake Version

Specify the minimum required version in `CMakeLists.txt` to ensure compatibility:

```cmake
cmake_minimum_required(VERSION 3.31)
```

## Use Targets for Everything

Avoid manual inclusion of files or libraries; use targets instead. For example:

```cmake
target_include_directories(MyTarget PRIVATE include/)
```

Commands like `include_directories()` apply to all targets and can introduce unintended dependencies. Always scope settings to specific targets.


## Separate Build Configurations

Always separate Debug and Release builds to avoid conflicts:

```sh
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
```