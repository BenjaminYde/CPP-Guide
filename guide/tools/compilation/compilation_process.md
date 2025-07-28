# The Compilation Process

## Overview

Compiling C++ code means translating human-readable source code into an executable program (or a library). This process involves several steps, each with its own purpose and tools. The process can be broken down into the following stages:

### 1. Coding 

The journey starts with writing C++ source files (`.cpp`) and their corresponding headers (`.h`). These files contain the core logic and structure of the program. The code you write directly influences the machine instructions generated later in the process, shaping the behavior of the final executable.

### 2. Preprocessing

Before the actual compilation begins, the preprocessor steps in to handle directives like `#include` and `#define`. It essentially expands the source code by inserting the contents of included files directly into the `.cpp` file. The result of this step is a fully expanded source file, often referred to as a **translation unit**, typically saved with a `.i` or `.ii` extension.The output is modified source code, free of preprocessor directives,

### 3. Compiling

Each translation unit is then fed into the compiler, where it is transformed into **assembly code**. Assembly is a low-level, human-readable representation of the program's instructions. During this stage, the compiler applies various optimizations to enhance performance and reduce resource usage. The output is an assembly file, commonly using extensions like `.s` or `.asm`.

### 4. Assembling

In the assembly phase, the human-readable assembly code is converted into **machine code** — a series of binary instructions that the CPU can directly execute. The resulting binary data is stored in an **object file**, which typically has an `.o` or `.obj` extension, depending on the platform and compiler. At this point, the file is no longer readable by humans.

### 5. Linking

The final step is linking, where the various object files produced during the assembly stage are combined. The linker resolves references between different translation units, ensuring that symbols and functions used across files are correctly connected. This process produces either an executable file or a dynamic library, ready to be run or loaded by other programs.

> [!CAUTION]
> TODO: Add image

## Preprocessing

Preprocessing is a crucial first stage in the compilation process that prepares your source code for translation into machine code. It involves executing preprocessor directives, which allow conditional compilation, macro definitions, file inclusions, and more.

### Key Tasks

#### 1. File Inclusion 

File inclusion involves inserting the contents of specified header files into your source file before compilation begins. This is typically achieved using the `#include` directive. This is crucial for:

- **Code Reusability**: Sharing declarations (classes, functions, constants) across multiple source files.
- **Modularity**: Organizing code into logical units (headers for interfaces, source files for implementations).
- **Standard Library Access**: Using pre-written code from the C++ Standard Library.


```c++
#include <iostream> // Includes the iostream standard library header
#include "my_header.h" // Includes a user-defined header
```

- `#include <header>`: Searches for header in the system include directories. Used for standard library headers (e.g., <iostream>, <vector>).
- `#include "header"`: First searches in the directory containing the current file, then in the system include directories. Used for user-defined headers.

#### 2. Macro Expansion

Macro expansion replaces macro names with their corresponding defined values throughout the source code. This allows for easier code modification and improved readability.

Object-like Macros:

```c++
#define PI 3.14159
#define MAX_SIZE 1000
```
Every occurrence of `PI` gets replaced with `3.14159`.

Function-like Macros:

```c++
#define SQUARE(x) ((x) * (x))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
```

This can be used like:

```c++
int area = PI * SQUARE(5); // Expands to: int area = 3.14159 * ((5) * (5));
```

#### 3. Conditional Compilation

Conditional compilation directives (`#ifdef`, `#ifndef`, `#if`, `#elif`, `#else`, `#endif`) allow you to selectively include or exclude blocks of code during preprocessing. This is incredibly useful for:

This is incredibly useful for:

- **Platform-Specific Code**: Adapting code for different operating systems or architectures.
- **Debugging/Tracing**: Enabling or disabling debug-specific code sections.
- **Feature Flags**: Including or excluding optional features at compile time.
- **Header Guards**: Protecting against multiple inclusions of the same file.

For example, the following only compiles if `DEBUG` is defined:

