#include <iostream>
#include <cstring> // For strlen and strcpy

class Program_01_Naive_String
{
public:
    class NaiveString {
    public:
        char* m_data;
        size_t m_size;

        NaiveString(const char* initial_data) {
            std::cout << "Constructor called for '" << initial_data << "'\n";
            m_size = std::strlen(initial_data) + 1; // +1 for null terminator
            m_data = new char[m_size];
            std::strcpy(m_data, initial_data);
        }

        // The compiler will generate a default shallow copy constructor like this:
        // NaiveString(const NaiveString& other)
        //     : m_data(other.m_data), m_size(other.m_size) {
        //     std::cout << "Shallow copy constructor called.\n";
        // }

        ~NaiveString() {
            std::cout << "Destructor called for '" << m_data << "'\n";
            delete[] m_data;
        }

        void print() const {
            std::cout << m_data << std::endl;
        }
    };

    void Run()
    {
        NaiveString str1("Hello");
        std::cout << "Copying str1 to str2...\n";
        NaiveString str2 = str1; // (Shallow) copy constructor is invoked here

        std::cout << "str1: "; str1.print();
        std::cout << "str2: "; str2.print();

        // Now, let's change str2 and see what happens to str1
        str2.m_data[0] = 'J';
        std::cout << "\nAfter modifying str2:\n";
        std::cout << "str1: "; str1.print(); // Surprise! str1 is also changed.
        std::cout << "str2: "; str2.print();
        
        std::cout << "\nExiting main. Destructors will be called.\n";
        // CRASH! Both destructors try to delete the same m_data.
    }
};