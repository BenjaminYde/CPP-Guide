#include <format>
#include <iostream>
#include <string>
#include <locale>
#include <chrono>
#include <stdexcept> // For std::runtime_error
#include <iomanip>   // For std::put_money
#include <algorithm> // For std::sort

class Program_04_Locales
{
public:
    void Run()
    {
        // We wrap locale-dependent code in a try-catch block because creating a
        // locale can fail if it's not supported by the operating system.
        try
        {
            // Attempt to create the locales we'll need.
            // These names are typical for Linux. Windows might use "German_Germany.1252"
            // or "en-US", for example.
            std::locale us_locale("en_US.UTF-8");
            std::locale de_locale("de_DE.UTF-8");
            std::locale fr_locale("fr_FR.UTF-8");
            std::locale ja_locale("ja_JP.UTF-8");
            std::locale ru_locale("ru_RU.UTF-8");

            numeric_formatting(us_locale, de_locale, fr_locale);
            monetary_formatting(us_locale, de_locale);
            datetime_formatting(us_locale, de_locale);
            collation_sorting(de_locale);
            time_zone_formatting(us_locale, fr_locale);
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << "Error: Could not create a required locale.\n";
            std::cerr << "Please ensure your system supports en_US, de_DE, and fr_FR locales.\n";
            std::cerr << "Exception: " << e.what() << "\n";
        }
    }

private:
    // Demonstrates how number formatting (decimal and thousands separators) changes.
    void numeric_formatting(const std::locale& us_locale, const std::locale& de_locale, const std::locale& fr_locale)
    {
        std::cout << "🚀 Exercise 1: Numeric Formatting\n";

        double big_number = 1234567.89;

        // The 'L' specifier tells std::format to use the provided locale.
        std::cout << std::format("Default locale: {:L}\n", big_number);
        std::cout << std::format(us_locale, "US locale:      {:L}\n", big_number);
        std::cout << std::format(de_locale, "German locale:  {:L}\n", big_number);
        std::cout << std::format(fr_locale, "French locale:  {:L}\n", big_number);
        std::cout << "\n";
    }

    // Demonstrates how currency symbols and placement change.
    // Note: std::format doesn't have a direct monetary specifier, so we
    // leverage the locale's stream facets, which is a common pattern.
    void monetary_formatting(const std::locale& us_locale, const std::locale& de_locale)
    {
        std::cout << "🚀 Exercise 2: Monetary Formatting\n";
        
        long double money_value = 1999.95;

        // To format as currency, we can use a stringstream imbued with the locale.
        auto format_currency = [](long double value, const std::locale& loc) {
            std::stringstream ss;
            ss.imbue(loc);
            // use_facet gets the monetary formatting rules from the locale.
            // The 'true' argument specifies international currency symbol (e.g., USD, EUR).
            ss << std::showbase << std::put_money(value * 100, true);
            return ss.str();
        };

        std::cout << "US (USD): " << format_currency(money_value, us_locale) << "\n";
        std::cout << "DE (EUR): " << format_currency(money_value, de_locale) << "\n";
        std::cout << "\n";
    }

    // Demonstrates how date and time representations differ.
    void datetime_formatting(const std::locale& us_locale, const std::locale& de_locale)
    {
        std::cout << "🚀 Exercise 3: Date/Time Formatting\n";

        // Get the current time point
        auto now = std::chrono::system_clock::now();
        
        // C++20 formatting for std::chrono types is powerful.
        // {:%c} is the standard format specifier for the locale's default
        // date and time representation.
        
        std::cout << "Default locale: " << std::format("{:%c}\n", now);
        std::cout << "US locale:      " << std::format(us_locale, "{:%c}", now) << '\n';
        std::cout << "German locale:  " << std::format(de_locale, "{:%c}", now) << '\n';
        
        // We can also be more specific. %x = date, %X = time.
        std::cout << "US (Date only): " << std::format(us_locale, "{:%x}", now) << '\n';
        std::cout << "DE (Date only): " << std::format(de_locale, "{:%x}", now) << '\n';
        std::cout << "\n";
    }

