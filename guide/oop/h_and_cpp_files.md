# Header (.h) and Source (.cpp) Files

## Header Files in C++

In C++, the code is typically separated into declaration and implementation. This makes it possible to split code across multiple files. This is where header files come into play.

Header files (with a `.h` or `.hpp` extension) contain declarations of functions, classes, and variables, which can be imported into another file with the `#include` directive.
### Why use header files?

1. **Separation of Declaration and Definition**: You can separate the structure of your classes and functions from their implementation details.
2. **Code Reusability**: By placing common declarations in a header file, you can `#include` them wherever needed.
3. **Encapsulation**: Helps in hiding the implementation details from the user, providing only necessary interfaces.
## Splitting Classes between Header and Source Files

### Header File (`ClassName.h`)
```c++
// This is a guard to prevent multiple inclusions
// It ensures the code within is defined only once
#ifndef CLASSNAME_HPP
#define CLASSNAME_HPP

class ClassName {

public:
    ClassName();
    ~ClassName();
    void publicMethod();
    
private:
    int privateAttribute;
};

#endif // CLASSNAME_HPP
```
### Source File (`ClassName.cpp`)

```c++
#include "ClassName.h"

// Constructor implementation
ClassName::ClassName() {
    // initialization logic
}

// Destructor implementation
ClassName::~ClassName() {
    // cleanup logic
}

// Method implementation
void ClassName::publicMethod() {
    // method logic
}
```

### Including a header:

When you want to use the `ClassName` class in another file, you'll include its header:

```c++
#include "ClassName.h"
```

## Tips and Notes:

- **Include Guards**: In the header files, you might have noticed the `#ifndef`, `#define`, and `#endif` preprocessor directives. They are called _include guards_. They prevent a file from being included more than once, which can cause redefinition errors.

- **Forward Declarations**: Sometimes, you only need to tell the compiler that a name exists without giving all the details. This is called a forward declaration. It can help reduce compile-time dependencies.

```c++
class B; // Forward declaration 

class A 
{ 
public: 
	A(); 
	void setB(B* b); 
	void showB(); 

private: 
	B* b_instance; // We can use pointer or reference to B 
};
```

- **Inline Functions**: If a member function is defined inside the class definition itself, it is by default an inline function. This suggests to the compiler that the function should be inlined where it's called for optimization, but the compiler can choose to ignore this suggestion.

- **Use `#pragma once`**: Some modern compilers support `#pragma once` as an alternative to traditional include guards. When you use `#pragma once`, the compiler ensures the header is included only once. However, it's non-standard and may not be supported everywhere.

## Libraries

Separating the class definition and class implementation is very common for libraries that you can use to extend your program. Throughout your programs, you’ve `#included` headers that belong to the standard library, such as iostream, string, vector, array, and other. Notice that you haven’t needed to add iostream.cpp, string.cpp, vector.cpp, or array.cpp into your projects. 

Your program needs the declarations from the header files in order for the compiler to validate you’re writing programs that are syntactically correct. However, the implementations for the classes that belong to the C++ standard library are contained in a precompiled file that is linked in at the link stage. You never see the code.

Outside of some open source software (where both .h and .cpp files are provided), most 3rd party libraries provide only header files, along with a precompiled library file. There are several reasons for this: 1) It’s faster to link a precompiled library than to recompile it every time you need it, 2) a single copy of a precompiled library can be shared by many applications, whereas compiled code gets compiled into every executable that uses it (inflating file sizes), and 3) intellectual property reasons (you don’t want people stealing your code).