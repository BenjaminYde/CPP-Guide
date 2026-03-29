# Compilers

## GCC (GNU Compiler Collection)

### History and Reason for Existence

GCC was started in the mid-1980s by Richard Stallman and the Free Software Foundation as part of the GNU project. Its original goal was to provide a free (both in cost and licensing terms) alternative to proprietary compilers, fostering software freedom and enabling the growth of open-source operating systems—most notably Linux. As the project evolved, it became a compiler collection, capable of handling multiple languages, with `g++` as the C++ front-end.

### A Note on `gcc` vs `g++`

- **`gcc`**: The C compiler front-end. When given a `.c` file, it compiles as C. When given a `.cpp` file, it will compile as C++ but will **not** automatically link the C++ standard library (`libstdc++`).
- **`g++`**: The C++ compiler front-end. It treats both `.c` and `.cpp` files as C++ and automatically links the C++ standard library.

In practice: always use `g++` for C++ code. Using `gcc` on C++ files will compile them but produce linker errors about missing standard library symbols unless you manually add `-lstdc++`.

```bash
# This fails at link time (missing libstdc++):
$ gcc main.cpp -o main

# This works:
$ g++ main.cpp -o main

# This also works (manually linking libstdc++):
$ gcc main.cpp -lstdc++ -o main
```

The same distinction applies to Clang: use `clang++` for C++ code, not `clang`.

### Core Philosophy

- **Freedom and Openness**: Developed under the GNU Project, GCC remains under the GPL, emphasizing free software principles.
- **Portability and Ubiquity**: Widely available on many architectures, it's the default compiler on most Linux distributions.
- **Sustained Community Development**: Benefits from a large community of contributors and continuous updates.

### Why Developers Love It

- **Mature Optimization Pipeline**: Decades of development have led to robust and effective optimizations.
- **Extensive Target Support**: Supports a vast range of architectures (x86, ARM, RISC-V, MIPS, PowerPC, etc.). For niche or embedded targets, GCC is often the only available toolchain.
- **Stable and Widely Used**: The Linux kernel, glibc, and the majority of Linux distribution packages are compiled with GCC, making it the de facto reference compiler for that ecosystem.
- **Comprehensive Documentation and Community**: Extensive resources and a strong community provide extensive support.

### Drawbacks

- **Compilation Speed**: Can be slower than Clang in some scenarios, especially with certain optimization levels or heavily templated codebases. However, this gap has narrowed significantly with GCC 13+ and the difference is workload-dependent — GCC is competitive or faster on some projects.
- **Diagnostics Quality**: Historically poorer diagnostics compared to Clang. GCC 13 and 14 have made significant improvements (including fix-it hints and better error context), but Clang's diagnostics are still generally considered more precise and developer-friendly.
- **Less Modular Internal Design**: Its monolithic architecture can make it harder to integrate into custom tooling compared to LLVM's modular design. This can be a challenge when developing new tools that need to interact directly with the compiler's internal processes.

### Best Uses

- **Building Linux/UNIX Software**: Most packages in Linux distributions are compiled with GCC, making it the "reference" compiler in that ecosystem.
- **Embedded and Cross-Compilation**: When targeting a non-standard or niche hardware platform, GCC is often the first toolchain available.
- **Compute-Heavy Workloads**: For numerical simulations, scientific computing, and similar workloads where runtime performance of the generated code is the top priority, GCC's optimization pipeline can sometimes produce faster binaries.

## MSVC (Microsoft Visual C++)

### History and Reason for Existence

MSVC emerged as the primary compiler for the Windows ecosystem. Microsoft needed a robust, well-integrated compiler to support its operating system, frameworks and the growing number of Windows applications. MSVC's tight integration with the Visual Studio IDE and the Windows API made it the natural choice for professional Windows application and driver developers.

### Core Philosophy

- **Integration with the Microsoft Ecosystem**: Closely tied to Windows APIs and the Visual Studio IDE.
- **Enterprise-Grade Stability and Backward Compatibility**: Maintains stable ABIs and ensures backward compatibility for long-lived projects.
- **Modernization Over Time**: Significant investments since Visual Studio 2015 to improve standards conformance and optimization.

### Why Developers Love It

