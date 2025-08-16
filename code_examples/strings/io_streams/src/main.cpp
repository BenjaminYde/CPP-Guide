#include "01_cout.h"
#include "02_cin.h"
#include "03_cerr.h"
#include "04_clog.h"
#include <format>

int main(int argc, char* argv[])
{
    int programId = 3;

    if (argc > 1)
        programId = std::atoi(argv[1]);

    std::cout << std::format("Running program {}...", programId) << std::endl;

    switch (programId)
    {
    case 1:
        Program_01_Cout program1;
        program1.Run();
        break;
    case 2:
        Program_02_Cin program2;
        program2.Run();
        break;
    case 3:
        Program_03_Cerr program3;
        program3.Run();
        break;
    case 4:
        Program_04_Clog program4;
        program4.Run();
        break;
    }

    return 0;
}