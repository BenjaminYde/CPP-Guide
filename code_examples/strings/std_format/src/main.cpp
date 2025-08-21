#include "01_basics.h"
#include "02_custom_formatter.h"
#include "03_custom_formatter_parse.h"
#include "04_locales.h"
#include <format>

int main(int argc, char* argv[])
{
    int programId = 4;

    if (argc > 1)
        programId = std::atoi(argv[1]);

    std::cout << std::format("Running program {}...", programId) << std::endl;

    switch (programId)
    {
    case 1:
        Program_01_Basics program1;
        program1.Run();
        break;
    case 2:
        Program_02_CustomFormatter program2;
        program2.Run();
        break;
    case 3:
        Program_03_Custom_Formater_Parse program3;
        program3.Run();
        break;
    case 4:
        Program_04_Locales program4;
        program4.Run();
        break;
    }

    return 0;
}