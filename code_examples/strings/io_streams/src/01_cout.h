#include <iostream>
#include <string>
#include <iomanip>  // Required for most manipulators
#include <vector>

// For Exercise 15, we need to define our custom type and its output operator
// before we use it in the class.
struct Point {
    int x, y;
};

// This is the operator overload that teaches cout how to print a Point.
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}


class Program_01_Cout
{
public:
    void Run()
    {
        exercise_01_basic_output();
        exercise_02_chaining_types();
        exercise_03_variables_and_expressions();
        exercise_04_floating_point_precision();
        exercise_05_field_width_and_alignment();
        exercise_06_fill_character();
        exercise_07_numerical_base();
        exercise_08_show_base_prefix();
        exercise_09_boolean_format();
        exercise_10_scientific_notation();
        exercise_11_quoted_strings();
        exercise_12_newline_performance();
        exercise_13_unformatted_char_output();
        exercise_14_unformatted_block_output();
        exercise_15_custom_type_output();
    }

private:
    // 1. How do you print a simple literal string?
    void exercise_01_basic_output() {
        std::cout << "\n🚀 1. Basic Output\n";
        std::cout << "Hello, C++!\n";
    }

    // 2. How do you combine different literal types in one statement?
    void exercise_02_chaining_types() {
        std::cout << "\n🚀 2. Chaining Literals\n";
        std::cout << "ID: " << 42 << ", Status: " << true << ", Pi: " << 3.14 << "\n";
    }

    // 3. How do you print the values of variables and the result of expressions?
    void exercise_03_variables_and_expressions() {
        std::cout << "\n🚀 3. Variables and Expressions\n";
        int a = 10, b = 20;
        std::cout << "a: " << a << ", b: " << b << ", a + b = " << (a + b) << "\n";
    }

    // 4. How do you control the number of decimal places for a double?
    void exercise_04_floating_point_precision() {
        std::cout << "\n🚀 4. Floating-Point Precision\n";
        double pi = 3.1415926535;
        std::cout << std::fixed << std::setprecision(4) << pi << "\n"; // Outputs: 3.1416
    }

    // 5. How do you align output within a specific width?
    void exercise_05_field_width_and_alignment() {
        std::cout << "\n🚀 5. Field Width and Alignment\n";
        std::cout << "|" << std::setw(10) << "right" << "|\n";
        std::cout << "|" << std::left << std::setw(10) << "left" << "|\n";
    }

    // 6. How do you change the padding character for alignment?
    void exercise_06_fill_character() {
        std::cout << "\n🚀 6. Fill Character\n";
        std::cout << std::left << std::setfill('-') << std::setw(15) << "Padded" << "\n"; // Outputs: Padded---------
    }

    // 7. How do you display integers in hexadecimal or octal?
    void exercise_07_numerical_base() {
        std::cout << "\n🚀 7. Numerical Base\n";
        int number = 255;
        std::cout << "Dec: " << std::dec << number 
                  << ", Hex: " << std::hex << number 
                  << ", Oct: " << std::oct << number << "\n";
        // Reset the base to decimal for subsequent exercises.
        std::cout << std::dec;
    }

    // 8. How do you force the base prefix (0x, 0) to be shown?
    void exercise_08_show_base_prefix() {
        std::cout << "\n🚀 8. Show Base Prefix\n";
        int number = 255;
        // std::showbase makes the base explicit in the output
        std::cout << std::showbase << std::hex << number << "\n"; // Outputs: 0xff
        // Reset the base to decimal for subsequent exercises.
        std::cout << std::dec;
    }

    // 9. How do you print boolean values as 'true'/'false' instead of '1'/'0'?
    void exercise_09_boolean_format() {
        std::cout << "\n🚀 9. Boolean Formatting\n";
        std::cout << "Default: " << true << "\n";
        std::cout << "Alpha: " << std::boolalpha << true << "\n";
    }

    // 10. How do you format numbers in scientific notation?
    void exercise_10_scientific_notation() {
        std::cout << "\n🚀 10. Scientific Notation\n";
        double large_num = 1234567890.0;
        double small_num = 0.000000123;
        std::cout << std::scientific << large_num << " " << small_num << "\n";
    }

    // 11. How do you print a string with quotes and handled escape characters?
    void exercise_11_quoted_strings() {
        std::cout << "\n🚀 11. Quoted Strings\n";
        std::string path = "C:\\Users\\Test User\\My Docs";
        std::cout << std::quoted(path) << "\n"; // Outputs: "C:\\Users\\Test User\\My Docs"
    }

    // 12. What is the performance-conscious way to print a newline?
    void exercise_12_newline_performance() {
        std::cout << "\n🚀 12. Newline Performance\n";
        // '\n' is just a character. It's fast and relies on the buffer.
        std::cout << "Prefer '\\n' in performance-critical code.\n";
        
        // std::endl inserts '\n' AND forces a slow buffer flush. Avoid in loops.
        std::cout << "Use std::endl only when you must flush." << std::endl;
    }
    
    // 13. Why would you use cout.put() instead of << for a character?
    void exercise_13_unformatted_char_output() {
        std::cout << "\n🚀 13. Unformatted Char Output (put)\n";
        char num_char = 65; // ASCII for 'A'
        // The << operator might print this as the number 65, not the character 'A'.
        std::cout << "Using <<: " << num_char << "\n";
        // cout.put() GUARANTEES it will be printed as a character.
        std::cout << "Using put(): ";
        std::cout.put(num_char).put('\n');
    }

    // 14. How do you write a block of unformatted data (like a substring)?
    void exercise_14_unformatted_block_output() {
        std::cout << "\n🚀 14. Unformatted Block Output (write)\n";
        const char* text = "C++ Programming";
        std::cout.write(text, 3); // Writes the first 3 characters: "C++"
        std::cout.put('\n');
    }

    // 15. How can cout print a custom struct or class?
    void exercise_15_custom_type_output() {
        std::cout << "\n🚀 15. Custom Type Output\n";
        Point p = {10, 20};
        // This works because we overloaded the << operator for the Point struct.
        std::cout << "My custom point: " << p << "\n";
    }
};