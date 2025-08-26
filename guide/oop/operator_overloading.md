## Operator Overloading

## What is operator overloading?

**Function overloading** provides a mechanism to create and resolve function calls to multiple functions with the same name, so long as each function has a unique function prototype. This allows you to create variations of a function to work with different data types, without having to think up a unique name for each variant.

In C++, operators are implemented as functions. By using function overloading on the operator functions, you can define your own versions of the operators that work with different data types. Using function overloading to overload operators is called **operator overloading**.

### Key Concepts of Operator Overloading

1. **Purpose:** It allows user-defined types to be used in the same manner as built-in types.
2. **Limitations:** You can't create new operators, and you can't change the precedence of operators.
3. **Syntax:** The keyword `operator` followed by the operator symbol is used to define the overloaded function.

### Types of Operators that can be Overloaded

1. Arithmetic operators: `+`, `-`, `*`, `/`
2. Relational operators: `==`, `!=`,` <`, `>`, `<=`,` >=`
3. Logical operators: `&&`, `||`, `!`
4. Increment & decrement: `++`, `--`
5. Assignment: `=`,` +=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`
6. Subscripting: `[]`
7. Function call: `()`
8. Member access: `->`, `->*`
9. Memory management: `new`, `delete`, `new[]`, `delete[]`

### Operators as functions

Consider the following example:

```c++
int x { 2 };
int y { 3 };
std::cout << x + y << '\n';
```

The compiler comes with a built-in version of the plus operator (`+`) for integer operands. When you see the expression x + y, you can translate this in your head to the function call `operator+(x, y)`.

Now consider what happens if we try to add two objects of a user-defined class:

```c++
class MyString
{
public:
    MyString(const char* s) : str(s) {}
private:
    std::string str;
};

MyString string1 { "Hello, " };
MyString string2 { "World!" };
std::cout << string1 + string2 << '\n';
```

What would you expect to happen in this case? The intuitive expected result is that the string “Hello, World!” would be printed on the screen. However, because `Mystring` is a program-defined type, the compiler does not have a built-in version of the plus operator that it can use for `Mystring` operands. So in this case, it will give us an error. In order to make it work like we want, we’d need to write an overloaded function to tell the compiler how the `operator+` should work with two operands of type `Mystring`.

## Creating a custom operator function

### Adding `operator+` to Mystring

```c++
class MyString
{
public:
    // ... constructor and other members

    // Overloading + operator as a member function
    MyString operator+(const MyString& rhs) const {
        return MyString((str + rhs.str).c_str());
    }
private:
    std::string str;
};
```

### Functors: `operator()`

A functor, or function object, is an object that can be called as if it were a function. This is achieved by overloading the function call operator, `operator()`. Functors are often used in the C++ Standard Library, for example, as predicates for algorithms.

```c++
#include <iostream>

class Greeter {
public:
    void operator()(const std::string& name) const {
        std::cout << "Hello, " << name << "!" << std::endl;
    }
};

int main() {
    Greeter greet;
    greet("World"); // Calls Greeter::operator()
    return 0;
}
```