- **Excellent Windows Integration**: MSVC plus Visual Studio offers top-notch tooling, debugging, and profiling on Windows. The Visual Studio debugger, in particular, is widely regarded as one of the best native code debuggers available.
- **Professional Support and Reliability**: Backed by Microsoft with predictable updates and official support.
- **Strong Standards Compliance**: As of MSVC 17.10+ (2024/2025), MSVC has full C++20 support and strong C++23 support (both core language and library). It is effectively at parity with GCC and Clang for shipped standards. The historical perception of MSVC lagging behind is no longer accurate for C++20 and C++23.
- **Stable ABI**: MSVC maintains a stable C++ ABI within a major Visual Studio version (e.g., all VS 2022 updates are ABI-compatible).

### Drawbacks

- **Windows-Centric Nature**: MSVC primarily targets Windows. While Visual Studio can build for other platforms (e.g., via WSL or MSVC for Linux), MSVC itself is not as platform-agnostic as GCC or Clang.
- **Different ABI from GCC/Clang**: MSVC uses the Microsoft C++ ABI, which is fundamentally different from the Itanium C++ ABI used by GCC and Clang on Linux/macOS. This means you cannot link MSVC-compiled object files with GCC/Clang-compiled object files — they use different name mangling, vtable layouts, exception handling mechanisms, and calling conventions. Cross-platform projects must be compiled separately for each toolchain.
- **Less Flexible Outside Visual Studio**: While command-line use is possible (via `cl.exe` and the Developer Command Prompt), MSVC's ecosystem thrives in Visual Studio.

### Best Uses

- **Windows Application and Driver Development**: If you're building native Windows applications, MSVC plus Visual Studio is often the most straightforward path.
- **Enterprise and ISV Environments**: Large organizations with strong ties to the Windows stack benefit from MSVC's professional support and predictable release cycles.
- **Game Development on Windows**: Major game engines (Unreal Engine, many in-house engines) use MSVC as their primary Windows compiler, and the Visual Studio debugger is heavily relied upon in game development workflows.

## Clang/LLVM

### History and Reason for Existence

LLVM started in the early 2000s at the University of Illinois as a modern, modular compiler framework. Apple later championed Clang (the C/C++/Objective-C front-end for LLVM) to replace GCC, creating a permissively licensed, tool-friendly compiler. Over time, Clang has become a favorite for developers seeking clear error messages and a flexible, high-performance compiler.

### Core Philosophy

- **Modularity and Extensibility**: LLVM is designed as a set of reusable components, and Clang leverages this architecture. This makes it possible to build custom tools (linters, analyzers, refactoring tools) on top of the same infrastructure.
- **High-Quality Diagnostics and Tooling**: Well-regarded for its error messages, static analyzers, and integration with tools like `clang-tidy` and `clang-format`.
- **Innovation and Rapid Evolution**: The LLVM project continually evolves, adding new optimizations, backends, and language support.

### Why Developers Love It

- **Clarity and Precision**: Clang's error and warning messages are renowned for their clarity and precision. They often pinpoint the exact location of an error and provide helpful context, making it easier to understand and fix problems.
  - **Fix-It Hints**: In many cases, Clang suggests possible fixes for errors, saving you time and effort.
  - **Impact**: Faster debugging, easier code comprehension, and a more pleasant development experience, especially for large and complex codebases.
- **Improved Code Quality Through Static Analysis**:
  - Clang has a powerful built-in static analyzer that can detect potential bugs like memory leaks, null pointer dereferences, and other common errors before we even run the code. This will lead to higher code quality and fewer runtime issues.
  - Clang is part of the LLVM ecosystem, which gives us access to a wide range of powerful tools for code analysis, optimization, and transformation.
- **Good Compilation Speed**:
  - Clang tends to compile faster than GCC for template-heavy codebases and large projects, though the difference is workload-dependent and has narrowed over time. For some projects GCC is competitive or faster. Profile your own build if compile time matters.
  - **Impact**: Reduced build times lead to faster development cycles and improved developer productivity.
- **Cross-Platform Support**: Clang is widely used on macOS, Linux, BSDs, and has growing support on Windows via `clang-cl`. On macOS, Clang is the default (and only supported) compiler.
- **Rapid Adoption of New C++ Standards**:
  - **Active Development**: Clang is under very active development, and the developers often prioritize implementing new C++ features and standards quickly.
  - **Impact**: If you want to use the latest and greatest C++ features, Clang is often the first compiler to provide full support.

### Drawbacks

