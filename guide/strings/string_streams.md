# String Streams

String streams provide an interface to treat a `std::string` as if it were an input/output stream, like `std::cin` or a file. This allows you to use the familiar insertion `(<<)` and extraction `(>>)` operators for in-memory string parsing and formatting.\

To use them, you must include the `<sstream>` header.

## Types of String Streams

There are three primary string stream classes:

- **Input String Stream**: `std::istringstream`
  - **Purpose**: To read from a string. It's an istream that takes a string as its source.
  - Think of it as wrapping a string to act like `std::cin`.
- **Output String Stream**: `std::ostringstream`
  - **Purpose**: To write to a string. It's an ostream that builds a string in memory.
  - Think of it as a `std::cout` that prints to an internal string instead of the console.
- **String Stream**: `std::stringstream`
  - **Purpose**: To both read from and write to a string.
  - This is the most flexible but also requires careful management of the stream's state.

- **`std::istringstream`**: Input string stream, are used to hold input from a data producer, such as a keyboard, a file, or a network.
- **`std::ostringstream`**: Output string stream, useful for creating formatted strings.
- **`std::stringstream`**: Both input and output operations can be performed.

## Standard streams

A **standard stream** is a pre-connected stream provided to a computer program by its environment. C++ comes with four predefined standard stream objects that have already been set up for your use.

- `cin` is an `istream` object tied to the standard input.
- `cout` is an `ostream` object tied to the standard output.
- `cerr` is an `ostream` object tied to the standard error.
- `clog` is an `ostream` object tied to the standard error.

## Writing to a String with `std::ostringstream`

This is the most common use case: building a formatted string from various data types.

```c++
#include <sstream>
#include <iostream>
#include <string>

int main() {
    std::ostringstream oss;
    
    std::string user = "Alex";
    int score = 1200;
    double completion = 85.5;

    // Use the insertion operator << to build the string
    oss << "User: " << user << ", Score: " << score << ", Completion: " << completion << "%";
    
    // To get the final result, call the .str() member function
    std::string result = oss.str();
    
    std::cout << result << std::endl;
}
```

Output:
```
User: Alex, Score: 1200, Completion: 85.5%
```

## Reading from a String with `std::istringstream`

This is ideal for parsing structured data out of a single string.

```c++
#include <sstream>
#include <iostream>
#include <string>

int main() {
    std::string data = "John 42 185.7";
    std::istringstream iss(data);
    
    std::string name;
    int age;
    double height;
    
    // Use the extraction operator >> to parse the string
    iss >> name >> age >> height;
    
    // Check the results
    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "Height: " << height << std::endl;
}
```

Output:
```
Name: John
Age: 42
Height: 185.7
```

## Retrieving and Setting the String

- `oss.str()`: Returns a copy of the internal string buffer. This is an important detail; it's not a reference. Modifying the returned string does not affect the stream.

- `ss.str("new content")`: Replaces the entire content of the stream's buffer with a new string.

```c++
std::stringstream ss;
ss << "Initial";

std::string content = ss.str(); // content is "Initial"
std::cout << content << std::endl;

ss.str("Replaced"); // The buffer is now "Replaced"
content = ss.str();
std::cout << content << std::endl;
```

## Reusing a String Stream

A common mistake is to try and reuse a stream without properly resetting it. A stream has both content and a state (e.g., `good`, `fail`, `eof`). Both must be reset.

```c++
std::stringstream ss;
ss << "First run";
std::cout << "1. Content: " << ss.str() << std::endl;

// To reuse it, you must do TWO things:
// 1. Clear the buffer content
ss.str("");
// 2. Clear any error flags (like EOF)
ss.clear();

ss << "Second run";
std::cout << "2. Content: " << ss.str() << std::endl;
```

**Bad Practice**: Forgetting to `clear()` the state. If you read from a stream until it fails or hits the end, the stream's state will be `fail` or `eof`. Further operations will do nothing until `clear()` is called.

```c++
// How NOT to do it
std::stringstream ss_bad;
int val;

ss_bad.str("123");
ss_bad >> val; // Works, val is 123. Stream is now at EOF.

ss_bad.str("456"); // You set new content...
ss_bad >> val;     // ...but this fails! The stream is still in an EOF state.

std::cout << "Bad practice value: " << val << std::endl; // Prints 123, not 456

// Correct way
ss_bad.str("456");
ss_bad.clear();    // Reset the state!
ss_bad >> val;     // Now it works.
std::cout << "Correct reuse value: " << val << std::endl; // Prints 456
```

