## `std::string`

The `std::string` class is the workhorse of C++ text manipulation and part of the STL (Standard Library) and provides a high-level, convenient way to handle strings. It's defined in the `<string>` header. 

## Declaration and Initialization

You can declare and initialize `std::string` in several ways:

```c++
std::string str1;               // Empty string
std::string str2 = "Hello";     // Initialization with a C-style string
std::string str3("World");      // Initialization using constructor
std::string str4 = str2;        // Copy initialization
std::string str5(5, 'a');       // Initialize with 5 'a' characters ("aaaaa")
```
## Concatenation

You can concatenate strings using the `+` and `+=` operators or the `append()` method:

```c++
str1 = str2 + " " + str3;  // "Hello World"
str1 += "!";
str1.append(" How are you?");
```

## Length and Capacity

You can get the length of the string using `length()` or `size()`:

```c++
size_t len = str1.length();
```

You can also check the capacity and resize the string:

```c++
size_t cap = str1.capacity();
str1.resize(50);
```

## Accessing Characters

You can access individual characters using the `[]` operator or the `at()` method:

```c++
char firstChar = str1[0];
char secondChar = str1.at(1);
```

## Substrings and Finding

To get a substring, you can use the `substr()` method:

```c++
std::string sub = str1.substr(0, 5);  // Gets first 5 characters
```

To find a substring or a character, you can use `find()`:

```c++
size_t pos = str1.find("World");  // Returns position or std::string::npos if not found
```

## Comparison

You can compare strings using relational operators (`==`, `!=`, `<`, `>`, etc.) or the `compare()` method:

```c++
if (str1 == str2) {
    // Strings are equal
}
```

## Conversion

To convert a `std::string` to a C-style string, you can use the `c_str()` method:

```c++
const char* cstr = str1.c_str();
```

To convert numbers to strings or vice versa, you can use `std::to_string()` and `std::stoi()`:

```c++
std::string numStr = std::to_string(42);
int num = std::stoi("42");
```

### String to Number Conversions

**Integer (`std::stoi`, `std::stol`, `std::stoll`)**: Convert to `int`, `long`, or `long long`.

```c++
int i = std::stoi("42");
long l = std::stol("42");
long long ll = std::stoll("42");
```

**Floating-Point (`std::stof`, `std::stod`, `std::stold`)**: Convert to `float`, `double`, or `long double`.

```c++
float f = std::stof("42.42");
double d = std::stod("42.42");
long double ld = std::stold("42.42");
```

**Unsigned Integer (`std::stoul`, `std::stoull`)**: Convert to `unsigned long` or `unsigned long long`.

```c++
unsigned long ul = std::stoul("42");
unsigned long long ull = std::stoull("42");
```
## Iterating

You can iterate through a string using iterators or range-based for loops:

```c++
for (char c : str1) {
    // Do something with c
}
```

## Memory Management

`std::string` manages memory automatically, resizing as needed. 

### Reallocation and Iterator Invalidation

When you append characters to a string and its size would exceed its capacity, the string must reallocate:

1. A new, larger block of memory is allocated on the heap (often 1.5x or 2x the old capacity).
1. All characters from the old block are copied to the new block.
1. The old block of memory is deallocated.

### Performance Considerations & Best Practices

When building a large string from many small pieces, call reserve() once if you have a reasonable estimate of the final size. This pre-allocates the necessary memory and avoids multiple, costly reallocations.

```c++
// Inefficient approach
std::string s;
for (int i = 0; i < 1000; ++i) {
    s += get_next_piece(); // Potentially causes many reallocations
}

// Efficient approach
std::string s;
s.reserve(40000); // Estimate the final size and reserve once
for (int i = 0; i < 1000; ++i) {
    s += get_next_piece(); // No reallocations will occur
}
```

## Immutability

Unlike strings in some other languages, `std::string` objects are mutable. You can change them after they are created. 

## Common Methods

### Constructors

- `string()` : Default constructor
- `string(const string& str)`: Copy constructor
- `string(const char* s)`: Construct from a C-string
- `string(size_t n, char c)`: Construct by repeating a character `n` times

### Element Access

- `operator[]`: Access element
- `at()`: Access element, throws out_of_range exception for invalid index
- `front()`: Access the first character
- `back()`: Access the last character

### Modifiers

- `append()`: Append to string
- `push_back()`: Append a character to the end
- `insert()`: Insert into string
- `erase()`: Erase a part of string
- `replace()`: Replace a part of string
- `swap()`: Swap content with another string
- `pop_back()`: Removes the last character
- `clear()`: Clears the content

### Capacity

- `size() / length()`: Return length
- `empty()`: Check if string is empty
- `resize()`: Change size
- `reserve()`: Reserve storage
- `capacity()`: Get capacity
- `shrink_to_fit()`: Shrink to fit

