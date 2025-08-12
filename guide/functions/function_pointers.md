# Function Pointers

## The Core Idea: Functions as Data

A **function pointer** is a variable that stores the memory address of a function. Normally, we think of pointers as holding the address of data like an `int` or an object. But the executable code for a function also resides in memory, and we can point to it.

It allows you to treat functions as data, meaning you can:

- Pass a function as an argument to another function.
- Return a function from another function.
- Store functions in an array or a map.

This is the foundation for many powerful programming patterns, most notably the **callback mechanism**.

## The Syntax

The syntax for declaring a function pointer is: `return_type (*pointer_name)(parameter_types);`

- `return_type`: The return type of the function.
- `(*pointer_name)`: The name of your pointer. The parentheses `()` around `*pointer_name` are **critical**. Without them, you'd be declaring a function that returns a pointer.
- `(parameter_types)`: A list of the parameter types the function accepts.

Let's look at a simple example:

```c++
#include <iostream>

// A function we want to point to
int add(int a, int b) {
    return a + b;
}

// Another function with a different signature
void sayHello() {
    std::cout << "Hello!" << std::endl;
}

int main() {
    // 1. Declaration: Declare a pointer `p_add` that can point to a function
    //    which takes two integers and returns an integer.
    int (*p_add)(int, int);

    // 2. Assignment: Assign the address of the 'add' function to the pointer.
    //    The `&` is optional but makes the intent clearer.
    p_add = &add;

    // 3. Invocation: Call the function through the pointer.
    int result = p_add(5, 3); // Implicit dereference, clean and common
    std::cout << "Result: " << result << std::endl;

    // You can also use explicit dereferencing, which is more C-like.
    int result2 = (*p_add)(10, 2);
    std::cout << "Result 2: " << result2 << std::endl;

    // --- Example with a different signature ---
    void (*p_sayHello)();
    p_sayHello = sayHello; // `&` is also optional here
    p_sayHello();
}
```

### How NOT to do it

A very common mistake is forgetting the parentheses in the declaration.

```c++
// MISTAKE: This declares a function named 'p_mistake' that takes two ints
// and returns a pointer to an int (int*).
// This is NOT a function pointer.
int* p_mistake(int, int);

// CORRECT: This declares a pointer named 'p_correct' to a function
// that takes two ints and returns an int.
int (*p_correct)(int, int);
```

## Use Cases

### Use Case 1: Callbacks

The most common use for function pointers is creating **callbacks**. You write a generic function that takes a function pointer as a parameter. This allows the caller to provide a specific action (the callback) for your generic function to execute.

Think of a function that sorts data. It needs to know *how* to compare two elements. You can pass it a comparison function.

```c++
// Generic function that applies an operation to two numbers
void applyOperation(int a, int b, int (*operation)(int, int)) {
    int result = operation(a, b);
    std::cout << "Applying operation to " << a << " and " << b << " -> " << result << std::endl;
}

// Operations
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }

int main() {
    applyOperation(10, 7, add);      // Pass the 'add' function
    applyOperation(10, 7, subtract); // Pass the 'subtract' function
    applyOperation(10, 7, multiply); // Pass the 'multiply' function
}
```

Here, `applyOperation` is generic. The logic it executes depends on the function pointer it's given at runtime.

### Use Case 2: Arrays of Function Pointers

You can create an array of function pointers to build a *dispatch table*. This is a clean way to select an action based on an index, often from user input or an enum.

```c++
// Assume 'add', 'subtract', and 'multiply' are defined as before.
int main() {
    // Create an array of function pointers.
    // The signature for all functions must match.
    int (*operations[])(int, int) = { add, subtract, multiply };

    int choice = 0; // e.g., from user input
    std::cout << "0: Add, 1: Subtract, 2: Multiply" << std::endl;
    std::cin >> choice;

    if (choice >= 0 && choice < 3) {
        int result = operations[choice](100, 20); // Call function by index
        std::cout << "Result: " << result << std::endl;
    }
}
```

## A Note on `void*`: The Generic but Unsafe Pointer

You will often see function pointers used with `void*`. A **`void` pointer** is a generic pointer that can hold the address of *any* type of data, but it doesn't know what type it is. Think of it as a raw memory address.

This pattern is common in C APIs for passing arbitrary "user data" to a callback. The callback receives the `void*` and must cast it back to the correct data type.

### This is powerful, but dangerous

