# Templates: Write It Once, Use It for Every Type

Sooner or later you write a function for `int` and then need the exact same logic for `double`, `std::string` or a class of your own. Copying the function for every type works, but it doesn't scale.

**Templates** solve this. You write the code once, with a placeholder where the type goes, and the compiler fills in the real type for you.

## The Problem: The Same Function for Every Type

Say you need a function that returns the larger of two values. C++ lets you **overload** a function: several functions share one name, and the compiler picks the one whose parameter types match the call. So you write one version per type:

```c++
#include <iostream>
#include <string>

int maxOf(int a, int b) {
    return a > b ? a : b;
}

double maxOf(double a, double b) {
    return a > b ? a : b;
}

std::string maxOf(std::string a, std::string b) {
    return a > b ? a : b;
}

int main() {
    std::cout << maxOf(3, 7) << std::endl; // Output: 7
    std::cout << maxOf(2.5, 1.5) << std::endl; // Output: 2.5
    std::cout << maxOf("apple", "banana") << std::endl; // Output: banana
}
```

Look at the three bodies: they are **identical**. Only the type changes. That causes real problems:

- **Every new type needs another copy.** `long`, `char`, `float`, your own `Money` class... the list never ends.
- **Every bug fix must be repeated in every copy.** Miss one, and the copies quietly behave differently.

## The Solution: A Template

Here is the same function written **once**, as a template:

```c++
#include <iostream>
#include <string>

template<typename T>
T maxOf(T a, T b) {
    return a > b ? a : b;
}

int main() {
    std::cout << maxOf(3, 7) << std::endl; // Output: 7
    std::cout << maxOf(2.5, 1.5) << std::endl; // Output: 2.5
    std::cout << maxOf("apple", "banana") << std::endl; // Output: banana
    std::cout << maxOf('a', 'z') << std::endl; // Output: z
}
```

The only new part is the first line, `template<typename T>`. It says: *"what follows is a template, and `T` is a placeholder for a type that will be filled in later."* Everywhere `T` appears, the real type takes its place. One function now handles `int`, `double`, `std::string`, `char` and any type you add later.

A type that can't be compared with `>` is rejected by the compiler:

```c++
struct Point {
    int x;
    int y;
};

int main() {
    Point p1{1, 2};
    Point p2{3, 4};
    maxOf(p1, p2);
    // ERROR: invalid operands to binary expression ('Point' and 'Point')
}
```

The mistake is caught at compile time, not discovered at runtime.

## How It Works: The Compiler Writes the Overloads for You

A template is not a function yet. It is a **blueprint** for functions. When the compiler sees `maxOf(3, 7)`, it:

1. looks at the arguments: `3` and `7` are both `int`, so `T` must be `int`;
2. takes the blueprint and replaces every `T` with `int`;
3. compiles the result as a normal function.

For the `main` above, the compiler generates these four functions behind the scenes:

```c++
int maxOf(int a, int b) { return a > b ? a : b; }                         // from maxOf(3, 7)
double maxOf(double a, double b) { return a > b ? a : b; }                // from maxOf(2.5, 1.5)
std::string maxOf(std::string a, std::string b) { return a > b ? a : b; } // from maxOf(apple, banana)
char maxOf(char a, char b) { return a > b ? a : b; }                      // from maxOf(letter, 'z')
```

These are the overloads from the start of this page, except that now the compiler writes them instead of you. That has a few consequences:

- **It happens at compile time.** By the time your program runs, the template is gone. Only the generated functions remain.
- **No runtime cost.** Each generated function is ordinary code, exactly as fast as one written by hand.
- **Only what you use is generated.** If you never call `maxOf` with a `long`, no `long` version exists.

## You Already Use Templates

Every time you write angle brackets after a name in the standard library, you are using a template:

```c++
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main() {
    // Class templates: you write the template arguments in < >
    std::vector<int> numbers = {5, 2, 8};
    std::array<double, 3> prices = {1.5, 2.0, 3.25};
    std::function<void()> greet = []() { std::cout << "Hi" << std::endl; };

    // Function templates: the template arguments are usually deduced, so no < >
    std::cout << std::max(3, 7) << std::endl; // Output: 7
    std::sort(numbers.begin(), numbers.end()); // numbers is now {2, 5, 8}

    // ...unless the compiler can't deduce them. Then you write them yourself.
    std::unique_ptr<std::string> name = std::make_unique<std::string>("Alex");
}
```

There are two kinds, and this guide covers both:

| Kind | A blueprint for... | Examples |
|---|---|---|
| **Function template** | functions | `std::max`, `std::sort`, `std::make_unique` |
| **Class template** | classes | `std::vector`, `std::array`, `std::unique_ptr`, `std::function` |

## The Verdict: When to Use What?

- **Same logic for many types?** Use a template.
- **Different logic for each type?** Write normal overloads. A template only helps when the bodies would be identical.
