# `std::predicate`: Callables That Answer Yes or No (C++20)

A **predicate** is a callable that returns a yes/no answer: "is this number even?", "is `a` smaller than `b`?". You pass them to `std::find_if`, `std::count_if`, `std::erase_if`, ... and to your own filter helpers.

`std::invocable` only checks that something **can be called**. `std::predicate` (header `<concepts>`) also checks that the result **can be used in an `if`**.

## The Problem: `std::invocable` Doesn't Check the Result

```c++
#include <concepts>
#include <functional>
#include <string>
#include <vector>

template<std::invocable<int> F>
int countMatches(const std::vector<int>& values, F condition) {
    int count = 0;
    for (int value : values)
        if (std::invoke(condition, value)) // Needs a bool-like result
            ++count;
    return count;
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4};

    // Returns a std::string, not a yes/no answer. std::invocable still accepts it.
    countMatches(numbers, [](int x) { return std::to_string(x); });
    // ERROR inside countMatches: no viable conversion from 'std::string' to 'bool'
}
```

The error points **inside** the template, at code the caller never wrote.

## The Solution: `std::predicate`

`std::predicate<F, Args...>` means:

1. `F` can be called with `Args...`, **and**
2. the result is **usable as a `bool`** (the standard calls this *boolean-testable*, and compiler errors use that name).

Change one line and the error moves to the call site:

```c++
#include <concepts>
#include <functional>
#include <iostream>
#include <vector>

template<std::predicate<int> F> // was: std::invocable<int>
int countMatches(const std::vector<int>& values, F condition) {
    int count = 0;
    for (int value : values) {
        if (std::invoke(condition, value)) {
            ++count;
        }
    }
    return count;
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};

    std::cout << countMatches(numbers, [](int x) { return x % 2 == 0; }) << std::endl; // Output: 3

    // countMatches(numbers, [](int x) { return std::to_string(x); });
    // ERROR: no matching function for call to 'countMatches'
    //        because 'std::string' does not satisfy '__boolean_testable'
}
```

The three ways to write the constraint:

```c++
// 1. Constrained template parameter
template<std::predicate<int> F>
int countMatches(const std::vector<int>& values, F condition);

// 2. requires clause
template<typename F> 
    requires std::predicate<F, int>       
int countMatches(const std::vector<int>& values, F condition);

// 3. auto
int countMatches(const std::vector<int>& values, std::predicate<int> auto condition); 
```

## Practical Example: A Validator

`std::predicate` rejects bad rules when they are added, and [std::function](./std_function.md) stores them:

```c++
#include <concepts>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class PasswordValidator {
public:
    void addRule(std::string message, std::predicate<const std::string&> auto rule) {
        rules.push_back({std::move(message), std::move(rule)});
    }

    bool validate(const std::string& password) const {
        bool valid = true;
        for (const auto& [message, check] : rules) {
            if (!check(password)) {
                std::cout << "- " << message << std::endl;
                valid = false;
            }
        }
        return valid;
    }

private:
    struct Rule {
        std::string message;
        std::function<bool(const std::string&)> check;
    };
    std::vector<Rule> rules;
};

int main() {
    PasswordValidator validator;
    validator.addRule("At least 8 characters", [](const std::string& p) { return p.size() >= 8; });
    validator.addRule("Contains a digit", [](const std::string& p) {
        return p.find_first_of("0123456789") != std::string::npos;
    });

    // validator.addRule("Broken", [](const std::string& p) { return p; });
    // ERROR: returns a std::string, which does not satisfy '__boolean_testable'

    std::cout << std::boolalpha << validator.validate("hello") << std::endl;
    // Output:
    // - At least 8 characters
    // - Contains a digit
    // false
}
```

## Predicates in the Standard Library

The C++20 range algorithms check what you pass with concepts built on `std::predicate`. For example, `count_if` needs a predicate on one element, `sort` needs one that compares two elements. Any kind of callable works, and a wrong one fails at the call site:

