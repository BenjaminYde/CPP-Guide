# Generic Callables: `std::invoke`, `std::invoke_result_t` and `std::invocable`

`std::function` and `std::move_only_function` store a callable behind a fixed signature. But very often you don't want to *store* it, you just want to write a function that **accepts any callable and calls it**: a retry helper, a timer, a logger, a thread pool, `std::async`...

That is the job of templates. Writing such a template raises three questions, and the standard library has one tool for each:

| Question | Tool | Header | Standard |
|---|---|---|---|
| How do I call *any* callable in one uniform way? | `std::invoke` | `<functional>` | C++17 |
| What type does calling it return? | `std::invoke_result_t` | `<type_traits>` | C++17 |
| How do I reject things that can't be called? | `std::invocable` | `<concepts>` | C++20 |

## `std::invoke`: One Way to Call Everything

A template that accepts "any callable" and simply writes `f(args...)` works for functions, lambdas and functors. It **does not** work for pointers to members, because those need the awkward `.*` or `->*` syntax.

```c++
#include <iostream>

struct Wallet {
    int balance = 100;
    void add(int amount) { balance += amount; }
};

template<typename F, typename... Args>
void callIt(F f, Args... args) {
    f(args...); // Fine for lambdas, but NOT for &Wallet::add
}

int main() {
    Wallet wallet;
    callIt([](int x) { std::cout << x << std::endl; }, 5); // OK

    callIt(&Wallet::add, wallet, 50);
    // ERROR: called object type 'void (Wallet::*)(int)' is not a function or function pointer
}
```

`std::invoke(f, args...)` hides the difference. It applies the right calling syntax for every kind of callable:

```c++
#include <functional>
#include <iostream>

struct Wallet {
    int balance = 100;
    void add(int amount) { balance += amount; }
};

int square(int x) { return x * x; }

int main() {
    Wallet wallet;

    // 1. Regular function and lambda: same as calling them directly
    std::cout << std::invoke(square, 4) << std::endl; // Output: 16
    std::cout << std::invoke([](int a, int b) { return a + b; }, 2, 3) << std::endl; // Output: 5

    // 2. Member function: the object comes first, then the arguments
    std::invoke(&Wallet::add, wallet, 50);   // same as wallet.add(50)
    std::invoke(&Wallet::add, &wallet, 25);  // also works with a pointer

    // 3. Data member: "calling" it reads the member
    std::cout << std::invoke(&Wallet::balance, wallet) << std::endl; // Output: 175
}
```

## `std::invoke_result_t`: What Does Calling It Return?

Sometimes you need the return type **before** you call the callable, for example to create a container for the results. `std::invoke_result_t<F, Args...>` gives you the type that `std::invoke(f, args...)` would return, without calling anything.

```c++
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

template<typename F>
auto transformAll(const std::vector<int>& inputs, F f) {
    // Which type does f(int) return? We need it to declare the vector.
    using Result = std::invoke_result_t<F, int>;

    std::vector<Result> output;
    for (int input : inputs)
        output.push_back(std::invoke(f, input));

    return output;
}

int main() {
    std::vector<int> numbers = {1, 2, 3};

    auto doubled = transformAll(numbers, [](int x) { return x * 2; });
    // Result = int  ->  std::vector<int>

    auto labels = transformAll(numbers, [](int x) { return "item " + std::to_string(x); });
    // Result = std::string  ->  std::vector<std::string>

    for (const auto& label : labels)
        std::cout << label << std::endl;

    // Output:
    // item 1
    // item 2
    // item 3
}
```

#### Good to know

- **The arguments are types, not values:** `std::invoke_result_t<decltype(square), int>` is `int`.
- **It replaces `std::result_of`:** The old `std::result_of<F(Args...)>` was deprecated in C++17 and removed in C++20.
- **It is a compile error if `F` can't be called with `Args...`**. To *check* first instead of failing, use `std::invocable`.

## `std::invocable`: Reject Non-Callables Early (C++20)

What happens when someone passes something that can't be called to an unconstrained template?

### How NOT to do it

