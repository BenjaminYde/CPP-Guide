# Shallow Copy vs Deep Copy

When you create a copy of an object in C++, the compiler needs to know how to perform that copy. For simple objects, this is straightforward. But when your objects manage resources, like dynamically allocated memory, the concept of "copying" becomes more complex. This is where the distinction between a shallow and a deep copy becomes one of the most critical concepts for writing safe and correct C++ code.

> [!TIP]
> Make sure to checkout the runnable code examples: `code_examples/memory`!

## The Default Behavior: Shallow Copy

When you don't provide your own copy logic, the compiler generates a default copy constructor and a default copy assignment operator for you. These perform a memberwise copy, also known as a shallow copy.

When classes are simple (e.g. do not contain any dynamically allocated memory), this works very well. A shallow copy means creating a new object that shares the same memory location or references as the original object. In the context of C++, a shallow copy typically involves copying the member values of one object to another, without allocating separate memory for any pointer members. This means that the pointer members of the new object will point to the same memory location as the pointer members of the original object.

Shallow copying is the default behavior of the copy constructor and assignment operator if you do not provide your own implementation.

This leads to two catastrophic problems:

- Changes to the data through one object's pointer will be reflected in the other object, which is usually not what you want.
- When the objects are destroyed, both will try to delete the same memory, leading to a double-free error and undefined behavior (usually a crash).

### Bad Practice: The Double-Free Disaster

Let's illustrate this with a naïve String class that manages a raw C-style string.

```c++
#include <iostream>
#include <cstring> // For strlen and strcpy

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

int main() {
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
    return 0; // CRASH! Both destructors try to delete the same m_data.
}
```

When you run this, you will see `str1` change when `str2` is modified. Worse, the program will crash at the end because both `str1` and `str2`'s destructors will attempt to `delete[]` the same memory address.

Output:
```
Constructor called for 'Hello'
Copying str1 to str2...
str1: Hello
str2: Hello

After modifying str2:
str1: Jello
str2: Jello

Exiting main. Destructors will be called.
Destructor called for 'Jello'
Destructor called for 'L�g:'
free(): double free detected in tcache 2
[1]    3398 IOT instruction (core dumped)
```

### Deep Copy & The Rule of Three

To fix this, we must perform a deep copy. A deep copy involves:

- Allocating new memory for the copy.
- Copying the contents from the original memory block to the new one.

**The Rule of Three**: If a class requires a user-defined destructor, a user-defined copy constructor, or a user-defined copy assignment operator, it almost certainly requires all three.

Our `NaiveString` has a destructor, so it needs the other two.

### Good Practice: Implementing the Rule of Three

Let's create a `BetterString` class that correctly implements deep copy.

```c++
#include <iostream>
#include <cstring>

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

int main() {
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
    return 0; // No crash! Each object manages its own memory.
}
```

Output:
```
Constructor called for 'Hello'
Deep copy constructor called.
str1: Hello
str2: Hello

After modifying str2:
str1: Hello
str2: Jello

Exiting main. Destructors will be called.
Destructor called for 'Jello'
Destructor called for 'Hello'
```

This version works correctly. Modifying the copy doesn't affect the original, and there is no double-free crash.

## Rule of Zero

Manually managing memory with new and delete and writing the "Rule of Three" members is tedious and error-prone. Modern C++ provides a much better way, embodied by the **Rule of Zero**.

**The Rule of Zero**: A class should not have to define any of the special member functions (destructor, copy/move constructors, copy/move assignment operators) if it manages its resources using other classes that already follow the Rule of Three/Five (e.g., RAII types).

Instead of raw pointers, use resource-managing classes from the standard library like `std::string`, `std::vector`, or smart pointers like `std::unique_ptr` and `std::shared_ptr`. These classes already handle their own memory correctly. The compiler-generated shallow copy then works perfectly, because "copying" a `std::string` or `std::vector` member automatically performs a deep copy of its contents.

> [!TIP]
> Read more about the Rule of Zero on a the dedicated page [here](../oop/rule_of_zero.md).