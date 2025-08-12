# `std::string_view`

`std::string_view`, introduced in C++17 and defined in the `<string_view>` header, is a lightweight, non-owning reference to a sequence of characters. Think of it as a read-only "view" into an existing string.

## The "Why": Solving Common Performance Problems

To understand why `std::string_view` was introduced, we need to look at a few very common operations that were surprisingly inefficient before C++17: passing strings to functions and creating substrings. Both often led to unnecessary memory allocations, which can severely degrade performance in demanding applications.

### Problem 1: The Hidden Cost of Flexible Functions

Let's say we have a simple logging function. We want it to accept any kind of string (`std::string`, string literals, etc.). The standard way to do this before C++17 was with `const std::string&`.

```c++
// The way before C++17
void logMessage(const std::string& message);

// This works perfectly and efficiently. 'user' is already a std::string.
std::string user = "Marc";
logMessage(user);

// This looks simple, but it hides a performance trap.
logMessage("User logged out");
```

When you pass a string literal like `"User logged out"` to a function expecting a c`onst std::string&`, here’s a simplified look at the costly work that happens behind the scenes:

1. **A Temporary std::string is Created**: The compiler sees that the types don't match. To fix this, it constructs a brand new, temporary std::string object from your literal.

1. **Memory is Allocated and Data is Copied**: This new std::string object can't use the original literal's memory. It must ask the system for a new block of memory on the heap, and then it copies all the characters from your literal into that new block. This allocation and copy is the main performance penalty.

1. **The Temporary is Used and Destroyed**: The function uses this temporary string, and as soon as the function call is over, the temporary object is destroyed. This frees the memory it allocated in the previous step.

So, in short: every time you make that call with a literal, you force a cycle of allocate -> copy -> use -> free.

`std::string_view` avoids this entire dance by simply creating a lightweight view that points directly to the original literal's memory, with no allocations or copies needed.

This allocation-copy-free cycle, while small, becomes a significant bottleneck when the function is called thousands of times a second in a server, a game loop, or a file parser.

### Problem 2: The High Price of Substrings

Another frequent operation is to extract a piece of a string. `std::string` has a `.substr()` method, but it comes with a cost: it always creates a new `std::string`.

Imagine parsing a key-value pair from a configuration line like `"username=Alice"`.

```c++
std::string line = "username=Alice";

// Find the position of the '='
size_t delimiter_pos = line.find('=');

// Extract the key and value using substr()
std::string key = line.substr(0, delimiter_pos);    // Allocates memory and copies "username"
std::string value = line.substr(delimiter_pos + 1); // Allocates memory and copies "Alice"
```

Even though the characters `username` and `Alice` already exist within the `line` string, we are allocating new memory and copying them just to create two new string objects. This is wasteful.

## The Solution: A Zero-Allocation View (`std::string_view`)

`std::string_view` solves both problems elegantly. It acts as a lightweight, non-owning "view" or "reference" to a piece of an existing string. It's essentially just a pointer and a length.

### Pro's and Con's

#### Pros:

- **Fast**: Creating and copying a `std::string_view` is extremely cheap (just copying a pointer and a size). It avoids dynamic memory allocation.

- **Flexible**: It can be created from a `std::string`, a C-style string (`const char*`), or a string literal, making function parameters much more versatile.

#### Cons:

- **Dangerous Lifetime**: Its biggest strength is its biggest danger. Since it's non-owning, you must ensure the underlying string data outlives the `string_view`. A failure to do so results in a dangling view, leading to undefined behavior.

- **Read-Only**: It provides a read-only view of the data.

- **Not Null-Terminated**: Unlike `std::string`, it does not guarantee that the character sequence it points to is null-terminated.

### Basic Usage

You can construct a `string_view` from almost any string type.

```c++
#include <iostream>
#include <string>
#include <string_view>

int main() {
    std::string str = "Hello, C++";
    std::string_view sv1 {str}; // From a std::string

    const char* c_str = "Hello, C-style";
    std::string_view sv2 {c_str}; // From a C-style string

    std::string_view sv3 {"Hello, Literal"}; // From a string literal
}
```

Let's rewrite the above problem examples using `std::string_view`:

### Problem 1: allocation-copy-free cycle

```c++
// The modern, efficient way
void logMessage(std::string_view message); // Takes a view

// No allocation here! The view just points to the original string's data.
std::string user = "Marc";
logMessage(user);

// No allocation here either! The view points directly to the string literal's memory.
logMessage("User logged out");
```

