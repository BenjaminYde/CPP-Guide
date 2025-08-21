#include <format>
#include <iostream>
#include <string>

class Person {
public:
    Person(unsigned long long id, const std::string& firstName, const std::string& lastName) noexcept
        : _id(id), _firstName(firstName), _lastName(lastName) {}

    auto getId() const noexcept -> unsigned long long { return _id; }
    auto getFirstName() const noexcept -> const std::string& { return _firstName; }
    auto getLastName() const noexcept -> const std::string& { return _lastName; }
private:
    unsigned long long _id;
    std::string _firstName;
    std::string _lastName;
};

// Specializing std::formatter for our Person class.
template<>
class std::formatter<Person> {
private:
    // This member variable will store the custom specifier we parse.
    char _presentation = 'n'; 
    
public:
    // The parse method is responsible for consuming custom format specifiers.
    // The return value points to the character after the last one parsed by our code.
    constexpr auto parse(std::format_parse_context& context) {
        auto it = context.begin();
        auto end = context.end();

        // Check if there is a custom specifier and if it's one we recognize.
        if (it != end && (*it == 'n' || *it == 'L' || *it == 'f' || *it == 'i')) {
            _presentation = *it;
            ++it;
        }
        return it;
    }
    
    // The format method uses the state parsed by the parse method to produce the output.
    auto format(const Person& person, std::format_context& context) const {
        std::string temp_str;
        switch (_presentation) {
            case 'n':
                temp_str = std::format("{} {}", person.getFirstName(), person.getLastName());
                break;
            case 'L':
                temp_str = std::format("{}, {}", person.getLastName(), person.getFirstName());
                break;
            case 'f':
                temp_str = std::format("{} {} (ID: {})", person.getFirstName(), person.getLastName(), person.getId());
                break;
            case 'i':
                temp_str = std::format("{}", person.getId());
                break;
        }
        
        // We can't simply return a new string. We must write to the output iterator
        // provided by the context.
        return std::format_to(context.out(), "{}", temp_str);
    }
};

class Program_03_Custom_Formater_Parse
{
public:
    void Run()
    {
        Person p(9500, "John", "Doe");

        std::cout << "🚀 Exercise 1: Default Formatting (no specifier)\n";
        std::cout << std::format("Default name: {}\n", p);

        std::cout << "\n🚀 Exercise 2: Using Custom Specifiers\n";
        std::cout << std::format("Last name first: {:L}\n", p);
        std::cout << std::format("Full details: {:f}\n", p);
        std::cout << std::format("ID only: {:i}\n", p);
    }
};