#include <iostream>
#include <string>
#include <vector>
#include <limits> // Required for std::numeric_limits

class Program_03_Cerr
{
public:
    // This Run function takes argc and argv to use in the examples.
    void Run()
    {
        exercise_1_basic_error();
        exercise_2_separating_output();
        exercise_3_reporting_usage_error();
        exercise_4_unbuffered_nature();
        exercise_5_stream_redirection();
        exercise_6_error_in_input_loop();
    }

private:
    // Question: How do you report a simple, critical error message?
    void exercise_1_basic_error()
    {
        std::cout << "\n🚀 Exercise 1: Basic Error Reporting\n";
        // Use std::cerr for messages that indicate a problem.
        std::cerr << "Error: This is a critical error message.\n";
    }

    // Question: Why should you use std::cerr for errors instead of std::cout?
    void exercise_2_separating_output()
    {
        std::cout << "\n🚀 Exercise 2: Separating Output from Errors\n";
        // This is the program's actual result, which goes to standard output.
        std::cout << "Operation successful. Result: 42\n";

        // This is a diagnostic/error message, which goes to standard error.
        std::cerr << "Warning: The operation completed with a minor issue.\n";
    }

    // Question: How do you handle incorrect program usage (e.g., missing arguments)?
    void exercise_3_reporting_usage_error()
    {
        std::cout << "\n🚀 Exercise 3: Reporting Usage Errors\n";
        // A real program would check a condition, but we'll simulate failure.
        if (true) { // Simulating the user not providing a needed argument.
            std::cerr << "Usage Error: " << " <input_file>\n";
            // In a real program, you would typically `return 1;` from main here.
        }
        else {
            std::cout << "Program arguments appear correct.\n";
        }
    }

    // Question: What is the practical difference between a buffered (cout) and unbuffered (cerr) stream?
    void exercise_4_unbuffered_nature()
    {
        std::cout << "\n🚀 Exercise 4: Unbuffered Nature\n";
        // This cerr message will appear on the console IMMEDIATELY.
        std::cerr << "Notice: This unbuffered error message appears instantly.\n";
        
        // This cout message might be buffered and not appear until later.
        std::cout << "Processing a long task...";

        std::cout << "...Done.\n";
    }

    // Question: How can a user separate my program's results from its error messages?
    void exercise_5_stream_redirection()
    {
        std::cout << "\n🚀 Exercise 5: Stream Redirection\n";
        std::cout << "This is a result.\n";
        std::cerr << "This is an error.\n";
        std::cout << "This is another result.\n";
        std::cerr << "This is another error.\n";
        std::cout << "To test, run from your terminal:\n";
        std::cout << "  ./io_streams 1> results.txt 2> errors.log\n";
    }

    // Question: Which stream should be used to report invalid user input?
    void exercise_6_error_in_input_loop()
    {
        std::cout << "\n🚀 Exercise 6: Reporting Errors in an Input Loop\n";
        int number;
        std::cout << "Enter a number (or type 'abc' to see the error): ";
        
        if (!(std::cin >> number)) {
            // Use cerr to report the input validation error.
            std::cerr << "Input Error: A valid integer was not provided.\n";
            
            // Cleanup cin for any subsequent program runs
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cout << "Success! You entered: " << number << "\n";
        }
    }
};