### String operations

- `c_str()`: Get C string equivalent
- `data()`: Get array of characters
- `copy()`: Copy substring to C-array
- `find()`: Find substring
- `rfind()`: Reverse find
- `find_first_of()`: Find character from set
- `find_last_of()`: Find last character from set
- `find_first_not_of()`: Find character not in set
- `find_last_not_of()`: Find last character not in set
- `substr()`: Generate a substring
- `compare()`: Compare strings

### Operators

- `operator+`: Concatenate strings
- `operator+=`: Append to string
- `operator== / operator!=`: Compare equality/inequality
- `operator< / operator<= / operator> / operator>=`: Lexicographical compare

## `std::from_chars`: The Modern Approach to Number Parsing (C++17)

While `std::stoi` and its family are convenient, they have drawbacks for high-performance or robust applications: they can throw exceptions, perform memory allocations, and are dependent on the system's locale.

`std::from_chars` (from the `<charconv>` header) was introduced to solve these problems. It is a low-level, locale-independent, non-allocating, and non-throwing function designed for maximum performance.

### How It Works

It attempts to parse a number from a character range `[first, last)`. It returns a `struct`, `std::from_chars_result`, which contains two essential members:

- `ptr`: A pointer to the first character not consumed by the parse.
- `ec`: An error code of type std::errc. It equals std::errc() (no error) on success.

This mechanism provides fast, explicit, and fine-grained control over parsing.

### Examples

> [!NOTE]
> The following examples use `std::string_view`. Read more about that in the specialized page in this guide.

#### Basic Integer and Floating-Point Parsing

```c++
#include <charconv>
#include <string_view>
#include <iostream>

int main() {
    // --- Integer Example ---
    std::string_view sv_int = "12345";
    int int_val = 0;
    if (auto [p, ec] = std::from_chars(sv_int.begin(), sv_int.end(), int_val); ec == std::errc()) {
        std::cout << "Parsed int: " << int_val << std::endl;
    }

    // --- Double Example ---
    std::string_view sv_flt = "3.14159";
    double dbl_val = 0.0;
    if (auto [p, ec] = std::from_chars(sv_flt.begin(), sv_flt.end(), dbl_val); ec == std::errc()) {
        std::cout << "Parsed double: " << dbl_val << std::endl;
    }
}
```

### Handling Errors

```c++
#include <charconv>
#include <string_view>
#include <iostream>

int main() {
    std::string_view sv = "abc";
    int value = 0;
    auto [ptr, ec] = std::from_chars(sv.begin(), sv.end(), value);

    if (ec == std::errc::invalid_argument) {
        std::cout << "Failed to parse as expected: Not a valid number." << std::endl;
    }
}
```

### Using the Remainder for Sequential Parsing

The returned ptr is key for efficient parsing of multiple values from a single string without creating substrings.

```c++
#include <charconv>
#include <string_view>
#include <iostream>

int main() {
    std::string_view sv = "42 198 -10";
    int v1 = 0, v2 = 0, v3 = 0;

    // Parse first number
    auto res1 = std::from_chars(sv.begin(), sv.end(), v1);
    std::cout << "Parsed: " << v1 << std::endl;

    // Parse second, starting from where the first parse ended (res1.ptr)
    auto res2 = std::from_chars(res1.ptr + 1, sv.end(), v2); // +1 to skip space
    std::cout << "Parsed: " << v2 << std::endl;

    // Parse third
    auto res3 = std::from_chars(res2.ptr + 1, sv.end(), v3); // +1 to skip space
    std::cout << "Parsed: " << v3 << std::endl;
}
```

Output:

```
Parsed: 42
Parsed: 198
Parsed: -10
```

### Using `std::chars_format` for Strict Validation

For floating-point numbers, you can enforce a specific format (`fixed`, `scientific`, or `hex`) for strict data validation. The default is `general`, which accepts both `fixed` and `scientific`.

```c++
#include <charconv>
#include <string_view>
#include <iostream>

int main() {
    std::string_view sv = "1.23e4";
    double value = 0;

    // Attempt to parse with 'fixed' format -- this will fail.
    auto res1 = std::from_chars(sv.begin(), sv.end(), value, std::chars_format::fixed);
    if (res1.ec != std::errc()) {
        std::cout << "'" << sv << "' is not a fixed-point number." << std::endl;
    }

    // Attempt to parse with 'scientific' format -- this will succeed.
    auto res2 = std::from_chars(sv.begin(), sv.end(), value, std::chars_format::scientific);
    if (res2.ec == std::errc()) {
        std::cout << "Parsed '" << sv << "' as scientific: " << value << std::endl;
    }
}
```

Output:

```
'1.23e4' is not a fixed-point number.
Parsed '1.23e4' as scientific: 12300
```