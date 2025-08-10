# String Literals

## What Are They?

You’ve probably seen strings written in a C++ program with quotes around them. For example, the following code outputs the string hello by including the string itself, not a variable that contains it:

```c++
std::cout << "hello" << std::endl;
```

In the preceding line, "hello" is a **string literal** because it is written as a value, not a variable. String literals are actually stored in a read-only part of memory. This allows the compiler to optimize memory usage by reusing references to equivalent string literals. That is, even if your program uses the string literal "hello" 500 times, the compiler is allowed to optimize memory by creating just one instance of hello in memory. This is called **literal pooling**.

## Types of String Literals

- **Simple String Literals**: These are the most straightforward type. 
   Example: `"Hello, World!"`

- **Wide String Literals**: Prefixed by `L` and uses wide characters. 
   Example: `L"Hello, World!"`

- **UTF-8 String Literals**: Prefixed by `u8`. 
   Example: `u8"Hello, World!"`

- **UTF-16 String Literals**: Prefixed by `u`. 
   Example: `u"Hello, World!"`

- **UTF-32 String Literals**: Prefixed by `U`. 
   Example: `U"Hello, World!"`

- **Raw String Literals**: These literals allow you to include backslashes and double quotes within the string without escaping them. 
  Example: `R"(Hello, "World!")"`


```c++
// Without Raw Strings (error-prone and hard to read)
std::string winPath_old = "C:\\Users\\John\\Documents\\file.txt";
std::string regex_old = "\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,}\\b";

// With Raw Strings (clean and clear)
std::string winPath_new = R"(C:\Users\John\Documents\file.txt)";
std::string regex_new = R"(\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\.[A-Z]{2,}\b)";
```

## Escape Sequences

You can use escape sequences to include special characters within string literals. Here are some commonly used escape sequences:

- `\"`: Double quote
- `\\`: Backslash
- `\n`: Newline
- `\t`: Tab

```c++
#include <iostream>

int main() {
    std::cout << "Hello, \"World!\"\n";
    return 0;
}
```

Output:
```
Hello, "World!"
```

## Concatenation of Adjacent Literals

The compiler will automatically concatenate adjacent string literals (of the same type) into a single literal during preprocessing. This is useful for formatting long strings in code without runtime overhead.

```c++
// These two lines are 100% equivalent
const char* query_single = "SELECT * FROM users WHERE id = 123 AND name = 'John Doe';";
const char* query_multi = "SELECT * FROM users "
                          "WHERE id = 123 "
                          "AND name = 'John Doe';";
```