#include <format>
#include <iostream>
#include <string>

class Program_01_Basics
{
public:
    void Run()
    {
        basic_formatting();
        positional_arguments();
        integer_formatting();
        floating_point_formatting();
        string_and_alignment();
    }

private:
    // Simple substitution of a value.
    void basic_formatting()
    {
        std::cout << "🚀 Exercise 1: Basic Formatting\n";
        std::string formatted = std::format("Hello, {}!", "World");
        std::cout << formatted << "\n\n";
    }

    // Explicitly specify the argument order.
    void positional_arguments()
    {
        std::cout << "🚀 Exercise 2: Positional Arguments\n";
        std::string formatted = std::format("The second argument is {1} and the first is {0}.", "one", "two");
        std::cout << formatted << "\n\n";
    }

    // Formatting integers in different bases.
    void integer_formatting()
    {
        std::cout << "🚀 Exercise 3: Integer Formatting\n";
        int number = 42;
        std::cout << std::format("Decimal: {}\n", number);
        std::cout << std::format("Binary: {:#b}\n", number);
        std::cout << std::format("Octal: {:#o}\n", number);
        std::cout << std::format("Hexadecimal: {:#x}\n", number);
    }

    // Formatting floating-point numbers with precision and scientific notation.
    void floating_point_formatting()
    {
        std::cout << "🚀 Exercise 4: Floating-Point Formatting\n";
        double pi = 3.1415926535;
        std::cout << std::format("Default: {}\n", pi);
        std::cout << std::format("2 decimal places: {:.2f}\n", pi);
        std::cout << std::format("Scientific notation: {:.2e}\n", pi);
        std::cout << std::format("Padded with zeros: {:010.2f}\n\n", pi);
    }

    // Formatting strings with alignment and width.
    void string_and_alignment()
    {
        std::cout << "🚀 Exercise 5: String and Alignment\n";
        std::string text = "Hello";
        std::cout << std::format("Default: '{}'\n", text);
        std::cout << std::format("Right aligned (width 10): '{:>10}'\n", text);
        std::cout << std::format("Left aligned (width 10): '{:<10}'\n", text);
        std::cout << std::format("Center aligned (width 10): '{:^10}'\n", text);
        std::cout << std::format("Padded with '*': '{:*<10}'\n\n", text);
    }
};