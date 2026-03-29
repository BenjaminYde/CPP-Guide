# ABI (Application Binary Interface)

## What is an ABI?

The ABI is a set of low-level rules that govern how software programs are compiled and linked to interact with each other on a specific platform. It defines the interface between different compiled code units, ensuring that separately compiled modules can work together correctly. In simpler terms, the ABI is the blueprint that ensures binaries can communicate.

**Analogy**: Think of it as a detailed agreement between different parts of a machine, ensuring all gears and cogs mesh correctly, even if they don't know how the other is built internally. If two parts don't follow the same ABI, the machine won't function.

Unlike an API (Application Programming Interface), which specifies how code interacts at the source level, the ABI ensures compatibility at the machine code level.

### Why is ABI important?

- **Interoperability**: Allows code compiled with different compilers, or even different versions of the same compiler, to be linked together and run correctly.
- **Binary Compatibility**: A stable ABI means that you can update libraries without recompiling applications that depend on them (as long as the API remains compatible). This is crucial for operating systems, shared libraries, and large software ecosystems.
- **Language Interfacing**: Enables code written in different programming languages (like C++ and C) to interact.

**Everyday Example**: Imagine electrical outlets. Different countries have different standards (voltage, plug shape). An appliance built for one standard won't work with another without an adapter. Similarly, software built with incompatible ABIs won't "plug in" together.

## The Two Major C++ ABIs

Before diving into details, it's important to understand that there are two fundamentally different C++ ABIs in widespread use:

### Itanium C++ ABI (GCC and Clang on Linux/macOS/BSDs)

The Itanium C++ ABI is the de facto standard on all non-Windows platforms. Despite the name (which comes from its origin on the Intel Itanium architecture), it is used on x86, x86_64, ARM, RISC-V, and essentially every architecture on Linux, macOS, and BSDs. Both GCC and Clang implement this ABI, which is why they can produce binary-compatible object files.

The Itanium ABI specifies name mangling rules, vtable layout, exception handling mechanisms (using the "zero-cost" table-based approach), RTTI layout, and calling conventions.

### Microsoft C++ ABI (MSVC on Windows)

MSVC uses its own C++ ABI, which differs from the Itanium ABI in virtually every aspect — different name mangling, different vtable layout, different exception handling (SEH-based), and different calling conventions. You cannot link MSVC-compiled C++ object files with GCC/Clang-compiled object files. Cross-platform projects must be compiled separately for each platform.