## Tokenizing a String (e.g., CSV parsing)

This is a powerful use case for `std::istringstream` combined with `std::getline`.

```c++
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string csv_line = "CPU,85.5,GPU,92.1,RAM,45.3";
    std::istringstream iss(csv_line);
    std::string token;
    std::vector<std::string> tokens;
    
    // std::getline can take a third argument: a delimiter character.
    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }
    
    for (const auto& t : tokens) {
        std::cout << t << std::endl;
    }
}
```

Output:
```
CPU
85.5
GPU
92.1
RAM
45.3
```

## Pros, Cons, and Alternatives

### Pros

- **Type-Safe**: String streams are type-safe. The compiler prevents you from mismatching format specifiers and types.
- **Familiar Syntax**: If you know `iostream`, you know `sstream`. The API is consistent.
- **Automatic Memory Management**: The stream handles its own memory allocation, growing as needed. You don't need to pre-allocate a buffer.
- **Extensible**: Works automatically with any custom types that have overloaded `operator<<` and `operator>>`.

### Cons

- **Performance Overhead**: Creating a stream object, performing locale-aware formatting, and dynamic memory allocation can be slower than alternatives. The `oss.str()` call creates a full copy of the string, which can be expensive for very large strings.
- **Verbosity**: For simple cases, `std::to_string(123)` is much cleaner than the `ostringstream` equivalent.
- **Stateful Nature**: The internal state flags (`fail`, `eof`, etc.) are a common source of bugs if not managed correctly (as shown in the "reusing" example).

### Modern Alternatives

- **For simple type conversion**: Use `std::to_string` and `std::sto*` functions (e.g., `std::stoi`, `std::stod`). They are faster and express intent more clearly.

- **For formatting (C++20 and later)**: `std::format` (from the `<format>` header) is the new standard. It combines the performance of C-style printf with the type safety of iostreams and a much more powerful syntax.

## Performance Considerations

### C++20: Moving the String

Since C++20, there is an overloaded `str()` member function for rvalues. This allows you to move the underlying string out of the stream instead of copying it. See [cppreference.com](https://en.cppreference.com/w/cpp/io/basic_ostringstream/str).

This is a destructive operation; you use it when you are finished with the stream object and just want to efficiently extract the result.

```c++
#include <sstream>
#include <iostream>
#include <string>
#include <utility> // For std::move

int main() {
    std::ostringstream oss;
    oss << "This is a potentially very long string that we want to avoid copying.";
    
    // Creates a copy:
    // std::string copied_string = oss.str(); 

    // Moves the buffer, no copy:
    std::string moved_string = std::move(oss).str();
    
    // After the move, 'oss' is in a valid but unspecified state.
    // You should not use it further without resetting it.
    std::cout << moved_string << std::endl;
}
```

### C++20: Moving the String

Sometimes you don't need ownership of the string; you just need to look at its contents, perhaps to pass it to a function that accepts a std::string_view. C++20 introduced a way to get a non-owning, zero-copy view into the stream's buffer. See [cppreference.com](https://en.cppreference.com/w/cpp/io/basic_stringbuf/view).

This is the ultimate read-only, high-performance solution.

```c++
#include <sstream>
#include <iostream>
#include <string_view> // Required for std::string_view

// A function that just needs to read the string data
void print_view(std::string_view sv) {
    std::cout << "Printing from view: " << sv << std::endl;
}

int main() {
    // This requires a C++20 compliant compiler and standard library
    std::ostringstream oss;
    oss << "Content that we only want to inspect, not own.";
    
    // Get a zero-copy, non-owning view of the buffer.
    // rdbuf() gets the underlying stream buffer.
    // view() is a C++20 method on the buffer.
    std::string_view sv = oss.rdbuf()->view();
    
    print_view(sv);
    
    // The 'oss' object is completely untouched and can be used further.
    oss << " More content.";
    
    // The old string_view 'sv' is now invalid! We must get a new one.
    sv = oss.rdbuf()->view();
    print_view(sv);
}
```