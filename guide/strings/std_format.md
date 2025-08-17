# `std::format`: The Modern C++ Formatting Library (C++20)

`std::format` is a feature introduced in C++20 that provides a type-safe and extensible way to format text. It's designed to be a safer, more robust replacement for older C-style formatting functions like `printf` and C++ stream-based formatting. The `std::format` function resides in the `<format>` header.

## Format Specifiers

`std::format` uses format specifiers within the format string to indicate how to format the arguments. The format specifiers are enclosed in curly braces `{}`.

```c++
std::string formatted = std::format("The answer is {}.", 42);
```
## Positional Arguments

You can specify the position of the arguments to be formatted:

```c++
std::string formatted = std::format("{1} before {0}", "world", "Hello");
// Output: "Hello before world"
```
## Type-Specific Formatting

You can specify the type of formatting you want for each argument:

**Integers**: You can specify the base, padding, etc.

```c++
std::cout << std::format("Binary: {0:#b}, Octal: {0:#o}, Hex: {0:#x}\n", 42);
// "Binary: 0b101010, Octal: 052, Hex: 0x2a"
```

**Floating-Point Numbers**: You can specify the precision, scientific notation, etc.

```c++
std::string float_format = std::format("Float: {:.2f}", 3.14159);  
// Output: 
// "Float: 3.14"
```

**Strings**: You can specify the alignment and width.

```c++
std::string string_format = std::format("String: {:>10}", "right");  
// Output: 
// "String:      right"
```
## Error Handling

### Compile-Time Type Checking

One of the advantages of `std::format` is that it's type-safe. If you try to use a format specifier that's incompatible with the type of the argument, it will result in a compile-time error.

```c++
// Most compilers will fail to compile this line:
// std::format("{:d}", "this is not a number");
```

### Runtime Errors

If the format string itself is invalid (e.g., unmatched `{`), the function will throw a `std::format_error` exception at runtime.

```c++
try {
    auto s = std::format("Mismatched brace {", 42);
} catch (const std::format_error& e) {
    std::cerr << "Formatting error: " << e.what() << std::endl;
}
```

## Performance

`std::format` is generally more efficient than using string streams for formatting. However, the type safety and extensibility often make it a better choice.

### Writing to a Destination (`std::format_to`)

`std::format` is efficient, but it always returns a new `std::string`, which requires a memory allocation. For performance-critical code, you can format directly into an existing buffer or string using `std::format_to`.

```c++
#include <vector>
#include <iterator> // for std::back_inserter

std::string log_prefix = "LOG: ";
// Efficiently append to an existing string, avoiding extra allocations
std::format_to(std::back_inserter(log_prefix), "User '{}' logged in.", "admin");
std::cout << log_prefix << std::endl; // "LOG: User 'admin' logged in."
```

## Formatting User-Defined Types

### Enum to String Formatting

```c++
#include <format>
#include <iostream>
#include <string_view>

enum class Direction { North, East, South, West };

std::string_view to_string( Direction direction ) {
    switch (direction) {
	using enum Direction;
        case North: return "north";
        case East:  return "east";
        case South: return "south";
        case West:  return "west";
        default:    return "unknown";
    }
}

template<>
struct std::formatter<Direction> : std::formatter<std::string_view> {
    auto format( Direction direction, auto& context ) const {
        return std::formatter<std::string_view>::format( to_string( direction ), context );
    }
};

int main() {
    Direction direction = Direction::East;
    std::cout << std::format( "Direction: {}", direction ) << '\n';
}
```

### Example Point

```c++
#include <format>
#include <iostream>
#include <string>

struct Point {
    int x, y;
};

template <>
struct std::formatter<Point> : std::formatter<std::string> {
    auto format(Point p, format_context& ctx) const {
        std::string point_str = std::format("({}, {})", p.x, p.y);
        return std::formatter<std::string>::format(point_str, ctx);
    }
};

int main() {
    Point p = {10, 20};

    std::cout << "Default formatting:\n";
    std::cout << std::format("|{}|\n", p);
}
```

### Example Person with custom character control `std::formatter<Person>`

Let's complete the `std::formatter` for your Person class. This is a great opportunity to show a more advanced technique where the formatter itself can parse different format specifiers to provide multiple output formats for the same object.

We will implement the `parse` function to look for specific characters that control the output.

- `{}` or `{:n}`: Format as "FirstName LastName" (Name).
- `{:L}`: Format as "LastName, FirstName".
- `{:f}`: Format as "FirstName LastName (ID: 123)" (Full).
- `{:i}`: Format as just the ID "123".

```c++
#include <format>
#include <iostream>
#include <string>

class Person {
public:
	Person() = delete;
	Person( unsigned long long id, const std::string& firstName, const std::string& lastName ) noexcept
		: _id( id ), _firstName( firstName ), _lastName( lastName ) {}

    auto getId() const noexcept -> unsigned long long { return _id; }
	auto getFirstName() const noexcept -> const std::string& { return _firstName; }
	auto getLastName() const noexcept -> const std::string& { return _lastName; }
private:
	unsigned long long _id;
	std::string _firstName;
	std::string _lastName;
};

// --- Formatter Implementation ---

template<>
class std::formatter<Person> {
private:
    char _presentation = 'n'; // Our custom specifier
    
    // A nested formatter to handle standard string format specifiers (width, align, etc.)
    std::formatter<std::string> _string_formatter; 

public:
	constexpr auto parse(std::format_parse_context& context) {
		auto it = context.begin();
        auto end = context.end();

		if ( it != end && (*it == 'n' || *it == 'L' || *it == 'f' || *it == 'i') ) {
			_presentation = *it;
			++it;
		}

        return it;
	}
	
	auto format(const Person& person, std::format_context& context ) const {
        // 1. First, create the correct string based on our custom specifier.
        std::string temp_str;
		switch (_presentation) {
			case 'n':
				temp_str = std::format("{} {}", person.getFirstName(), person.getLastName());
                break;
			case 'L':
				temp_str = std::format("{}, {}", person.getLastName(), person.getFirstName());
                break;
			case 'f':
				temp_str = std::format("{} {} (ID: {})", person.getFirstName(), person.getLastName(), person.getId());
                break;
			case 'i':
				temp_str = std::format("{}", person.getId());
                break;
		}
        
        // 2. Delegate the final formatting to the nested string formatter.
        //    This will apply any alignment, width, and fill options.
        return _string_formatter.format(temp_str, context);
	}
};

int main() {
    Person p( 9500, "John", "Doe" );

    std::cout << std::format("Default name: {}\n", p);
    std::cout << std::format("Last name first: {:L}\n", p);
    std::cout << std::format("Full details: {:f}\n", p);
    std::cout << std::format("ID only: {:i}\n", p);
}
```

Output:
```
Default name: John Doe
Last name first: Doe, John
Full details: John Doe (ID: 9500)
ID only: 9500
```