- **Aggressive Warning Flags**: Clang's `-Weverything` enables literally every warning the compiler supports. Combined with `-Werror` (treat warnings as errors), this frequently breaks builds of third-party code that compiles cleanly with GCC. This is a common pain point when building dependencies from source — as seen with projects like NVIDIA PhysX, where the combination of `-Werror -Weverything` on modern Clang versions produces dozens of harmless warnings promoted to hard errors.
- **Legacy Codebases**: Very old projects built and tested with GCC might be safer to stick with GCC. Some projects rely on GCC-specific extensions or behavior.
- **Specific Platform/Architecture Support**: GCC has better support for some niche or embedded platforms/architectures not yet well-supported by Clang (though Clang's target coverage continues to grow).

### Best Uses

- **Modern C++ Development**: If you want the best developer experience, with clear diagnostics and powerful refactoring tools, Clang is often the top choice.
- **Cross-Platform Tooling**: Clang is widely used on Linux, macOS, and now Windows (via `clang-cl`), providing a common toolchain across different operating systems.
- **Tooling Development and Research**: Because of LLVM's modular design, researchers and advanced developers often pick Clang/LLVM for building language tools, analyzers, and domain-specific languages.
- **macOS/iOS Development**: Clang is the only supported compiler for Apple platforms. If you're targeting macOS, iOS, or related platforms, Clang is mandatory.

## Standard Libraries: `libstdc++` vs `libc++`

The compiler is only half the story. The **C++ standard library implementation** you link against matters just as much for ABI compatibility. There are two major implementations on Linux and macOS:

### `libstdc++` (GNU)

- The standard library that ships with GCC.
- On Linux, both GCC and Clang default to linking against `libstdc++`.
- The most widely used C++ standard library on Linux. Virtually all Linux distribution packages are built against it.

### `libc++` (LLVM)

- The standard library developed as part of the LLVM project.
- The default on macOS (Apple Clang always uses `libc++`).
- On Linux, Clang *can* use `libc++` but doesn't by default — you must opt in with `-stdlib=libc++`.
- Has a different ABI from `libstdc++`. The internal layout of `std::string`, `std::vector`, and other containers is different.

### Why This Matters

`libstdc++` and `libc++` are **ABI-incompatible**. You cannot pass a `std::string` created by code compiled with `libstdc++` to code compiled with `libc++` — the internal representation is completely different, and doing so will cause crashes or data corruption.

This means:

- If library A is built with `clang++ -stdlib=libc++` and your app is built with `g++` (which uses `libstdc++`), and library A's API exposes `std::string`, **this will break at runtime**.
- On macOS, everything uses `libc++`, so this is not usually a problem. The issue primarily arises on Linux when someone explicitly opts into `libc++` for one part of the build but not others.

**Practical rule**: On Linux, use `libstdc++` for everything unless you have a specific reason to use `libc++` and can ensure your entire dependency chain is also built with `libc++`.

## Choosing a Compiler: Practical Guidance

### When to use GCC

- You're building for Linux and want maximum compatibility with the ecosystem (most distro packages are GCC-compiled).
- You're targeting embedded or niche architectures where GCC is the only available toolchain.
- You're building the Linux kernel or contributing to projects that mandate GCC.

### When to use Clang

- You want the best developer experience (diagnostics, tooling, fast iteration).
- You're building cross-platform software and want a single compiler on Linux, macOS, and Windows (`clang-cl`).
- You're building tooling or infrastructure on top of the compiler itself (LLVM's modularity).
- You're developing on macOS (Clang is the only option).

### When to use MSVC

- You're targeting Windows exclusively or primarily.
- You're developing games or applications deeply integrated with the Windows ecosystem.
- You want the Visual Studio debugger and profiling tools.
- Your project dependencies are distributed as pre-built Windows binaries (these are almost always MSVC-compiled).

### Mixing Compilers

On Linux, GCC and Clang are generally ABI-compatible because they both implement the Itanium C++ ABI and both default to `libstdc++`. However, mixing compilers safely depends on what crosses the API boundary between components. See the [ABI guide](abi.md) and the "Mixing Compilers at Link Time" section in the [compilation process guide](compilation_process.md) for detailed rules, examples, and a decision matrix.

The short version:

- **Safe to mix**: Libraries with C-style APIs (PhysX, zlib, SQLite, etc.)
- **Not safe to mix**: Libraries that expose STL types (`std::string`, `std::vector`, etc.) in their public API (OpenUSD, Qt, Boost, etc.) — these must be built with the same compiler as your application.
- **Never mix**: `libstdc++` and `libc++` in the same binary. Pick one for the entire project.