```c++
#include <algorithm>
#include <functional>
#include <vector>

bool isEven(int x) { return x % 2 == 0; }

int main() {
    std::vector<int> numbers = {5, 2, 8, 1, 4};

    std::ranges::count_if(numbers, isEven); // 3: a plain function
    std::ranges::count_if(numbers, [](int x) { return x > 4; }); // 2: a lambda
    std::ranges::sort(numbers, std::greater<int>{}); // {8, 5, 4, 2, 1}: a standard function object

    // std::ranges::count_if(numbers, [](int x) {});
    // ERROR at the call: constraints not satisfied, returns nothing instead of a yes/no
}
```

## Example, a Predicate With Two Arguments

Let's take:

```c++
using Text = const std::string&;

bool check(Text a, Text b, std::predicate<Text, Text> auto compare) {
    return compare(a, b);
}
```

**What the parameter accepts.** `std::predicate<Text, Text>` is a **concept**, not a type. A type like `int` or `std::string` describes one specific kind of value. A concept is a *rule* about types: any type that follows the rule is allowed. That's why `compare` can be many different kinds of things, as long as each one passes the rule's two checks:

1. It can be **called with two strings**.
2. What it returns, a `bool`, or something that converts to one.

`auto` becomes whatever type you pass in, and the compiler runs those two checks on it. It doesn't care what kind of thing it is, only that it can be called that way.

**Things that pass.** Because only the call matters, very different kinds of callables all fit:

```c++
#include <concepts>
#include <iostream>
#include <string>

// `compare` can be a lambda
check("cat", "dog", [](Text a, Text b) { return a.size() == b.size(); });

// `compare` can be a plain function pointer
bool sameFirstLetter(Text a, Text b) { return a[0] == b[0]; }
check("cat", "cow", sameFirstLetter);

// `compare` can be a standard library function object
check("apple", "banana", std::less<std::string>{});  // true: "apple" comes before "banana"

// `compare` can be a struct with operator()
struct Shorter {
    bool operator()(Text a, Text b) const { return a.size() < b.size(); }
};
check("cat", "horse", Shorter{});

// `compare` can be a std::function
std::function<bool(Text, Text)> rule = sameFirstLetter;
check("cat", "cow", rule);
```

**Things that fail.** Each of these breaks one of the two checks, and the error shows up at the call:

```c++
check("cat", "dog", [](Text a) { return a.empty(); }); // Check 1 fails: takes 1 argument, not 2
check("cat", "dog", 42); // Check 1 fails: an int can't be called
check("cat", "dog", [](Text a, Text b) { return a + b; }); // Check 2 fails: returns a string, not a yes/no
```

**What happens behind the scenes.** Because of `auto`, `check` is a template: the compiler writes a separate version for every type you pass, as if you had written the overloads by hand. Each callable is called directly with no overhead, and lambdas work even though you could never write their overload yourself, since their type has no name.

**Compared with `std::invocable` and `std::function`.** Here's the same `check` written three ways:

```c++
// 1. std::invocable: a concept that only checks compare can be called with two strings
bool check(Text a, Text b, std::invocable<Text, Text> auto compare) {
    return compare(a, b);
}

// 2. std::function: one real type, any callable gets wrapped into it
bool check(Text a, Text b, std::function<bool(Text, Text)> compare) {
    return compare(a, b);
}

// 3. std::predicate: a concept that checks the call and that the result is a yes/no
bool check(Text a, Text b, std::predicate<Text, Text> auto compare) {
    return compare(a, b);
}
```

What happens when you pass a lambda that returns a string:

```c++
check("cat", "dog", [](Text a, Text b) { return a + b; });
```

| Version | Result |
|---|---|
| `std::invocable` | Accepted at the call, then fails inside `check` at `return compare(a, b);` |
| `std::function` | Rejected at the call: a string can't be returned as `bool` |
| `std::predicate` | Rejected at the call: constraints not satisfied |