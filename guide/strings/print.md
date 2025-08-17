# `std::print`: Formatted Output (C++23)

Introduced in C++23, `std::print` and its companion `std::println` are in the `<print>` header and provide a modern, type-safe, and high-performance mechanism for formatted output, designed to replace the legacy `printf` family and the often-verbose `std::cout`.

## Core Concepts

At its heart, `std::print` is a function that writes formatted text directly to standard output. It leverages the formatting engine from C++20's `<format>` library.

### Pro's

- **Compile-Time Safety**: The format string is parsed at compile time. If the type or number of arguments doesn't match the format specifiers, you get a compiler error, not a runtime crash or garbage output.

- **High Performance**: It is faster than `std::cout` as it avoids the overhead of iostreams' complex architecture. It's designed to be a highly efficient, low-level I/O utility.

- **Clean and Readable Syntax**: The Python-inspired syntax using `{}` placeholders is more intuitive and less error-prone than printf's `%` specifiers or the `<<` chaining of iostreams.

## Basic Usage

The syntax is straightforward. You provide a format string with `{}` placeholders for the arguments that follow.

The library offers two primary functions for convenience:

- `std::print(fmt, args...)`: Prints the formatted string.
- `std::println(fmt, args...)`: Prints the formatted string and automatically appends a newline character.

```c++
#include <print>
#include <string>

int main() {
    std::string name = "Alice";
    int age = 30;
    double height = 1.68;

    // Using std::print
    std::print("Name: {}, Age: {}, Height: {}\n", name, age, height);

    // Using std::println for automatic newline
    std::println("Name: {}, Age: {}, Height: {}", name, age, height);

    // Positional arguments
    std::println("Hello {0}. {0}, welcome to {1}!", "World", "C++23");
    
    return 0;
}
```

```
Name: Alice, Age: 30, Height: 1.68
Name: Alice, Age: 30, Height: 1.68
Hello World. World, welcome to C++23!
```

## Formatting, using on `std::format` in the backend

- `std::format`: Creates and returns a `std::string`. It performs no I/O.
- `std::print`: Performs I/O by writing formatted text directly to stdout.

```c++
#include <print>
#include <numbers> // for std::numbers::pi

int main() {
    // 1. Width, Alignment, and Fill
    // <: left-align, >: right-align, ^: center-align
    std::println("|{:<10}|", "left");
    std::println("|{:>10}|", "right");
    std::println("|{:*^10}|", "center"); // Use '*' as a fill character

    // 2. Floating-Point Precision
    double pi = std::numbers::pi;
    std::println("Pi to 3 decimal places: {:.3f}", pi);
    std::println("Pi in scientific notation: {:.4e}", pi);

    // 3. Integer Bases
    int value = 100;
    std::println("Decimal: {}", value);
    std::println("Hex: {:#x}", value);    // # adds the '0x' prefix
    std::println("Binary: {:#b}", value); // # adds the '0b' prefix
    
    // 4. Positional Arguments
    std::println("First: {0}, Second: {1}, First again: {0}", "alpha", "beta");
    
    return 0;
}
```

```
|left      |
|     right|
|**center**|
Pi to 3 decimal places: 3.142
Pi in scientific notation: 3.1416e+00
Decimal: 100
Hex: 0x64
Binary: 0b1100100
First: alpha, Second: beta, First again: alpha
```