```c++
#include <functional>

template<typename F>
void run(F f) {
    std::invoke(f);
}

int main() {
    run(42); // Oops, an int is not callable
}
```

The compiler only notices the mistake **deep inside** the template (and inside the standard library's implementation of `std::invoke`). The result is a long error message that points at code you didn't write.

### The concept way

`std::invocable<F, Args...>` is a concept that is satisfied when `std::invoke(f, args...)` would compile. Putting it on the template moves the error to the **call site** with a clear message:

```c++
#include <concepts>
#include <functional>
#include <iostream>

template<std::invocable F>   // F must be callable with no arguments
void run(F f) {
    std::invoke(f);
}

int main() {
    run([]() { std::cout << "Hello" << std::endl; });
    // Output:
    // Hello

    // run(42);
    // ERROR: no matching function for call to 'run'
    //        because 'int' does not satisfy 'invocable'
}
```

There are three equivalent ways to write the constraint. Pick the one that reads best:

```c++
// 1. Constrained template parameter
template<std::invocable<int> F>
void each(F f);

// 2. requires clause: handy when combining several conditions
template<typename F>
    requires std::invocable<F, int>
void each(F f);

// 3. Abbreviated syntax with auto (shortest)
void each(std::invocable<int> auto f);
```

## Putting It All Together: A `retry` Helper

This helper calls any callable up to `attempts` times and returns whatever the callable returns:

- `std::invocable` makes sure only callables are accepted.
- `std::invoke_result_t` names the return type.
- `std::invoke` performs the call.

```c++
#include <concepts>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <type_traits>

template<std::invocable F>
std::invoke_result_t<F> retry(F f, int attempts) {
    for (int i = 1; i < attempts; ++i) {
        try {
            return std::invoke(f);
        } catch (const std::exception& e) {
            std::cout << "Attempt " << i << " failed: " << e.what() << std::endl;
        }
    }
    return std::invoke(f); // Last attempt: let the exception reach the caller
}

int main() {
    int calls = 0;

    // Simulates a flaky network call that fails twice, then succeeds
    auto fetchValue = [&calls]() {
        if (++calls < 3) {
            throw std::runtime_error("connection lost");
        }
        return 42;
    };

    int result = retry(fetchValue, 5); // std::invoke_result_t<F> is int
    std::cout << "Result: " << result << std::endl;
    // Output:
    // Attempt 1 failed: connection lost
    // Attempt 2 failed: connection lost
    // Result: 42
}
```

## When to Use What?

1. **Need the result type?** Use `std::invoke_result_t<F, Args...>`, or simply `auto` when the return type doesn't have to be named.
2. **Storing a callable for later?** Use [std::move_only_function](./std_move_only_function.md) (or [std::function](./std_function.md) when it must be copyable).
3. **Calling a member function generically?** Use `std::invoke` instead of `.*` / `->*`.

## `std::is_invocable`

`std::is_invocable` answers one question at compile time: *can this thing be called with these arguments?* It doesn't call anything. It looks at the types and gives you a `true` or `false` to branch on or assert on.

"Called" follows the rules of `std::invoke`, so it covers functions, lambdas, function objects, and member pointers like `&Item::empty`.

| Trait | True when |
|---|---|
| `std::is_invocable_v<F, Args...>` | `F` can be called with `Args...` |
| `std::is_invocable_r_v<R, F, Args...>` | ...and the result to `R` |
| `std::is_nothrow_invocable_v<F, Args...>` | ...and the call is `noexcept` |

#### Choose behavior with `if constexpr`

A fallback that can be either a plain value or a function that computes it, so expensive defaults are only built when needed:

```cpp
template <typename T, typename Fallback>
T value_or(const std::optional<T>& opt, Fallback fallback) {
    if (opt) 
        return *opt;
    if constexpr (std::is_invocable_v<Fallback>)
        return fallback();
    else
        return fallback;
}

std::optional<std::string> name = find_name(id);

value_or(name, "unknown"); // Plain value
value_or(name, [] { return load_default_name(); }); // Only called when name is empty
```

`return fallback;` never has to compile when `fallback` is a lambda.