    // Demonstrates how string sorting rules change with locale.
    void collation_sorting(const std::locale& de_locale)
    {
        std::cout << "🚀 Exercise 5: String Collation (Sorting)\n";

        std::vector<std::string> words = { "Zebra", "Äpfel", "Apfel", "Straße" };

        auto print_sorted = [&](const std::string& name, auto compare) {
            std::vector<std::string> sorted_words = words;
            std::sort(sorted_words.begin(), sorted_words.end(), compare);
            std::cout << name << " order: ";
            for (const auto& word : sorted_words) {
                std::cout << word << " ";
            }
            std::cout << "\n";
        };
        
        // 1. Default (lexicographical) sort
        print_sorted("Default C++", std::less<std::string>());
        
        // 2. German locale sort
        // We get the 'collate' facet from the locale, which defines the comparison rules.
        const auto& collate_facet = std::use_facet<std::collate<char>>(de_locale);
        auto german_compare = [&](const std::string& a, const std::string& b) {
            // The compare method returns <0 if a<b, 0 if a==b, >0 if a>b.
            return collate_facet.compare(a.data(), a.data() + a.size(), b.data(), b.data() + b.size()) < 0;
        };
        print_sorted("German", german_compare);
        std::cout << "\n";
    }

    // Demonstrates formatting a single time point across multiple time zones.
    void time_zone_formatting(const std::locale& us_locale, const std::locale& fr_locale)
    {
        const auto universal_time = std::chrono::system_clock::now();
        const std::vector<std::string> time_zone_names = {
            "America/New_York",
            "Europe/London",
            "Europe/Brussels",
            "Asia/Tokyo",
            "Australia/Sydney"
        };
        
        std::cout << std::format("\n--- Report generated at {:%Y-%m-%d %H:%M:%S} UTC ---\n", universal_time);

        for (const auto& tz_name : time_zone_names)
        {
            try
            {
                const auto* time_zone_ptr = std::chrono::locate_zone(tz_name);
                const std::chrono::zoned_time zoned = { time_zone_ptr, universal_time };

                // 1. get the locale
                std::string location = tz_name;
                std::string us_full_date = std::format(us_locale, "{0:%A}, {0:%B %d}, {0:%Y} at {0:%I:%M:%S %p}", zoned);
                std::string us_time_zone = std::format("Time Zone: {0:%Z} (UTC{0:%z})", zoned);
                // This does not work?
                std::string fr_full_date = std::format(fr_locale, "{0:%A} {0:%e} {0:%B} {0:%Y} à {0:%T}", zoned);

                // French formatting with stringstream (seems to work)
                std::string fr_full_date_ss;
                {
                    const auto sys_time_point = zoned.get_sys_time();
                    const std::time_t time_t_point = std::chrono::system_clock::to_time_t(sys_time_point);
                    struct tm time_struct = *std::localtime(&time_t_point);

                    std::stringstream ss;
                    ss.imbue(fr_locale); // Apply the French locale
                    ss << std::put_time(&time_struct, "%A %e %B %Y à %T");
                    fr_full_date_ss = ss.str();
                }

                // Show the data
                std::cout << "----------------------------------------\n";
                std::cout << std::format("  Location: {} {}\n", location, us_time_zone);
                std::cout << std::format("  (en_US)   {}\n", us_full_date);
                std::cout << std::format("  (fr_FR)   {}\n", fr_full_date);
                std::cout << std::format("  (fr_FR) (ss)  {}\n", fr_full_date_ss);
            }
            catch (const std::chrono::nonexistent_local_time& e)
            {
                std::cerr << "Error with time zone '" << tz_name << "': " << e.what() << "\n";
            }
        }
        std::cout << "----------------------------------------\n";
    }
};