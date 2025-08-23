#include "01_naive_string.h"
#include "02_better_string.h"
#include <format>

int main(int argc, char* argv[])
{
    int programId = 1;

    if (argc > 1)
        programId = std::atoi(argv[1]);

    std::cout << std::format("Running program {}...", programId) << std::endl;

    switch (programId)
    {
        case 1:
            Program_01_Naive_String program1;
            program1.Run();
            break;
        case 2:
            Program_02_Better_String program2;
            program2.Run();
            break;
    }

    return 0;
}