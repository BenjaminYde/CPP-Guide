# `std::function`: The Modern, Type-Safe Callable Wrapper

The power and the pain of function pointers are fast and C-compatible but have clunky syntax, can't hold state, and require unsafe `void*` tricks to pass user data.

Modern C++ provides a beautiful solution to all these problems with `std::function`. It is a general-purpose, polymorphic function wrapper available in the `<functional>` header.

Think of `std::function` as a "smart" function pointer. It can hold, copy, and invoke **any callable object**. Not just function pointers, but also lambdas.

## The Core Idea: One Wrapper to Rule Them All

The problem with raw pointers is that a `void (*)(int)` is a completely different, incompatible type from a `void (*)(double)`. `std::function` solves this by "erasing" the specific type of the callable it holds, storing it in a generic, type-safe wrapper.

The syntax is clean and descriptive:
`std::function<return_type(parameter_types)>`

```c++
#include <iostream>
#include <functional>
#include <string>

// A regular function
void printMessage(const std::string& msg) {
    std::cout << "Regular function: " << msg << std::endl;
}

// A function object (functor)
struct Divider {
    double operator()(int a, int b) {
        return static_cast<double>(a) / b;
    }
};

int main() {
    // 1. Declare a std::function that can hold any callable
    //    taking a const string& and returning void.
    std::function<void(const std::string&)> p_print;

    // Assign a regular function to it
    p_print = printMessage;
    p_print("Hello World!"); // Call it like a normal function

    // 2. Declare a std::function for a different signature
    std::function<double(int, int)> p_calculate;

    // Assign a lambda to it
    p_calculate = [](int x, int y) { return x * y; };
    std::cout << "Lambda result: " << p_calculate(10, 5) << std::endl;

    // Assign a function object (functor) to it
    Divider divider;
    p_calculate = divider;
    std::cout << "Functor result: " << p_calculate(10, 4) << std::endl;
}
```

Notice how the same `p_calculate` object first held a lambda and then a functor. This flexibility is the superpower of `std::function`.

## The Killer Feature: Type-Safe State

Remember the unsafe `void*` trick we used to pass user data to a C-style callback? Lambdas and `std::function` make that pattern obsolete.

A lambda can **capture** variables from its enclosing scope. When you store that lambda in a `std::function`, the captured state is carried along with it, safely and transparently.

Let's refactor our old `void*` example into modern, safe C++.

### The Old, Unsafe Way (for comparison)

```c++
void on_button_click(void* data) {
    UserData* userData = static_cast<UserData*>(data); // Unsafe cast!
    userData->clickCount++;
}
```

### The New, Modern, and Safe Way

```c++
#include <functional>

struct UserData {
    std::string name;
    int clickCount = 0;
};

// This function now takes a std::function. No more void*!
void register_callback(std::function<void()> callback) {
    // Imagine an event happens... now we just call the function.
    // We don't need to know anything about user data.
    std::cout << "Event occurred, executing callback..." << std::endl;
    callback();
}

int main() {
    UserData myData;
    myData.name = "Alex";

    // We create a lambda that captures `myData` by reference.
    // The lambda "remembers" myData.
    auto my_callback = [&myData]() {
        myData.clickCount++;
        std::cout << myData.name << " has now clicked " 
                  << myData.clickCount << " times." << std::endl;
    };

    // Pass the lambda to the function.
    // The state (myData) is carried with it, 100% type-safely.
    register_callback(my_callback);
    register_callback(my_callback);
}
```

No casts, no `void*`, no danger. The lambda's capture list `[&myData]` handles everything. This is the idiomatic way to handle stateful callbacks in modern C++.

## Simplifying Member Functions

`std::function` also massively simplifies calling member functions. You no longer need the awkward `.*` or `->*` syntax. You can use a lambda to capture the object instance (`this`).

```c++
class Wallet {
public:
    void addMoney(int amount) {
        balance += amount;
        std::cout << "Added " << amount << ", new balance is " << balance << std::endl;
    }
private:
    int balance = 0;
};

int main() {
    Wallet myWallet;
    
    // Create a std::function that takes an int and returns void
    std::function<void(int)> p_operation;

    // Assign a lambda that captures `myWallet` by reference and calls its member function.
    p_operation = [&myWallet](int value){
        myWallet.addMoney(value);
    };

    // Now just call it like a regular function
    p_operation(50); // Calls myWallet.addMoney(50)
    p_operation(20); // Calls myWallet.addMoney(20)
}
```

## Copying vs. Moving a `std::function`

A `std::function` **owns** the callable it holds, *including everything the lambda captured*. That has a big consequence:

- **Copying** a `std::function` copies the callable and all of its captured state. If the captures are large (a `std::vector`, a `std::string`, ...) this can mean heap allocations.
- **Moving** a `std::function` hands the stored callable over to the new object. Nothing is duplicated, it is usually just a few pointers being swapped.

The difference is not only about performance. With a lambda, a copy gives you **two independent states**:

```c++
#include <functional>
#include <iostream>
#include <utility>

int main() {
    // A lambda with its own internal counter (init-capture + mutable)
    std::function<int()> counter = [count = 0]() mutable { return ++count; };
    counter(); // 1
    counter(); // 2

    // COPY: the copy starts with its own count of 2
    std::function<int()> copy = counter;
    std::cout << copy() << std::endl;    // Output: 3
    std::cout << counter() << std::endl; // Output: 3  -> two separate counters!

    // MOVE: the state itself is handed over, there is still only one counter
    std::function<int()> moved = std::move(counter);
    std::cout << moved() << std::endl;   // Output: 4
}
```

> **Rule of thumb:** If there should only be *one* owner of a callback, move it. Copy it only when you really want an independent duplicate.

## How to Use Move

```c++
#include <functional>
#include <iostream>
#include <string>
#include <utility>

class Button {
public:
    // By value: the parameter is a fresh object that this function owns...
    void setOnClick(std::function<void()> callback) {
        // ...so we can move it into the member instead of copying a second time.
        onClick = std::move(callback);
    }

    void click() const {
        if (onClick) // Calling an empty std::function throws std::bad_function_call
            onClick();
    }
private:
    std::function<void()> onClick;
};

int main() {
    Button button;
    std::string user = "Alex";

    // 1. Temporary lambda: moved into the parameter, then moved into the member. No copies.
    button.setOnClick([user]() { std::cout << user << " clicked!" << std::endl; });
    button.click(); // Output: Alex clicked!

    // 2. Named std::function we still want to use afterwards: copied once (on purpose).
    std::function<void()> logClick = []() { std::cout << "Click logged" << std::endl; };
    button.setOnClick(logClick);
    logClick(); // Still usable. Output: Click logged

    // 3. Named std::function we don't need anymore: move it in. No copies.
    button.setOnClick(std::move(logClick));
    button.click(); // Output: Click logged
    // From here on, don't use logClick anymore (see "The Dangers of Move").
}
```

**When to use `std::move` on a `std::function`:**

- When **storing** a callback you received by value.
- When **transferring ownership** into another object, and you don't need it yourself anymore.

## Passing a `std::function` by Reference

Passing by value means the callee gets its **own** `std::function`. With a reference or a pointer, the callee only gets **access to someone else's** `std::function`. Nothing is copied, but now the callee depends on an object it doesn't own. Let's look at what happens in each case.

### `const std::function<...>&`: Fine for calling, dangerous for storing

For a function that only *calls* the callback, a `const&` is fine:

```c++
#include <functional>
#include <iostream>

void runTwice(const std::function<void()>& callback) {
    callback();
    callback();
}

int main() {
    runTwice([]() { std::cout << "Hi" << std::endl; });
    // Output:
    // Hi
    // Hi
}
```

But notice: `runTwice` expects a `std::function`, and we passed a **lambda**. The compiler silently creates a **temporary** `std::function` that only lives until the end of the statement (the `;`). That is harmless while `runTwice` is running, but it becomes a disaster when the reference (or its address) is **stored**:

```c++
#include <functional>
#include <iostream>
#include <string>

class Button {
public:
    // Looks efficient: "no copy"...
    explicit Button(const std::function<void()>& callback) : onClick(callback) {}

    void click() const { onClick(); }

private:
    const std::function<void()>& onClick; // BAD: a reference to someone else's std::function
};

int main() {
    std::string user = "Alex";
    // A temporary std::function is created from the lambda and destroyed at the ';'
    Button button([user]() { std::cout << user << " clicked!" << std::endl; });
    button.click(); // Undefined behavior: onClick refers to a destroyed object
}
```

This compiles **without a single warning** on GCC and Clang. What happened when we ran it:

- The "string" it printed was garbage memory.
- AddressSanitizer (`-fsanitize=address`) reports `stack-use-after-scope`.

That is undefined behavior at its worst: it can crash, print garbage or *appear to work*. **Never store a reference or pointer to a callback parameter.** If you need to keep it, take it by value and move it.

### `std::function<...>&` (non-const): The callee can steal your callback

A non-const reference can't bind to a temporary, so passing a lambda directly doesn't even compile. That is the "good" news. The bad news is what happens with a named `std::function`: the callee has full write access to **your** object and can move out of it.

```c++
#include <functional>
#include <iostream>
#include <utility>

std::function<void()> storage;

void registerCallback(std::function<void()>& callback) {
    storage = std::move(callback); // Takes the caller's callback away
}

int main() {
    std::function<void()> myCallback = []() { std::cout << "Hello" << std::endl; };
    registerCallback(myCallback); // Looks like a normal call...

    if (!myCallback) {
        std::cout << "myCallback is empty now!" << std::endl;
    }
    // Output: myCallback is empty now!
}
```

The call silently empties `myCallback`. A by-value parameter makes the caller write `std::move`, so the hand-over is visible.

Use a non-const reference only when changing the caller's callback is the whole point of the function, and name it so that's obvious:

```c++
#include <functional>
#include <iostream>
#include <utility>

void wrapWithLogging(std::function<void()>& callback) {
    callback = [inner = std::move(callback)]() {
        std::cout << "Calling callback" << std::endl;
        inner();
    };
}

int main() {
    std::function<void()> myCallback = []() { std::cout << "Hello" << std::endl; };
    wrapWithLogging(myCallback); // The name says it changes myCallback

    myCallback();
    // Output:
    // Calling callback
    // Hello
}
```

## Passing a `std::function` by Pointer

### `std::function<...>*`: Possible, but rarely what you want

```c++
#include <functional>
#include <iostream>

void runIfSet(const std::function<void()>* callback) {
    // Two different kinds of "nothing": a null pointer AND an empty std::function
    if (callback != nullptr && *callback) {
        (*callback)();
    }
}

int main() {
    std::function<void()> hello = []() { std::cout << "Hello" << std::endl; };

    runIfSet(&hello);   // Output: Hello
    runIfSet(nullptr);  // Does nothing
    // runIfSet(&[]() {}); // ERROR: can't take the address of a temporary
}
```

A pointer has all the problems of a reference (it can dangle if stored), plus:

- **Clumsy to call:** You can't pass a lambda directly, and you need `(*callback)()`.
- **Two "empty" states:** `nullptr` and an empty `std::function` both mean "no callback", so you must check both.
- **Unclear ownership:** Who deletes it, and how long does it live? The signature doesn't say.

### `std::ref`: Storing a reference *inside* a `std::function`

Sometimes you really do want a `std::function` that **refers to** an existing callable instead of copying it, for example to share one stateful object. `std::ref` (from `<functional>`) wraps the callable in a `std::reference_wrapper`, and `std::function` stores that small wrapper instead of a copy:

```c++
#include <functional>
#include <iostream>

int main() {
    auto counter = [count = 0]() mutable { return ++count; };

    std::function<int()> byCopy = counter;            // Has its own copy of count
    std::function<int()> byRef  = std::ref(counter);  // Refers to the original counter

    byCopy();
    byCopy(); // Only the copy's count changes

    byRef();
    byRef();  // Changes the ORIGINAL counter

    std::cout << counter() << std::endl; // Output: 3  (2 calls via byRef + this one)
}
```

The same lifetime rule applies here: **`byRef` dangles as soon as `counter` is destroyed.** Only use `std::ref` when the original clearly outlives every `std::function` that refers to it.

## The Dangers of Move

`std::move` doesn't move anything by itself. It is only a cast that says "you are allowed to steal from this object". That makes a few mistakes very easy to write and hard to spot.

### Danger 1: Using a moved-from `std::function`

After a move, you can't know what's left in the original. With GCC's library the `std::function` is empty, but with Clang's libc++ it can still hold the lambda. So never use a moved-from object without assigning it a new value first.

```c++
#include <functional>
#include <iostream>
#include <utility>

int main() {
    std::function<void()> greet = []() { std::cout << "Hello" << std::endl; };
    std::function<void()> owner = std::move(greet);

    // greet();  // BAD: either calls something or throws std::bad_function_call. Don't rely on either.

    // OK: assigning a new value makes the moved-from object usable again.
    greet = []() { std::cout << "Hello again" << std::endl; };
    greet(); // Output: Hello again
}
```

Treat a moved-from `std::function` as "dead" until you assign it a new value.

### Danger 2: `std::move` on a `const` object silently copies

A `const` object can't be stolen from, so `std::move` produces a `const&&`, which happily binds to the **copy constructor**. It compiles without a warning and does a full copy:

```c++
#include <functional>
#include <utility>

int main() {
    const std::function<void()> original = []() {};

    std::function<void()> target = std::move(original); // Compiles, but COPIES
    // original is untouched and still holds its callable
}
```

The same applies to `const std::function<...>&` parameters: you can't move out of them. If a function needs to keep the callback, take it **by value** instead. Tools like clang-tidy warn about this (`performance-move-const-arg`).

### Danger 3: Moving doesn't fix dangling captures

Moving a `std::function` moves the *closure*, not the variables it captured **by reference**. If those variables die, the moved callback dangles just like the original would:

```c++
#include <functional>
#include <iostream>

std::function<int()> makeCounter() {
    int count = 0;
    // BAD: captures a reference to a local variable that dies at the end of this function
    return [&count]() { return ++count; };
}

std::function<int()> makeSafeCounter() {
    // GOOD: the lambda owns its own copy of the state
    return [count = 0]() mutable { return ++count; };
}

int main() {
    auto bad = makeCounter();
    // bad(); // Undefined behavior: count no longer exists

    auto good = makeSafeCounter();
    std::cout << good() << std::endl; // Output: 1
}
```

## Why `std::move_only_function` Exists

You may have noticed that `std::function` relies on copying in several places. That is no accident: **`std::function` itself is copyable, so it requires the callable it stores to be copyable too**, even when your code only ever moves it.

So what happens when your lambda *owns* something that can't be copied, like a `std::unique_ptr`? Try to store it in a `std::function` and the compiler simply refuses.

C++23 has an answer for exactly this: **`std::move_only_function`**. Check [std::move_only_function](./std_move_only_function.md).