#include <iostream>
#include <string>

class Program_04_Clog
{
public:
    void Run() 
    {
        // Use clog for detailed, non-critical progress updates.
        std::clog << "LOG: Task started. Preparing stage 1...\n";
        std::clog << "LOG: computation...\n";
        std::clog << "LOG: Task finished.\n";

        // The final, important result is sent to cout.
        std::cout << "Final Result: 42\n";
    }
};