```c++
#ifdef DEBUG
std::cout << "Debug mode is ON\n";
#endif
```

For example, when working with platform-specific code:

```c++
#ifdef _WIN32
    // Windows-specific code
#elif defined(__linux__)
    // Linux-specific code
#else
    // Fallback code
#endif
```

Common preprocessor directives:

- `#include`: Incorporates the contents of a file.
- `#define`: Defines macros for constants or functions.
- `#undef`: Undefines a macro.
- `#ifdef`, #ifndef, #endif: Allow conditional compilation based on whether macros are defined.
- `#pragma`: Direct compiler-specific behaviors, such as warnings or optimizations​

#### 4. Comment & Whitespace Removal

While often overlooked, the preprocessor's removal of comments and extraneous whitespace has a subtle but important impact. This step reduces the overall size of the preprocessed file and eliminates any variability in compilation behavior that might be caused by differing commenting styles or white space in header files. This streamlined file is then passed on to the compiler, ensuring that only the essential code is processed in subsequent compilation phases.

### Preprocessor Challenges

- **Excessive Macro Usage**:  Can lead to:
  - **Obscure Code**: Difficult to read and debug due to unexpected text substitutions.
  - **Side Effects**: Function-like macros might evaluate arguments multiple times, leading to unintended behavior.
  - **Namespace Pollution**: Macros don't respect scope, potentially leading to naming conflicts.
- **Header File Dependencies**:
  - **Circular Dependencies**: Header files that include each other, leading to compilation errors.
  - **Increased Compilation Times**: Unnecessary inclusions can significantly slow down builds.
- **Mismanaged Conditional Compilation**:
  - **Inconsistent Builds**: Different build configurations might produce subtly different executables.
  - **Dead Code**: Code that is never included in any build, increasing maintenance burden.

### Best Practices

#### 1. Use Header Guards

Use header include guards or `#pragma once` to prevent multiple inclusions of the same header file:

```c++
#ifndef HEADER_NAME_H
#define HEADER_NAME_H

// Header contents

#endif
```

Alternatively, use `#pragma once` if supported by your compiler:

```c++
#pragma once
```

#### 2. Static Analysis Tools:

- **cppcheck**: Helps detect unused functions, potential macro issues, and other common C++ errors.
- **include-what-you-use**: Analyzes header file dependencies and suggests improvements for faster builds.

#### 3. Minimize Header File Dependencies:

- **Forward Declarations**: Declare classes or structs without defining them to break dependency cycles.
- **Pimpl Idiom (Pointer to Implementation)**: Hide implementation details in source files to reduce header file dependencies.

#### 4. Favor `const`, `constexpr`, and `inline` over Macros:

- Replace function-like macros with `constexpr` or inline functions to improve type safety and code readability:

```c++
constexpr double PI = 3.14159; // Instead of #define PI 3.14159
inline int square(int x) { return x * x; } // Instead of #define SQUARE(x) ((x) * (x))
```

## Compiling

The compilation stage is the heart of the C++ translation process, where your preprocessed source code undergoes a metamorphosis, ultimately being transformed into assembly language—a low-level representation specific to the target architecture. The compiler parses each preprocessed translation unit independently during this stage, before they are finally linked.

### 1. Lexical Analysis (Tokenization): Breaking Down the Code into its Atomic Units

The compiler's first task is lexical analysis, often referred to as tokenization. Here, the stream of characters from the preprocessed source file is dissected into a sequence of tokens. These tokens are the fundamental building blocks of the language—the smallest meaningful units that the compiler can work with.

#### Token Categories

- **Keywords**: Reserved words with special meanings (e.g., `int`, `class`, `if`, `for`, `while`, `return`).
- **Identifiers**: Names given to variables, functions, classes, etc. (e.g., `myVariable`, `calculateArea`, `User`).
- **Literals**: Represent fixed values (e.g., `42`, `3.14f`, `"Hello, world!"`, `true`).
- **Operators**: Symbols that perform operations (e.g., `+`, `-`, `*`, `/`, `=`, `==`, `<`,`>`).
- **Punctuators**: Structural elements (e.g., `;`, `{}`, `()`, `,`).

