#include <iostream>
#include <string>
#include <limits>   // Required for std::numeric_limits

class Program_02_Cin
{
public:
    void Run()
    {
        exercise_1_basic_input();
        exercise_2_whitespace_pitfall();
        exercise_3_reading_full_lines();
        exercise_4_mixing_reads_trap();
        exercise_5_robust_error_handling();
        exercise_6_unformatted_char_input();
    }

private:
    // Question: How do you read a single word and a number from the user?
    void exercise_1_basic_input()
    {
        std::cout << "\n🚀 Exercise 1: Basic Input\n";
        std::string name;
        int id;
        std::cout << "Enter your first name and ID (e.g., 'Alice 42'): ";
        std::cin >> name >> id;
        std::cout << "Read Name: " << name << ", ID: " << id << "\n";
    }

    // Question: What happens if you try to read a full name with spaces using `>>`?
    void exercise_2_whitespace_pitfall()
    {
        std::cout << "\n🚀 Exercise 2: Whitespace Pitfall\n";
        std::string fullName;
        std::cout << "Enter your full name (e.g., 'John Smith'): ";
        std::cin >> fullName; // This will only read "John"
        std::cout << "Hello, " << fullName << "! The rest of the input is left in the buffer.\n";
        
        // Cleanup for the next exercise
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Question: What is the correct way to read an entire line of text?
    void exercise_3_reading_full_lines()
    {
        std::cout << "\n🚀 Exercise 3: Reading Full Lines (getline)\n";
        std::string address;
        std::cout << "Enter your full address: ";
        // std::getline reads everything until the user presses Enter.
        std::getline(std::cin, address);
        std::cout << "Address captured: " << address << "\n";
    }

    // Question: Why does `getline` seem to be skipped after reading a number with `>>`?
    void exercise_4_mixing_reads_trap()
    {
        std::cout << "\n🚀 Exercise 4: The Trap of Mixing `>>` and `getline`\n";
        int age;
        std::string name;
        std::cout << "Enter your age: ";
        std::cin >> age; // Reads the number, but leaves the newline ('\n') in the buffer.
        
        // To fix the "skipped" getline, we must ignore the leftover newline.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Enter your name: ";
        std::getline(std::cin, name); // Without ignore(), this would read the empty line and seem to be skipped.
        std::cout << "Hello, " << name << "! You are " << age << " years old.\n";
    }

    // Question: How do you prevent the program from crashing or looping if the user enters text instead of a number?
    void exercise_5_robust_error_handling()
    {
        std::cout << "\n🚀 Exercise 5: Robust Error Handling\n";
        int number;
        std::cout << "Enter an integer: ";
        // The loop continues as long as the input operation fails.
        while (!(std::cin >> number)) {
            std::cerr << "Invalid input. Please enter an integer: ";
            std::cin.clear(); // 1. Clear the error flags.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 2. Discard the bad input.
        }
        std::cout << "You entered the valid number: " << number << "\n";
    }

    // Question: How can you read the very next character, even if it's a space?
    void exercise_6_unformatted_char_input()
    {
        std::cout << "\n🚀 Exercise 6: Unformatted Char Input (get)\n";
        char ch;
        std::cout << "Enter a character, then a space, then another character (e.g., 'a b'): ";
        // cin.get() reads the next character without skipping whitespace.
        ch = std::cin.get();
        std::cout << "First character read: '" << ch << "'\n";
        ch = std::cin.get();
        std::cout << "Second character read: '" << ch << "'\n";
        ch = std::cin.get();
        std::cout << "Third character read: '" << ch << "'\n";

        // Cleanup for any subsequent program runs
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};