> [!NOTE] 
> On Windows, `clang-cl` (Clang's MSVC-compatible mode) implements the Microsoft ABI, not the Itanium ABI. This means `clang-cl` output is compatible with MSVC, but not with regular `clang++` targeting Linux.

## When ABI Compatibility Matters Most

ABI compatibility becomes paramount in these situations:

#### 1. Shared Libraries (Dynamic Linking)

- When your application **dynamically links against shared libraries** (e.g., `.so` files on Linux, `.dylib` on macOS, `.dll` on Windows), ABI compatibility is essential. If the library's ABI changes in an incompatible way and your application is not recompiled, it might crash or behave unexpectedly when trying to use the updated library.
- **Operating System Libraries**: System libraries (like the C standard library, libc, libstdc++) evolve over time. Applications compiled against older versions of these libraries might not be compatible with newer versions if the ABI has changed.
- **Third-Party Libraries**: If your project relies on external libraries (e.g., for graphics, networking, databases), ABI compatibility is needed to ensure seamless integration without requiring recompilation each time the library updates.

#### 2. Plugin Architectures

- Applications that load plugins dynamically at runtime rely heavily on ABI stability. If the plugin interface (defined by the host application) changes incompatibly, existing plugins might break.
- **Example**: Houdini loads HDK plugins as `.so`/`.dll` files. If you build a Houdini plugin with a different compiler or different flags than SideFX used to build Houdini itself, the plugin will crash when Houdini loads it. The same applies to USD file format plugins loaded by OpenUSD.

#### 3. Language Interoperability

- When combining code written in different languages (e.g., calling C code from C++, or using Python extensions written in C), the ABIs of those languages must be compatible for functions to be called and data to be exchanged correctly.
- **Example**: `extern "C"` in C++ is crucial for ensuring that C++ code can correctly call C functions by disabling name mangling and adhering to C calling conventions.

#### 4. Object-Oriented Frameworks and Polymorphism

- When using object-oriented frameworks or libraries that rely on polymorphism (virtual functions, dynamic dispatch), ABI compatibility is critical to ensure that objects can be used correctly across different code modules.
- If the layout of a base class changes in an incompatible way, derived classes compiled against the old ABI may access memory incorrectly, leading to crashes or undefined behavior.
- **Example**: A UI framework might define a base `Widget` class with virtual methods. If the framework's ABI changes the `Widget` class's vtable layout, applications using older versions of the framework might experience crashes when interacting with widgets.

## Common Causes of ABI Incompatibilities

#### 1. Changes to Data Structures

- **Adding/Removing/Reordering Data Members**: Modifying the data members of a class or struct (especially if it's used in a public interface) alters its size and memory layout. This breaks compatibility with code compiled against the old definition.
- **Changing Data Member Types**: Even if the size remains the same, altering the type of a data member can break ABI compatibility if the new type has different alignment requirements or if it affects how the compiler generates code to access the member.
- **Virtual Table Layout Changes**: Modifying virtual functions can alter the vtable's structure, including:
  - Adding or removing virtual functions.
  - Changing the order of virtual functions.
  - Changing the inheritance hierarchy (adding/removing base classes with virtual functions).

#### 2. Changes to Functions

- **Altering Function Signatures**: Modifying a function's parameters (type, order, number), return type, or calling convention directly breaks compatibility, as the caller and callee will no longer agree on how to pass arguments and return values.
- **Name Mangling Changes**: In C++, changes that affect how function names are mangled (e.g., due to changes in compiler internals, namespaces, or template instantiations) can lead to linker errors or incorrect function calls.
- **Inlining Changes**: Modifying whether a function is inlined (e.g., due to compiler optimization changes or the use of inline keywords) can affect code that relies on the function having a specific address.

```c++
// Old library version:
void processData(int dataSize, const char* data);
```

```c++
// New library version:
void processData(size_t dataSize, const char* data);
```

- **ABI Problem**: The type of `dataSize` has changed from `int` to `size_t`. On 64-bit platforms, `int` is 4 bytes and `size_t` is 8 bytes. The caller and callee will disagree on how many bytes the argument occupies.
- **Result**: If your application (compiled with the old header) calls `processData` in the new library, it might pass the `dataSize` argument incorrectly. The library might read the wrong amount of data, leading to crashes or data corruption.

#### 3. Changes to Enumerations

- **Adding/Removing Enumerators**: Adding or removing enumerators in an enum (especially in C where enums are often used as integral types) can change the underlying numerical values of existing enumerators, breaking code that relies on those values.
- **Changing the Underlying Type**: Altering the underlying type of an `enum class` can impact its size and how it's passed in function calls.

```c++
// Old library version:
enum class ErrorCode {
    None,
    NotFound,
    PermissionDenied
};
```

```c++
// New library version:
enum class ErrorCode {
    None,
    NetworkError, // New!
    NotFound,
    PermissionDenied
};
```

- **ABI Problem**: The underlying numerical values of `NotFound` and `PermissionDenied` have changed, even though their names are the same. Your application, compiled with the old header, still thinks `NotFound` is 1, but in the new library, it's 2.
- **Result**: If your application receives an `ErrorCode` from the new library and compares it to the old values, it will misinterpret the error code. For instance, it might think a `NetworkError` is a `NotFound` error.
- **Safe alternative**: Always add new enumerators at the end, so existing values don't shift.

#### 4. Compiler and Toolchain Differences

- **Different Compiler Versions**: Different versions of the same compiler might make changes to their internal implementation, optimization strategies, or default settings that result in ABI incompatibilities.
- **Different Compilers**: GCC and Clang use the Itanium ABI on Linux and are generally compatible (see "Mixing GCC and Clang" below). MSVC uses a completely different ABI — mixing MSVC with GCC/Clang is never possible at the C++ level.
- **Different Standard Library Implementations**: Using different C++ standard library implementations (`libstdc++` vs `libc++`) leads to ABI issues because their internal data structure layouts differ (see "Mixing GCC and Clang" below).

#### 5. Changes to Templates

- Modifying template definitions can lead to ABI breaks because the compiler generates different code for different template instantiations.
- Changes to the template code itself or the way it's used can result in different mangled names or altered function behavior.

## Real-World Examples of ABI Problems

### 1. The `std::string` Incompatibility in GCC

- **Background**: Historically, `libstdc++` (the GNU C++ Standard Library) used a reference-counted implementation for `std::string`. To comply with the C++11 standard, this was changed to a "short-string optimization" (SSO) strategy. This seemingly internal change drastically altered the size and memory layout of `std::string` objects.
- **Scenario**: Imagine you have a C++ application that dynamically links against a shared library. The library developers update their library, recompiling it with a newer version of GCC that uses the new `std::string` implementation. Your application, however, was compiled with an older GCC and the older `std::string` definition.
- **Problem**: Your application expects `std::string` to have a certain size and memory layout based on the older GCC's implementation. When your application interacts with the updated library, it passes and receives `std::string` objects that are fundamentally different in their internal structure.
- **Result**:
  - **Crash (Segmentation Fault)**: If your application passes an old-style `std::string` to a function in the new library, the library, expecting a new-style `std::string`, might try to access memory beyond the bounds of the string, leading to a crash.
  - **Data Corruption**: The library might write data to what it thinks is part of the string's internal storage, but because of the layout mismatch, it ends up overwriting other data in your application's memory, leading to subtle and hard-to-debug errors.
  - **Linker Error**: In a best-case scenario, if the linker is sophisticated enough, it might detect the ABI mismatch between your application and the library and produce an error, preventing you from running the incompatible program.
- **Solution**: GCC introduced a dual ABI mechanism to mitigate this problem. Developers can choose between the old and new `std::string` implementations using the `_GLIBCXX_USE_CXX11_ABI` macro. This allows older code to remain compatible with the old ABI while newer code can use the C++11 compliant `std::string`.

You can detect which ABI a library uses by inspecting its symbols:

```bash
$ nm -C libmylibrary.so | grep basic_string | head -3

# New ABI (=1): symbols contain std::__cxx11::basic_string
# Old ABI (=0): symbols contain std::basic_string (without __cxx11)
```

### 2. Linux Shared Library Versioning

- **Background**: Linux systems use shared libraries extensively.
- **Problem**: If a library updates and breaks ABI compatibility, applications linked against the older version might stop working.
- **Solution**: Linux employs a system of shared library versioning (using SONAMEs). Libraries are named with a version number (e.g., `libmylibrary.so.1`). Applications are linked against a specific version. Multiple versions of the same library can coexist, allowing older applications to continue working while newer applications use the updated library.

### 3. Different Compiler Optimizations

- **Scenario**: You have a library that you provide to your users in compiled form. You decide to upgrade your compiler to take advantage of new optimization features. You recompile your library with the new compiler, using more aggressive optimization settings.
- **ABI Problem**: Even without changes to your code, the new compiler might make different decisions about:
  - **Inlining**: It might inline functions that the old compiler didn't, or vice-versa.
  - **Register Allocation**: It might use different registers to pass arguments or store temporary values.
  - **Struct Padding**: It might add or remove padding between struct members differently.
- **Result**: Client code that was compiled with the older version of your library and compiler may experience:
  - **Linker Errors**: If a function that used to be non-inlined is now inlined by the new compiler, the linker won't find the function's definition.
  - **Crashes or subtle bugs**: If calling conventions or struct layouts are different, the client code may pass data incorrectly or access memory in an invalid way.

This example shows that ABI compatibility is not just about your source code, but also about the specific tools and settings used to compile it.

## Strategies for Mitigating ABI Issues

### 1. Design for ABI Stability

- **Minimize Public Interfaces**: Reduce your API's surface area. Fewer public elements mean fewer potential points of ABI breakage.
- **Opaque Pointers (Pimpl Idiom)**: Hide implementation details. This allows changes to the implementation without affecting the public class's size or layout, preserving ABI.
- **Abstract Base Classes (Interfaces)**: Decouple interfaces from implementations using pure virtual functions. Implementations can change without altering the interface's ABI.
- **Favor Composition over Inheritance**: Using composition can reduce coupling between classes compared to inheritance.
- **Avoid Inlining in Public Headers**: Changes to inlined functions in public headers force recompilation of dependent code, potentially breaking ABI.
- **Use Standard Layout Types**: When interfacing with C or when memory layout predictability is crucial, use standard layout types.
- **Use C-Style APIs for Maximum Portability**: If your library will be consumed by code compiled with different compilers, expose a C API (`extern "C"`) with opaque pointers. This is what PhysX, SQLite, zlib, and most cross-compiler-safe libraries do.

### 2. Employ Versioning Strategically

- **Semantic Versioning (SemVer)**: Use a major.minor.patch versioning scheme. Increment the major version for ABI-breaking changes.
- **Symbol Versioning**: (Primarily on Linux) Use linker version scripts to manage exported symbols and allow multiple library versions to coexist.
- **Inline Namespaces (C++)**: Wrap different API versions in inline namespaces to prevent accidental linking with incompatible versions.

#### Example: Versioning with Inline Namespaces

```c++
// mylibrary.h — version 2
namespace mylibrary {
    inline namespace v2 {
        class MyClass { /* ... v2 implementation ... */ };
    }
}

// Consumer code just writes:
mylibrary::MyClass obj;
// The compiler actually resolves this to mylibrary::v2::MyClass
// which produces a different mangled symbol than mylibrary::v1::MyClass
// So if someone links against the wrong version, they get a linker error
// instead of a silent ABI mismatch
```

This is how `libstdc++` implements the dual ABI — the new `std::string` lives in `std::__cxx11::basic_string` while the old one lives in `std::basic_string`, and `inline namespace __cxx11` controls which one you get.

### 3. Document and Communicate

- **ABI Stability Policy**: Clearly document your ABI stability guarantees and versioning policy.
- **Migration Guides**: Provide detailed guides for users when introducing ABI-breaking changes.

### 4. Verify with Tools

Use ABI checking tools to catch breaking changes before they reach users:

**`libabigail`** (recommended — actively maintained by Red Hat, used by Fedora/RHEL):

```bash
$ sudo apt install abigail-tools    # Ubuntu/Debian

# Compare two versions of a library for ABI breaks
$ abidiff libfoo_v1.so libfoo_v2.so

# Check if your app is compatible with a specific library version
$ abicompat my_app libfoo.so

# Dump the full ABI of a library for inspection/archival
$ abidw libfoo.so > libfoo.abi.xml
```

## Data Type Representation

Object layout refers to how the data members of a class or struct are arranged in memory. The C++ standard doesn't dictate the precise layout (except for standard-layout types), leaving it up to the compiler and the ABI.

- **Size and Alignment**: Specifies the size (in bytes) and memory alignment requirements of fundamental data types (e.g., `int`, `float`, `char`, `pointers`).
- **Example**: An ABI might define `int` as 4 bytes, `long long` as 8 bytes, and require pointers to be aligned on 8-byte boundaries on a 64-bit system.
  - Different compilers may have different padding strategies for structs.

```c++
struct MyStruct {
    char a;
    int b;
};
// Layout may differ:
// 5 bytes vs 8 bytes due to padding/alignment.
// - a char typically takes up 1 byte of memory.
// - an int typically takes up 4 bytes of memory.
```

To align `b` properly in memory (often to a 4-byte boundary), the compiler inserts 3 bytes of padding after `a` as follows: `a (1 byte) + Padding (3 bytes) + b (4 bytes)`

Another similar example:

```c++
#include <iostream>
#include <cstddef>

struct MyStruct {
    char a;     // 1 byte
    int b;      // 4 bytes (likely 3 bytes of padding after 'a')
    double c;   // 8 bytes
};

int main() {
    std::cout << "Size of MyStruct: " << sizeof(MyStruct) << std::endl; // Likely 16
    std::cout << "Offset of a: " << offsetof(MyStruct, a) << std::endl; // 0
    std::cout << "Offset of b: " << offsetof(MyStruct, b) << std::endl; // 4
    std::cout << "Offset of c: " << offsetof(MyStruct, c) << std::endl; // 8
    return 0;
}
```

### Consequences of Padding

- **Increased Memory Usage**: Padding leads to some memory being "wasted" as it is not used to store data directly.
- **Improved Performance**: The primary reason for padding is performance. Aligning data according to processor requirements allows for faster memory access. The processor can often fetch properly aligned data in a single memory operation.
  - Accessing misaligned data can be significantly slower on some architectures, or even cause crashes.
- **Platform/Compiler Dependent**: The exact amount of padding added can vary depending on the:
  - Compiler
  - Target architecture (e.g., 32-bit vs. 64-bit)
  - Operating system
  - Compiler settings

### Data Model Differences Across Platforms

Even when using the same ABI family, the size of fundamental types can differ between platforms:

| Type | Linux x86_64 (LP64) | Windows x64 (LLP64) |
|:---|:---|:---|
| `int` | 4 bytes | 4 bytes |
| `long` | **8 bytes** | **4 bytes** |
| `long long` | 8 bytes | 8 bytes |
| `size_t` | 8 bytes | 8 bytes |
| `pointer` | 8 bytes | 8 bytes |

This is why cross-platform code should avoid `long` in public APIs and prefer fixed-width types like `int32_t`, `int64_t`, or `size_t` instead.

## Function Calling Conventions

Function calling conventions are fundamental to how functions in compiled code communicate with each other. A mismatch in calling conventions between a caller and a callee will lead to undefined behavior, crashes, or subtle data corruption. They specify the precise mechanisms for:

- **Argument Passing**: Defines how function arguments are passed to a function (registers, stack, or a combination).
- **Return Value**: Determines how the function's return value is passed back to the caller.
- **Stack Management**: How the call stack is managed during function calls (who is responsible for allocating and deallocating stack space).
- **Register Usage**: Which CPU registers are used for what purpose during a function call, and who is responsible for preserving their values.
- **Real World Example**: If you're calling a C library from C++, the calling convention is crucial to make sure your C++ code and the C library are communicating arguments and return values the same way.

These conventions are specific to a particular architecture (e.g., x86-64), operating system, and sometimes even the compiler.

### Why They Matter: Enabling Interoperability and Correctness

Without a shared understanding of these rules, separately compiled code modules would be unable to communicate effectively. Calling conventions ensure:

- **Interoperability**: Code compiled by different compilers (or different versions of the same compiler) can be linked together successfully, enabling the use of libraries and the creation of modular software.
- **Correctness**: Proper argument passing, return value handling, and stack management prevent data corruption, unexpected behavior, and crashes.
- **Language Interfacing**: They are the bridge that allows code written in different programming languages (like C++ and C) to interact seamlessly.

### Key Components and Practical Implications

#### 1. Argument Passing:

- **Interfacing with C Libraries**: When calling C functions from C++, `extern "C"` is crucial. It tells the C++ compiler to use C-style linkage (preventing name mangling) and adhere to the C calling conventions, ensuring that arguments are passed in the manner expected by the C code.
- **Performance**: Understanding which arguments are likely to be passed in registers can guide you in designing functions that minimize data movement. For instance, prefer passing small, frequently used data structures in registers instead of large ones.

C code:
```c
// C header (c_library.h)
typedef struct {
    int id;
    double value;
} Data;

void processData(Data* data, int count);
```

C++ code:
```c++
// C++ code
extern "C" {
    #include "c_library.h"
}

int main() {
    Data items[10];
    // ... initialize items ...
    processData(items, 10); // Pass a pointer to the array and its size
    return 0;
}
```

> **Note**: The `extern "C"` should wrap the `#include` in the C++ code, not appear in the C header itself. C headers should remain valid C.

#### 2. Return Value

Small return values (like integers or pointers) are typically returned in a designated register (`RAX` on x86-64). Larger values might involve the stack or a hidden pointer mechanism.

In practice this means:

- **Debugging**: When a function returns an incorrect value, knowing the designated return register allows you to inspect its contents in a debugger and trace the problem back to its source.
- **Performance**: Returning very large objects by value can be inefficient. Consider returning a pointer or utilizing move semantics to avoid unnecessary copying.

## Name Mangling

Name mangling is a technique employed by C++ compilers to **encode function and variable names into unique symbols** within the compiled object files (and consequently, in libraries and executables). 

This encoding process transforms human-readable names from your source code into linker-friendly symbols that are guaranteed to be unique, even in the presence of features like overloading, namespaces, and templates. It is also applied to class, struct, enum names, etc.

### Why is it Necessary?

C++ supports features that allow multiple entities to share the same name in the source code:

- **Function Overloading**: Defining multiple functions with the same name but different parameter lists.
- **Namespaces**: Organizing code into logical groups to prevent naming collisions.
- **Templates**: Creating generic functions and classes that can operate on different types.
- **Classes**: Methods within a class can have the same name as functions outside the class.

Without name mangling, the linker (the tool that combines object files) would be unable to distinguish between these identically named entities, leading to errors or incorrect program behavior.

### How it Works: The Encoding Process

The specific rules for name mangling (the "mangling scheme") are part of the ABI and can vary between compilers and platforms. However, the general idea is to encode the following information into the mangled symbol:

- **Name**: The original function or variable name.
- **Namespace**: The namespace(s) the entity belongs to (if any).
- **Class**: If it's a member function, the class it belongs to.
- **Function Parameters**: The data types of the function's parameters (for overloaded functions).
- **Return Type**: In some cases the return type is also included in the mangled name (but not always, as it is not part of the overload resolution in C++).
- **Template Arguments**: The specific types used to instantiate a template (if applicable).
- **const/volatile qualifiers**: Whether a member function is const or not.

### Example: Itanium ABI

```c++
namespace MyStuff {
    class MyClass {
        public:
            int doSomething(double d, const char* str) const;
    };
}
```

Using the Itanium ABI, the mangled name for `doSomething` might look something like this:

```
_ZNK7MyStuff7MyClass11doSomethingEdPKc
```

Let's break it down (roughly):

- **_Z**: A prefix indicating a mangled name.
- **NK**: N = nested name, K = const member function.
- **7MyStuff**: The namespace `MyStuff` (length-prefixed).
- **7MyClass**: The class `MyClass` (length-prefixed).
- **11doSomething**: The function name `doSomething` (length-prefixed).
- **E**: Indicates the end of the nested name.
- **d**: Encodes the `double` parameter.
- **PKc**: Encodes the `const char*` parameter (P for pointer, K for const, c for char).

Both GCC and Clang use the Itanium mangling scheme on Linux, which is why their object files are link-compatible. MSVC uses a completely different mangling scheme (e.g., `?doSomething@MyClass@MyStuff@@QEBANPEBD@Z`).

### Real-World Implications and Practical Benefits

#### 1. Resolving Linker Errors:

- **Problem**: You encounter linker errors like `"undefined reference to ..."` or `"multiple definition of ..."`.
- **Name Mangling's Role**: These errors often arise from name mangling mismatches. For example, if you declare a function in a header file but define it differently in a source file (e.g., different parameter types), the mangled names won't match, and the linker won't be able to find the correct definition. Similarly, different compilers, or even different versions of the same compiler, might use different mangling schemes, leading to problems when linking object files they generated.
- **Solution**:
  - **`c++filt`**: Use a demangler tool to translate mangled names back into human-readable form. This helps you identify the source of the mismatch.
  - **`nm`**: This utility can list the symbols in an object file including their mangled names. This helps in finding issues.
  - **`extern "C"`**: For interfacing with C code or libraries, use `extern "C"` to prevent C++ name mangling and ensure compatibility.
  - **Compiler Options**: Ensure that all your project's object files and libraries are compiled with compatible compiler options, especially regarding the standard library and any optimization flags that could affect the ABI.

```bash
# Demangle a symbol
$ echo "_ZNK7MyStuff7MyClass11doSomethingEdPKc" | c++filt
MyStuff::MyClass::doSomething(double, char const*) const

# List all symbols in a library, demangled
$ nm -DC libmylibrary.so

# Search for a specific symbol
$ nm -DC libmylibrary.so | grep doSomething
```

#### 2. Library Compatibility:

- **Problem**: You're using a pre-compiled library, and you encounter linker errors or runtime crashes.
- **Name Mangling's Role**: The library might have been compiled with a different compiler or different compiler settings than your project, leading to incompatible name mangling schemes.
- **Solution**:
  - **Recompile the Library**: If you have the source code, recompile the library using the same compiler and settings as your project.
  - **Use Compatible Libraries**: Ensure that the libraries you use are built with compatible tools and are designed for the same ABI as your project.

### Example: Linker Error Due to Name Mangling

**file: myclass.h**

```c++
class MyClass {
public:
    void myFunction(int a);
};
```

**file: myclass.cpp**

```c++
#include "myclass.h"

void MyClass::myFunction(double a) { // Error! Parameter type mismatch
    // ... implementation ...
}
```

**Error**: When you try to compile and link this code, you'll likely get a linker error because `myFunction` expects the version that takes an `int`, but the implementation in `myclass.cpp` is for a version that takes a `double`. The mangled names will be different, and the linker won't be able to find the correct match.

## Exception Handling

Exception handling is an ABI concern because the mechanism for throwing, propagating, and catching exceptions must be agreed upon by all code in the same process. On Linux (Itanium ABI), C++ uses "zero-cost" table-based exception handling — there is no runtime overhead when exceptions are not thrown, but the compiler generates tables that the runtime uses to unwind the stack when an exception occurs.

### ABI Relevance

- **Exception tables and personality routines**: The format of these tables is part of the ABI. GCC and Clang both implement the same Itanium exception handling ABI on Linux, so exceptions can propagate across GCC-compiled and Clang-compiled code within the same binary.
- **MSVC uses a different mechanism**: MSVC uses Structured Exception Handling (SEH) on Windows, which is incompatible with the Itanium mechanism. This is another reason you cannot mix MSVC and GCC/Clang object files.

### The `-fno-exceptions` Problem

Some libraries and projects disable exceptions entirely with `-fno-exceptions` for performance or code size reasons. This creates an ABI boundary:

- If code compiled with `-fno-exceptions` calls code that throws, the program will call `std::terminate` instead of unwinding the stack.
- This is safe if exceptions never cross the boundary. Many C-style API libraries (like PhysX) disable exceptions internally but never throw across their API.
- It becomes a problem when exceptions are expected to propagate through a call chain that includes code compiled with `-fno-exceptions`.

**Practical rule**: If a library you depend on disables exceptions, check whether its API can throw. If it has a C-style API with error codes, mixing is safe. If it has a C++ API that documents throwing behavior, both sides must agree on whether exceptions are enabled.

## Runtime Type Information (RTTI)

RTTI is the mechanism that allows `dynamic_cast` and `typeid` to work at runtime. The compiler generates `typeinfo` structures for polymorphic types (classes with virtual functions), and the format of these structures is part of the ABI.

### ABI Relevance

- **`typeinfo` layout**: The Itanium ABI specifies how `typeinfo` objects are laid out, including how type names are stored and how inheritance relationships are encoded. GCC and Clang agree on this layout.
- **`dynamic_cast` across boundaries**: For `dynamic_cast` to work correctly, the `typeinfo` for the target type must be visible and compatible across all code in the process. This generally works between GCC and Clang on Linux because they use the same RTTI format.

### The `-fno-rtti` Problem

Some performance-critical codebases disable RTTI with `-fno-rtti` (notably LLVM itself, some game engines, and some NVIDIA libraries). This creates issues:

- **Missing `typeinfo` symbols**: If library A is compiled with `-fno-rtti`, it won't emit `typeinfo` for its classes. If your code (compiled with RTTI enabled) tries to `dynamic_cast` to one of those classes, you'll get a linker error about missing `typeinfo` symbols.
- **Mixing is safe if RTTI never crosses the boundary**: If the library has a C-style API or never exposes polymorphic types that you'd need to `dynamic_cast`, disabling RTTI in the library is harmless.

**Practical rule**: Same as exceptions — check what crosses the API boundary. If you never `dynamic_cast` to types defined in the library, the RTTI mismatch doesn't matter.

## Mixing GCC and Clang on Linux

This is the question that comes up most often in practice: can you safely link GCC-compiled and Clang-compiled code together?

**Short answer**: Yes, to a large extent, because both implement the Itanium C++ ABI on Linux. But "ABI compatible" means more than just the calling convention. All of the following must hold.

### 1. Same standard library implementation

Both sides must link against the same standard library — either both use `libstdc++` or both use `libc++`. You cannot mix them. A `std::string` in `libstdc++` and a `std::string` in `libc++` are completely different types at the binary level, even though they have the same API.

On Linux, both GCC and Clang default to `libstdc++`, so this is usually fine unless someone explicitly passes `-stdlib=libc++` to Clang. Be explicit in your build system:

```bash
-stdlib=libstdc++    # the Linux default, but don't rely on defaults
```

### 2. Same `_GLIBCXX_USE_CXX11_ABI` setting

When both sides use `libstdc++`, they must agree on the ABI version. GCC 5 introduced a dual ABI for `std::string` and `std::list` to comply with C++11. The default on modern GCC/Clang is `_GLIBCXX_USE_CXX11_ABI=1` (the new ABI). If one side was built with `=0` (the old ABI) and the other with `=1`, any `std::string` or `std::list` passed across the boundary will be silently misinterpreted — different memory layout, different size.

Be explicit:

```bash
-D_GLIBCXX_USE_CXX11_ABI=1
```

You can detect which ABI a pre-built library uses:

```bash
$ nm -C libsomething.so | grep basic_string | head -3
# __cxx11::basic_string → new ABI (=1)
# basic_string without __cxx11 → old ABI (=0)
```

### 3. Compatible exception handling

If one side is compiled with `-fno-exceptions` and the other side throws a C++ exception that propagates across the boundary, the program will `std::terminate`. This comes up in practice with libraries like PhysX and LLVM, which disable exceptions for performance.

If the library has a C-style API, the mismatch doesn't matter — exceptions won't cross the boundary. If it has a C++ API that throws, both sides must agree.

### 4. Compatible RTTI settings

If one side is compiled with `-fno-rtti` and the other uses `dynamic_cast` or `typeid` on types defined in that code, you get linker errors (missing `typeinfo` symbols) or undefined behavior at runtime. Game engines and performance-critical libraries often disable RTTI.

Same rule as exceptions: if the library has a C-style API or you never `dynamic_cast` to its types, the mismatch doesn't matter.

### 5. Same C++ standard version for ABI-affecting types

The `std::string` change (point 2) is the most well-known example. More subtly, the layout of types like `std::optional`, `std::variant`, and `std::any` can differ between `-std=c++17` and later standards in some `libstdc++` versions. If these types cross the API boundary, both sides should use the same `-std=` flag.

### 6. No struct packing or alignment mismatches

Flags like `-fpack-struct`, `#pragma pack`, or platform-specific alignment attributes change the memory layout of structs. If one side packs a struct to 1-byte alignment and the other uses the default, any struct passed across the boundary will be read with wrong offsets. This is rare but devastating when it happens.

### What matters most in practice

Points 1 and 2 are the ones that actually bite people. Points 3 and 4 only matter if one side explicitly disables exceptions or RTTI (uncommon in general-purpose code, common in game engines and some NVIDIA libraries). Points 5 and 6 are edge cases that matter when debugging mysterious corruption.

**If all of the above hold**, then STL types (`std::string`, `std::vector`, etc.) **are safe** to pass across the boundary. The STL layout is identical between GCC and Clang when they use the same `libstdc++` with the same settings.

### Practical checklist

**Lock down ABI flags in your build system.** Don't rely on compiler defaults — they can change between versions:

```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=1)
```

**Libraries with C-style or POD-only APIs can safely be built with a different compiler.** PhysX, zlib, OpenEXR's C API, SQLite — the API boundary only uses plain types, so points 1–5 don't apply.

**Libraries that expose STL types must be built with the same toolchain.** If OpenUSD is built with Clang, build your app and any USD-dependent plugins with Clang too. Same for Qt, Boost, or anything whose headers expose `std::string`, `std::vector`, `std::function`, etc.

**When in doubt, check the library's public headers.** If you see STL containers in function signatures, return types, or class definitions that cross the library boundary — that library must match your compiler, standard library, ABI flag, and `-std=` version.

**Verify pre-built libraries before linking.** When you receive pre-built binaries:

```bash
# What compiler built it?
$ readelf -p .comment libsomething.so

# What standard library does it link against?
$ ldd libsomething.so | grep -E "stdc\+\+|c\+\+"

# What ABI does it use for std::string?
$ nm -C libsomething.so | grep basic_string | head -5
```

If any of these don't match your build environment and the library exposes STL types, rebuild from source or match your settings to the library's.

**For maximum safety, build everything from source with one toolchain.** Projects like OpenUSD's `build_usd.py` do exactly this — they build every dependency (TBB, Boost, OpenSubdiv, etc.) from source with the same compiler, same flags, same standard library. It takes longer but eliminates all ABI concerns.

## References

- [Wikipedia: Application binary interface](https://en.wikipedia.org/wiki/Application_binary_interface)
- [Itanium C++ ABI specification](https://itanium-cxx-abi.github.io/cxx-abi/abi.html)
- [libabigail — ABI verification tools](https://sourceware.org/libabigail/)
- [CppCon 2019: Louis Dionne "The C++ ABI From the Ground Up"](https://www.youtube.com/watch?v=DZ93lP1I7wU)
- [SwedenCpp: Arvid Norberg: Evolving the C++ ABI](https://www.youtube.com/watch?v=Uy_Ls-jQbR4)
- [CppCon: What is an ABI, and Why is Breaking it Bad? - Marshall Clow - CppCon 2020](https://www.youtube.com/watch?v=7RoTDjLLXJQ&t=103s)