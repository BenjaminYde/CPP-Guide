#include <iostream>
#include <cstring> // For strlen and strcpy

class Program_02_Better_String
{
public:
    class BetterString {
    public:
        char* m_data;
        size_t m_size;

        BetterString(const char* initial_data) {
            std::cout << "Constructor called for '" << initial_data << "'\n";
            m_size = std::strlen(initial_data) + 1;
            m_data = new char[m_size];
            std::strcpy(m_data, initial_data);
        }

        // 1. Destructor (we already have this)
        ~BetterString() {
            std::cout << "Destructor called for '" << m_data << "'\n";
            delete[] m_data;
        }

        // 2. Deep Copy Constructor
        BetterString(const BetterString& other) {
            std::cout << "Deep copy constructor called.\n";
            m_size = other.m_size;
            m_data = new char[m_size]; // Allocate new memory
            std::strcpy(m_data, other.m_data); // Copy the data
        }

        // 3. Deep Copy Assignment Operator
        BetterString& operator=(const BetterString& other) {
            std::cout << "Deep copy assignment called.\n";
            // Protect against self-assignment (e.g., str1 = str1)
            if (this == &other) {
                return *this;
            }

            // Free the old memory
            delete[] m_data;

            // Allocate new memory and copy the data
            m_size = other.m_size;
            m_data = new char[m_size];
            std::strcpy(m_data, other.m_data);

            return *this;
        }

        void print() const {
            std::cout << m_data << std::endl;
        }
    };

    void Run()
    {
        BetterString str1("Hello");
        BetterString str2 = str1; // Calls deep copy constructor

        std::cout << "str1: "; str1.print();
        std::cout << "str2: "; str2.print();

        // Now, let's change str2 and see what happens to str1
        str2.m_data[0] = 'J';
        std::cout << "\nAfter modifying str2:\n";
        std::cout << "str1: "; str1.print(); // str1 is unchanged!
        std::cout << "str2: "; str2.print();
        
        std::cout << "\nExiting main. Destructors will be called.\n";
        // No crash! Each object manages its own memory.
    }
};