### Problem 2: Substrings

```c++
std::string_view line_view = "username=Alice";

size_t delimiter_pos = line_view.find('=');

// Creating substrings is now virtually free. No allocations!
std::string_view key = line_view.substr(0, delimiter_pos);    // Just a pointer + size.
std::string_view value = line_view.substr(delimiter_pos + 1); // Just a pointer + size.
```

By using `std::string_view`, we have eliminated all the unnecessary memory allocations in these common scenarios. We are simply pointing to memory that already exists, making our code faster and more memory-efficient. This is the core reason why `std::string_view` is an indispensable tool for modern C++ development.

## The Golden Rule: Lifetime Management

A `std::string_view` must not outlive the character array it points to. This is the single most important rule to remember. Failure to follow it will lead to crashes and undefined behavior.

### How NOT to do it: Creating Dangling Views

Here are the most common ways to create a dangling `string_view`. Avoid these patterns.

#### 1. Returning a view to a local variable:

The `std::string` local is destroyed when the function returns, leaving the `string_view` pointing to garbage memory.

```c++
std::string_view get_a_dangling_view() {
    std::string local = "This will not end well";
    return local; // DANGEROUS! The string 'local' is destroyed here.
} // The memory for 'local' is gone.
```

#### 2. Creating a view from a temporary `std::string`:

The temporary `std::string` created by the + operator is destroyed at the end of the line, leaving `sv` dangling.

```c++
std::string first = "Hello";
std::string second = "World";
std::string_view sv = first + " " + second; // DANGEROUS!
// The temporary std::string from the concatenation is destroyed immediately.
// 'sv' is now dangling.
```

## Utility Functions

A major strength of `std::string_view` is that it provides almost the same rich, read-only interface as `std::string`. This means you can immediately use familiar functions:

- **Accessors**: `size()`, `length()`, `empty()`, `front()`, `back()`, `operator[]`, and `data()`.
- **Searching**: `find()`, `rfind()`, `find_first_of()`, `find_last_of()`, etc.

It also has two very useful methods that `std::string` lacks for modifying the view itself (not the underlying data):

- `remove_prefix(n)`: Shrinks the view by moving its start forward `n` characters.
- `remove_suffix(n)`: Shrinks the view by reducing its size by `n` characters from the end.

These are extremely efficient for parsing tasks, as they don't involve any copying.

## The Null-Termination Trap

This is a critical difference from `std::string` that every developer must know. A `std::string` is always guaranteed to be null-terminated. A std::`string_view` is NOT.

A view can point to a subsection of another string, so there is no guarantee that the character after its last character is a null terminator (`\0`).

### How NOT to do it: Passing a string_view to a C API

Legacy C functions (like `printf`, `strcmp`, `atoi`) expect null-terminated strings. Passing them data from a string_view can read past the end of the buffer, leading to crashes or security vulnerabilities.

```c++
#include <cstdio> // for printf
#include <string>
#include <string_view>

int main() {
    std::string str = "Hello C++"; // Contains "Hello C++\0"
    std::string_view sv{str.data(), 5}; // sv now views "Hello"

    // sv.data() points to 'H', but there is no '\0' after the 'o'.
    // The memory after 'o' contains ' ', 'C', '+', '+', '\0'.
    printf("%s\n", sv.data()); // DANGEROUS! Will likely print "Hello C++" or garbage.
}
```

### How to do it: Create a temporary std::string

If you absolutely must pass the contents of a view to a C-style API, the only safe way is to first create a `std::string` from it. This allocates memory and creates a proper, null-terminated copy.

```c++
// Correct way to interact with a C API
std::string temp_str(sv);
printf("%s\n", temp_str.c_str()); // SAFE. Prints "Hello"
```

## `sv` Literal

C++17 also introduced a user-defined literal, `sv`, to make creating a `string_view` from a literal effortless. This improves readability and makes your intent clear.

To use it, you must bring the appropriate namespace into scope.

```c++
#include <string_view>
#include <iostream>

// You need to bring the namespace into scope
using namespace std::literals;

void printView(std::string_view sv) {
    std::cout << sv << std::endl;
}

int main() {
    // Instead of this:
    std::string_view sv1 {"A literal"};

    // You can do this:
    auto sv2 = "A literal"sv; // The type of sv2 is std::string_view

    printView(sv2);
}
```