```c++
#include <string>

// A struct to hold our "user data"
struct UserData {
    std::string name;
    int clickCount = 0;
};

// A typical C-style callback that receives generic user data
void on_button_click(void* data) {
    std::cout << "Button was clicked!" << std::endl;
    
    // We MUST cast the void* back to its true type to use it.
    // static_cast is the C++ way to do this.
    UserData* userData = static_cast<UserData*>(data);
    
    // Now we can safely use the data
    userData->clickCount++;
    std::cout << userData->name << " has clicked " << userData->clickCount << " times." << std::endl;
}

// A function that registers a callback and the data to give it
void register_callback(void (*callback)(void*), void* userData) {
    // Imagine some event happens here, like a user clicking a button...
    // Now we call the callback with the user data.
    callback(userData);
}

int main() {
    UserData myData;
    myData.name = "Marc";

    // We pass our callback function and a pointer to our data.
    // The address of `myData` is implicitly converted to void*.
    register_callback(on_button_click, &myData);
    register_callback(on_button_click, &myData);
}
```

### Why Be Careful with `void*`?

  * **Type Unsafe:** The compiler cannot help you. If you cast a `void*` to the wrong pointer type inside the callback, your program will have **undefined behavior**. This is a massive source of bugs.
  * **Verbose:** You always have to write the `static_cast`, which is a signal that you're working outside the C++ type system.

The `void*` pattern for user data is completely replaced by lambda captures in modern C++.

## Taming the Syntax with `using`

The raw function pointer syntax is notoriously difficult to read, especially in function parameters. Modern C++ (`C++11` and later) provides the `using` keyword to create a type alias, which dramatically improves readability.

```c++
// Create an alias named 'ArithmeticFunc' for the complex pointer type
using ArithmeticFunc = int (*)(int, int);

// The alias makes the function declaration much cleaner
void applyOperationClean(int a, int b, ArithmeticFunc op) {
    int result = op(a, b);
    std::cout << "Clean apply: " << result << std::endl;
}

// And it simplifies array declarations
ArithmeticFunc operations[] = { add, subtract, multiply };

int main() {
    applyOperationClean(8, 2, add);
}
```

**Pro Tip:** Always use `using` (or `typedef` in older C++) to create aliases for your function pointer types. Your future self and your colleagues will thank you.

## Pointers to Member Functions (The Tricky Part)

Pointers to non-static member functions are different. A member function requires an object instance to be called on (the `this` pointer). Regular function pointers don't account for this.

The syntax is also different and more complex.

  * **Declaration:** `return_type (ClassName::*pointer_name)(params);`
  * **Invocation:** `(object.*pointer_name)(args);` or `(object_ptr->*pointer_name)(args);`

```c++
class Calculator {
public:
    int add(int a, int b) { return a + b; }
    int subtract(int a, int b) { return a - b; }
};

int main() {
    // Declare a pointer to a member function of Calculator
    int (Calculator::*p_calc)(int, int);

    // Assign a member function
    p_calc = &Calculator::subtract;

    // To call it, you MUST provide an object instance
    Calculator myCalc;
    int result = (myCalc.*p_calc)(10, 4); // result is 6
    std::cout << "Member function pointer result: " << result << std::endl;

    // Using a pointer to an object
    Calculator* myCalcPtr = &myCalc;
    result = (myCalcPtr->*p_calc)(20, 5); // result is 15
    std::cout << "Member function pointer (via ptr) result: " << result << std::endl;
}
```

Notice the `ClassName::` in the declaration and the special `.*` and `->*` operators for invocation.

## Pros, Cons, and The Modern Way

While essential to understand, raw function pointers are often superseded by more powerful and safer alternatives in modern C++.

### Pros of Function Pointers

  * **C Compatibility:** The #1 reason you still need them. They are essential for interfacing with C libraries and older C-style APIs.
  * **Performance:** A function pointer is just an address. Calling it is a direct jump. There is virtually no overhead.
  * **Simplicity:** For simple callbacks without state, they are straightforward.

### Cons of Function Pointers

  * **Awful Syntax:** The syntax is error-prone and hard to read, especially for member functions.
  * **No State:** A function pointer is just an address. It cannot "remember" or capture any local variables from where it was created. This is a major limitation (which the `void*` pattern tries to solve unsafely).
  * **Not a "Catch-All":** They can only point to free functions or `static` member functions. They cannot point to lambdas with captures or function objects (functors).

## The Modern Alternative: `std::function` and Lambdas

For new C++ code, you should almost always prefer `std::function` and lambdas.

  * **Lambdas:** Anonymous, on-the-fly functions that can *capture* state from their surrounding scope in a type-safe way.
  * **`std::function`:** A general-purpose polymorphic function wrapper from the `<functional>` header. It can store, copy, and invoke *any* callable target—a regular function pointer, a lambda, or a function object.

>[!TIP]
> Checkout the page dedicated about lambdas [here](../functions/lambdas.md)!      
> Checkout the page dedicated about `std::function` [here](../functions/std_function.md)!

**Verdict:** Use raw function pointers when you must (e.g., C APIs). For all other cases, `std::function` and lambdas provide a safer, more flexible, and more powerful solution.