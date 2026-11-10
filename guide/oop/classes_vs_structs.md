# Classes vs Structs

## Structs

- Defined with the `struct` keyword.
- By default, all members inside a struct are `public`.
- Commonly used to group data.

```c++
#include <iostream>

struct MyStruct 
{
  int value; // public by default

  MyStruct(int v) : value(v) {}

  void show() 
  {
    std::cout << "Value: " << value << std::endl;
  }
};

void main() 
{
  MyStruct obj(5);
  obj.show(); // Output: Value: 5
}
```
## Classes

- Defined with the `class` keyword.
- By default, all members inside a class are `private`.
- Commonly used for encapsulating data and behavior together.

```c++
#include <iostream>

class MyClass 
{
  int value; // private by default

public:
  MyClass(int v) : value(v) {}

  void show() 
  {
    std::cout << "Value: " << value << std::endl;
  }
};

int main() 
{
  MyClass obj(5);
  obj.show(); // Output: Value: 5
}
```
## Similarities

- Both can have data members, member functions, constructors, destructors, nested types, etc.
- Both can be used with inheritance and polymorphism.
- Both can have access specifiers like `public`, `private`, and `protected`.
## When to use classes

- **Encapsulation:** When you want to encapsulate data and behavior together and control the access to the members, using a class makes the intent clear.

- **Complex Behavior:** If the type you're defining has complex behavior and interactions with other objects, using a class can help you organize the code and emphasize that there's more than just data grouping.

- **Object-Oriented Design:** If you are working within a clearly object-oriented design, using classes might be more in line with the idioms and expectations of the language and project.