#### Example

```c++
int result = calculateSum(a, 5);
```

is broken down into the following tokens: `int`, `result`, `=`, `calculateSum`, `(`, `a`, `,`,` 5`, `)`, `;`

Tokenization transforms the source code into a structured stream that the parser can readily process. It's akin to breaking down a sentence into individual words before understanding its grammatical structure.

### 2. Syntax Analysis (Parsing): Building the Abstract Syntax Tree (AST)

Syntax analysis, or parsing, is where the compiler verifies that the sequence of tokens conforms to the grammatical rules of the C++ language. The parser constructs an **Abstract Syntax Tree (AST)** — a hierarchical, tree-like representation of the code's structure.

#### Key Concepts

- **Grammar**: A set of rules defining the valid syntax of C++. The parser uses this grammar to validate the token sequence.
- **AST**: The tree structure represents the code's syntactic relationships. Each node in the tree corresponds to a construct in the code (e.g., a variable declaration, an expression, a statement, a function definition).

For the code `int b = a + 5;`, the AST might look something like this (simplified):

```
   = (Assignment)
  / \
 b   + (Addition)
    / \
   a   5
```

- The AST captures the code's structure independently of its textual representation.
- It serves as the foundation for subsequent analysis and transformation phases.
- Syntax errors (e.g., missing semicolons, unbalanced parentheses) are detected during parsing, resulting in compiler errors.

If a piece of code is missing essential syntax elements, such as a semicolon, the compiler will raise a syntax error:

```
int b = a + 2
```

This will produce an error like: `g++: expected ';' at end of declaration`

### 3. Semantic Analysis: Giving Meaning to the Code

Semantic analysis is where the compiler delves into the meaning of the code. It goes beyond syntax to ensure that the code is semantically valid within the context of the C++ language rules.

#### Key Checks

