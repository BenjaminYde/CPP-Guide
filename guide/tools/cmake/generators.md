# CMake Generators

## Generator Types

Generators in CMake define the type of buildsystem CMake will produce, tailored to the tools and workflows you're using. They determine how CMake structures the files needed to compile and build your project.

Here some examples of generator commands:

- Makefiles: `cmake -G "Unix Makefiles"`
- Ninja: `cmake -G "Ninja"`
- Visual Studio: `cmake -G "Visual Studio 17 2022"`
- Xcode: `cmake -G "Xcode"`

You can find more generators [here](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#cmake-generators).

## Single vs. Multi-Config Generators

### Single-Config Generators

Single-config generators, like Makefiles or Ninja, allow only one build configuration (e.g., Debug or Release) per build directory. To change the configuration, you must reconfigure with `CMAKE_BUILD_TYPE`

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

## Multi-Config Generators

Multi-config generators, such as Visual Studio and Xcode, support multiple build configurations in a single build directory (e.g., `Debug`, `Release`, `RelWithDebInfo`). Specify the configuration during the build stage:

```sh
cmake --build build --config Debug
```

### Ninja Multi-Config

Starting with CMake 3.17, the Ninja generator supports multi-configuration builds through the Ninja Multi-Config generator. This functionality allows you to generate build files for multiple configurations (e.g., Debug, Release) simultaneously, similar to how Visual Studio and Xcode handle multi-config builds.

#### Key Features:

- **Multiple Build Configurations**: Generate separate `build-<Config>.ninja` files for each configuration specified in `CMAKE_CONFIGURATION_TYPES`. For example, specifying Debug and Release will create `build-Debug.ninja` and `build-Release.ninja` files.

- **Cross-Config Mode**: Enables building targets across different configurations. For instance, you can build a target in the Release configuration while using tools built in the Debug configuration. This is particularly useful for scenarios where you want to run a release-optimized version of a generator utility while still building the debug version of the targets built with the generated code. 

#### Usage:

```sh
cmake -G "Ninja Multi-Config" -S <source_dir> -B <build_dir>
```

```sh
cmake --build <build_dir> --config Release
```

See blogpost from Kitware [here](https://www.kitware.com/multi-config-ninja-generator-in-cmake-3-17/).
