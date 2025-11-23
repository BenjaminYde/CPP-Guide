#include "01_rule_of_five.h"
#include <format>

int main()
{
    int programId = 1;
    std::cout << std::format("Running program {}...", programId) << std::endl;

    switch (programId)
    {
    case 1:
        Program_01_CopyMove program1;
        program1.Run();
        break;
    }

    return 0;
}