- **Type Checking**: Verifies that operations are performed on compatible types (e.g., you can't add a string to an integer).
- **Scope Resolution**: Determines the visibility and lifetime of variables. It ensures that variables are declared before use and are accessed within their valid scope.
- **Function Call Validation**: Checks that functions are called with the correct number and types of arguments.
- **Overload Resolution**: If multiple functions have the same name (overloading), the compiler determines the appropriate function to call based on the argument types.

#### Example

```c++
std::string s = "hello";
int x = s + 5; // Error: Cannot add an integer to a string
```

### 4. Intermediate Code Generation: Bridging the Gap to Machine Code

After semantic analysis, the compiler creates an intermediate representation (IR) of the code. This IR is a platform-independent, lower-level representation that is closer to machine code but still abstract enough for optimization.

#### Characteristics of IR

- **Simplicity**: IR is typically simpler than C++ source code, with fewer constructs and a more uniform structure.
- **Abstraction**: It abstracts away from specific machine instructions, making it suitable for optimization across different architectures.

#### Example

The following C++ code:

```c++
class Rectangle {
public:
    int getArea() { return width_ * height_; }
private:
    int width_;
    int height_;
}
```

might be translated into a simplified, C-like IR (conceptual example):

```c
struct Rectangle {
    int width_;
    int height_;
};

int Rectangle_getArea(Rectangle* this) { 
    return this->width_ * this->height_; 
}
```

### 5. Optimization: Enhancing Performance and Efficiency

The optimization phase is where the compiler applies a wide array of techniques to improve the performance and efficiency of the generated code. Optimizations are typically performed on the IR.

#### Common Optimization Techniques

- **Constant Folding**: Evaluating constant expressions at compile time (e.g., int x = 5 * 3; becomes int x = 15;).
- **Dead Code Elimination**: Removing code that has no effect or is never reached.
- **Inlining**: Replacing function calls with the function's body (reduces call overhead but can increase code size).
- **Loop Optimizations**:
  - **Loop Unrolling**: Replicating the loop body to reduce loop overhead.
  - **Loop Invariant Code Motion**: Moving calculations that don't change within the loop outside the loop.
  - **Loop Fusion**: Combining adjacent loops into a single loop.
- **Register Allocation**: Assigning frequently used variables to CPU registers for faster access.
- **Instruction Scheduling**: Reordering instructions to improve pipeline utilization and reduce stalls.
- **Tail Call Optimization**: Replacing certain recursive calls with iterative code, avoiding stack growth.
- **Strength Reduction**: Replacing costly operations by less expensive ones (e.g. replacing some multiplications by bit shifts).

**Optimization Levels**: Compilers typically offer different optimization levels (e.g., `-O0`, `-O1`, `-O2`, `-O3` on GCC/Clang). Higher levels generally result in more aggressive optimization, potentially at the cost of longer compilation times.

#### Example

```c++
int sum = 0;
for (int i=0; i<1000; ++i) {
    sum += i;
}
```

An optimizing compiler might transform this into (conceptually):

```c++
int sum = 1000; // Result precalculated
```

**Trade-offs**: Optimization can significantly improve performance, but it can also increase compilation time and sometimes make debugging more difficult (as the optimized code might differ significantly from the source code).

### 6. Assembly Code Generation: The Final Translation

The last step in the compilation stage is the generation of assembly code. This is a low-level, human-readable representation of machine instructions specific to the target architecture (e.g., x86, ARM).

#### Key Aspects:

- **Target-Specific**: Assembly language is tied to a particular instruction set architecture (ISA).
- **One-to-One (Mostly)**: Each assembly instruction typically corresponds to a single machine instruction.
- **Assembler Input**: The generated assembly code is then fed to the assembler, which translates it into machine code (object files).

#### Example:

A simple C++ statement like `int c = a + b`; might be translated into x86 assembly code like this (simplified example):

```
movl a, %eax  ; Move the value of 'a' into register EAX
addl b, %eax  ; Add the value of 'b' to register EAX
movl %eax, c  ; Move the result from EAX to 'c'
```

## Assembling

The assembly stage is where the human-readable (though cryptic) assembly language, generated by the compiler, is translated into the raw binary language of the machine—machine code. This stage is handled by the **assembler**, a specialized tool that bridges the gap between the compiler's output and the executable instructions that the CPU can understand. Each compilation unit produces an **object file** containing machine code that will later be combined during linking to form the executable.

### Key Tasks of the Assembler

**Machine Code Generation**: The assembler's primary responsibility is to convert each assembly language instruction into its corresponding binary machine code representation. This involves:

- **Opcode Translation**: Replacing mnemonic opcodes (e.g., `mov`, `add`, `jmp`) with their numerical equivalents, which the CPU directly interprets as instructions.
- **Operand Encoding**: Converting operands (registers, memory addresses, immediate values) into the appropriate binary format as specified by the target architecture's instruction set.

#### Example:

Consider a simplified x86 assembly instruction:

```
mov eax, 10  ; Move the value 10 into the EAX register
```

The assembler might translate this into a machine code sequence like:

```
B8 0A 00 00 00
```

Where:

- `B8` is the opcode for moving an immediate value into the `EAX` register.
- `0A 00 00 00` is the immediate value 10 (in little-endian byte order).

### Object File Structure (Simplified View)

An object file typically contains the following sections:

- **.text**: Contains the machine code instructions.
- **.data**: Contains initialized global and static variables.
- **.bss**: Holds information about uninitialized global and static variables (space will be allocated for them during program loading).
- **.rodata**: Stores read-only data, such as string literals and constant values.
- **Symbol Table**: Lists the symbols defined and referenced in the object file.
- **Relocation Table**: Contains entries for addresses that need to be adjusted during linking.
- **Debug Information (Optional)**: Provides information that can be used by debuggers to map machine code back to the original source code.

### The Assembler's Role in the Bigger Picture

- **Independence from the Compiler**: The assembler is generally independent of the compiler that generated the assembly code. This allows for flexibility in the toolchain (you could potentially use different compilers and assemblers).
- **Target Architecture Specificity**: The assembler is inherently tied to a specific target architecture (e.g., x86, ARM, MIPS). The machine code it generates is only valid for that architecture.
- **Input to the Linker**: The object files produced by the assembler are the primary input for the linker, which combines them to create the final executable.

## Linking

Linking is the final step of the compilation process, where the object files generated by the assembler—are combined with libraries to create a self-contained, runnable executable (or library). The linker orchestrates the resolution of symbols, the merging of code and data, and the creation of the final executable file.

### Key Tasks of the Linker

#### 1. Symbol Resolution

This is the linker's most crucial task. It involves resolving all the external symbols that were left undefined during the assembly stage. The linker examines the symbol tables of each object file and library, matching references to external symbols with their corresponding definitions.

- **Matching Symbols**: For each unresolved symbol in an object file, the linker searches for a definition of that symbol in other object files or libraries.
- **Multiple Definitions (ODR Violation)**: If the linker finds multiple definitions for the same global symbol, it typically issues an error, as this violates the One Definition Rule (ODR) in C++. There are exceptions, such as weak symbols, that allow for intentional overriding of definitions.
- **Unresolved Symbols**: If the linker cannot find a definition for a symbol, it generates an "unresolved external symbol" error, halting the linking process. This commonly occurs when you forget to link a necessary library or if there's a typo in a function or variable name.

#### 2. Relocation

Once symbols are resolved, the linker performs relocation. This involves adjusting the addresses of code and data within each object file so that they can all reside together in the executable's address space without conflicts.

- **Address Adjustment**: The linker assigns a final memory address to each section (`.text`, `.data`, `.rodata`, etc.) from each object file. Then, it goes through the relocation tables, modifying the instructions and data that refer to addresses that have changed due to the merging of sections.
- **Relocation Types**: The linker handles different types of relocations, such as:
  - **Absolute Relocations**: The address is fixed at link time.
  - **Relative Relocations**: The address is calculated relative to a base address (often used for position-independent code).

#### 3. Library Linking

The linker incorporates code from libraries into the final executable. There are two main types of libraries:

- **Static Libraries** (`.a` on Linux/macOS, `.lib` on Windows): These libraries are essentially archives of object files. The linker extracts the necessary object files from the static library and includes them directly into the executable. This results in a larger executable but avoids runtime dependencies on external libraries.
- **Dynamic Libraries** (`.so` on Linux/macOS, `.dll` on Windows): These libraries are not directly incorporated into the executable. Instead, the linker adds information to the executable that allows the operating system's dynamic loader to find and load the dynamic library at runtime. This results in smaller executables and allows libraries to be shared between multiple programs, but it introduces a runtime dependency.

#### 4. Executable File Generation

After resolving symbols and performing relocation, the linker creates the final executable file (or dynamic library). It combines the modified sections from the object files, along with any necessary startup code, into a single file with a well-defined format (e.g., ELF, PE, Mach-O). This executable contains the machine code, data, and metadata needed for the operating system to load and run the program.

### Common Linker Errors

- **Unresolved External Symbols**: The most common linker error, indicating that the linker could not find a definition for a symbol referenced in your code. Causes include:
  - Forgetting to link a necessary library.
  - Typos in function or variable names.
  - Incorrect header file inclusion (leading to missing declarations).
- **Multiple Definitions**: Occurs when the linker finds more than one definition for the same global symbol. This usually points to a violation of the ODR.

## Notes

- gcc vs g++
- static vs dynamic libraries:
  - https://www.linkedin.com/pulse/differences-between-static-dynamic-libraries-nasser-abuchaibe/
  - see c++ compiling book page 53
  - when to use what
- what is inside object files? (see c++ compiling book page 26)
- talk about ABI
  - multiple compilers
  - cross platform

## Static vs Dynamic Libraries

Libraries are collections of pre-compiled code (object files) that can be reused across multiple programs. The primary distinction lies in when and how the library's code is incorporated into your final program.

### Static Libraries (`.a`, `.lib`)

A **static library** is an archive of object files. During the linking phase, all the required code from the static library (.a or .lib file) is copied directly into your final executable file. This creates a larger, but completely self-contained, program. Because the code is now part of the executable itself, the original library file is no longer needed at runtime.**
#### Pros:

- **Self-Contained Executable**: The executable has no external dependencies on the library, making distribution and deployment simpler. Just copy the executable, and it runs.
- **Potentially Faster Execution**: Since the code is part of the executable, it can sometimes be loaded faster at runtime and allow for more aggressive whole-program optimizations by the linker.

#### Cons:

- **Larger Executable Size**: Every program that uses the library gets its own copy of the code, leading to larger file sizes.
- **Difficult to Update**: If a bug is found in the library, every program that uses it must be re-linked and redistributed.

### Dynamic Libraries (`.so`, `.dll`)

A **dynamic library** (or shared library) is a separate file that is not copied into the executable at link time. Instead, the linker places a reference to the library in the executable. When the program is run, the operating system's dynamic loader finds the required library on the system and loads it into memory, where it can be shared among multiple running programs.

#### Pros:

- **Smaller Executable Size**: The executable is much smaller because it only contains references to the library, not the library code itself.
- **Shared Memory**: A single copy of the library in memory can be used by multiple programs, saving RAM.
- **Easier Updates**: To update the library, you can simply replace the .so or .dll file. All programs using it will benefit from the update on their next run without needing to be recompiled or re-linked (assuming the ABI remains compatible).

#### Cons:

- **External Dependency**: The program requires the dynamic library file to be present on the target system in a location the OS can find. This can lead to "DLL Hell" or dependency issues.
- **Slightly Slower Startup**: There is a small overhead at program launch while the dynamic loader locates and loads the necessary libraries.

### When to use which

| Scenario                                          | Recommended Choice | Rationale                                                                                             |
| :------------------------------------------------ | :----------------- | :---------------------------------------------------------------------------------------------------- |
| Distributing a simple, standalone application     | Static Library     | Creates a single, easy-to-deploy executable file with no external dependencies.                       |
| Developing a large system with many components    | Dynamic Library    | Allows modules to be updated independently and reduces overall memory footprint.                      |
| Creating a plugin system                          | Dynamic Library    | Plugins are a natural fit for dynamic loading at runtime.                                             |
| Working in a resource-constrained environment (disk space) | Dynamic Library    | Minimizes disk space by sharing common code.                                                          |
| Prioritizing maximum performance and link-time optimization | Static Library     | Allows the linker to perform optimizations across both the application and library code.              |

## References

### Books

- **Advanced C and C++ Compiling** (Paid)
  - Learning how to write C/C++ code is only the first step. To be a serious programmer, you need to understand the structure and purpose of the binary files produced by the compiler: object files, static libraries, shared libraries, and, of course, executables.
  - Link [here](https://www.amazon.com/Advanced-C-Compiling-Milan-Stevanovic/dp/1430266678)

### Links

- https://www.linkedin.com/pulse/c-compilation-steps-amit-nadiger/
- https://www.toptal.com/c-plus-plus/c-plus-plus-understanding-compilation
- https://subscription.packtpub.com/book/programming/9781789801491/1/ch01lvl1sec03/the-c-compilation-model
- https://app.studyraid.com/en/read/1708/24051/compilation-process
- https://mikelis.net/code-to-binary-an-in-depth-look-at-the-c-building-process/
- https://evolved-cpp.netlify.app/e-compiling-and-linking/01-the-compilation-process/#the-c-compilation-process
- https://elhacker.info/manuales/Lenguajes%20de%20Programacion/C++/Advanced%20C%20and%20C++%20Compiling%20(Apress,%202014).pdf