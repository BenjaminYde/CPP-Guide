# Function Templates: Your First Template

A **function template** is a blueprint for functions. You already saw `maxOf` work for every type. This page takes it apart: what each piece of syntax means, how the compiler works out `T`, and what to do when it can't.

## The Syntax

```c++
template<typename T>   // 1. Starts a template with one type parameter, T
T maxOf(T a, T b) {    // 2. T can be used wherever a type is expected
    return a > b ? a : b;
}
```

- **`template<...>`**: Starts a template. The angle brackets list its **template parameters**.
- **`typename T`**: Declares a placeholder for a type, named `T`. The name is a convention: `Type` or `Value` work just as well.
- **Inside the function**, `T` is used like any other type: for parameters, the return type and local variables.

`template<class T>` means exactly the same as `template<typename T>`. You'll see `class` in older code, but prefer `typename`: `T` doesn't have to be a class, `int` works too.

## Calling a Function Template

There are two ways to call it:

```c++
#include <iostream>

template<typename T>
T maxOf(T a, T b) {
    return a > b ? a : b;
}

int main() {
    // 1. Deduced: the compiler works out T from the arguments
    std::cout << maxOf(3, 7) / 2 << std::endl; // T = int. Output: 3

    // 2. Explicit: you choose T yourself, and the arguments are converted
    std::cout << maxOf<double>(3, 7) / 2 << std::endl; // T = double. Output: 3.5
}
```

Working out `T` from the arguments is called **template argument deduction**. Most of the time it's what you want, so you rarely write the `<...>`.

## When Deduction Fails: Mixed Types

```c++
maxOf(3, 7.5);
// ERROR: no matching function for call to 'maxOf'
// note: candidate template ignored: deduced conflicting types for parameter 'T' ('int' vs. 'double')
```

The compiler looks at each argument on its own. The first says `T` is `int`, the second says `T` is `double`, and `T` can only be one type. During deduction the compiler **never converts** an argument to make them match.

Three ways to fix it:

```c++
maxOf<double>(3, 7.5);              // 1. Say what T is: 3 is converted to 3.0
maxOf(static_cast<double>(3), 7.5); // 2. Make both arguments the same type
```

3. Give each argument its own type parameter:

```c++
#include <iostream>

template<typename T, typename U>
auto maxOf(T a, U b) {
    return a > b ? a : b;
}

int main() {
    std::cout << maxOf(3, 7.5) << std::endl;  // T = int, U = double. Output: 7.5
    std::cout << maxOf(10, 7.5) << std::endl; // Output: 10 (as a double)
}
```

The return type is now [`auto`](../typing/auto.md#function-return-types-and-auto): the compiler takes it from the `return` statement. Mixing an `int` and a `double` in `? :` gives a `double`, so both calls return a `double`.

Fix 1 is usually the simplest. Use fix 3 only when mixed types are part of what the function is for.

## The String Literal Trap

Deduction picks the type of what you **actually pass**, and a string literal like `"apple"` is not a `std::string`:

```c++
#include <iostream>
#include <string>

template<typename T>
T maxOf(T a, T b) {
    return a > b ? a : b;
}

int main() {
    std::cout << maxOf("apple", "banana") << std::endl;
    // T = const char*: compares the ADDRESSES of the two texts, not the texts.
    // Prints "apple" or "banana", depending on where the compiler stored them.

    std::cout << maxOf<std::string>("apple", "banana") << std::endl; // Output: banana
}
```

It compiles without a warning and may even print the right answer. Pass `std::string` objects, or say `<std::string>`, whenever you mean text.

## Pass by Value or by `const T&`?

`T a` makes a **copy** of every argument. That's free for an `int`, but for a `std::string` or a `std::vector` it copies the whole thing. A template doesn't know which `T` it will get, so the usual choice for parameters it only reads is `const T&`:

```c++
template<typename T>
T maxOf(const T& a, const T& b) { // No copies going in...
    return a > b ? a : b;         // ...only the result is copied out
}
```

## When `T` Doesn't Fit: Reading the Error

A type without `>` can't be used with `maxOf`:

```c++
struct Point {
    int x;
    int y;
};

template<typename T>
T maxOf(const T& a, const T& b) {
    return a > b ? a : b;
}

int main() {
    Point p1{1, 2};
    Point p2{3, 4};
    maxOf(p1, p2);
}
```

Clang reports:

```text
main.cpp:8:14: error: invalid operands to binary expression ('const Point' and 'const Point')
    8 |     return a > b ? a : b;
      |            ~ ^ ~
main.cpp:14:5: note: in instantiation of function template specialization 'maxOf<Point>' requested here
   14 |     maxOf(p1, p2);
      |     ^
```

The error points **inside** the template (line 8), not at your mistake (line 14). That's because the compiler only notices the problem while generating `maxOf<Point>`. Here it's two lines, but a wrong type passed to `std::sort` can produce hundreds, all pointing into standard library headers.

**Fix the type or the call.** Here, give `Point` a `>` with [Operator Overloading](../oop/operator_overloading.md).

```c++
#include <iostream>

struct Point {
    int x;
    int y;

    bool operator>(const Point& other) const {
        return x * x + y * y > other.x * other.x + other.y * other.y;
    }
};

template<typename T>
T maxOf(const T& a, const T& b) {
    return a > b ? a : b;
}

int main() {
    Point p1{1, 2};
    Point p2{3, 4};

    Point biggest = maxOf(p1, p2); // Compiles now: T = Point
    std::cout << biggest.x << ", " << biggest.y << std::endl; // Output: 3, 4
}
```

The template didn't change at all. `Point` now supports the one thing `maxOf` needs, a `>`, so `maxOf<Point>` can be generated.

We can also use C++20 **concepts** to let a template state its requirements up front, so the error appears at the call with a clear message. They are covered later in this guide.

## The Verdict

- **Let the compiler deduce `T`.** Write `<...>` only when deduction fails or you want a different type.
- **Mixed argument types?** Prefer `maxOf<double>(3, 7.5)` over adding a second type parameter.
- **Meant text?** Pass `std::string`, not string literals.
- **Only reading the parameters?** Take them as `const T&`.
