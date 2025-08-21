#include <format>
#include <iostream>
#include <string>

struct Point {
    int x, y;
};

// Specializing std::formatter for our Point struct.
template <>
struct std::formatter<Point> : std::formatter<std::string> {
    // We only need the format method. 
    // The default parse() from std::formatter<std::string> is sufficient for this simple case.
    auto format(Point p, format_context& ctx) const {
        // option 1
        {
            std::string point_str = std::format("({}, {})", p.x, p.y);
            return std::formatter<std::string>::format(point_str, ctx);
        }
        // option 2: writes "(10, 20)" directly into the final destination. No extra string created.
        {
            return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
        }

    }
};

class Program_02_CustomFormatter
{
public:
    void Run()
    {
        Point p = {10, 20};

        std::cout << "🚀 Exercise 1: Default Formatting\n";
        std::cout << std::format("The point is {}\n", p);

        std::cout << "\n🚀 Exercise 2: Leveraging String Formatting\n";
        std::cout << std::format("Point, right-aligned: '{:>15}'\n", p);
        std::cout << std::format("Point, center-aligned: '{:^15}'\n\n", p);
    }
};