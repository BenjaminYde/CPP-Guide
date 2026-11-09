# `std::move_only_function`: `std::function` for Callables That Can't Be Copied (C++23)

`std::function` is great at storing "something callable", but it has one hidden requirement: **the callable must be copyable**. That sounds harmless until you capture something that can only be *moved*, like a `std::unique_ptr`, a file handle, a socket or a `std::packaged_task`.

C++23 fixes this with `std::move_only_function`, available in the `<functional>` header. It is the same idea as `std::function`, but the wrapper itself is move-only, so it happily stores move-only callables.

> **Prerequisites:** [std::function](./std_function.md), [Lambdas](./lambdas.md) (especially init-captures like `[p = std::move(ptr)]`), [Copy and Move Semantics](../oop/copy_and_move.md) and [Smart Pointers](../memory/smart_pointers.md).

## The Problem: `std::function` Requires a Copyable Callable

A lambda that captures a `std::unique_ptr` by move becomes move-only itself, because its hidden member (the `unique_ptr`) can't be copied.

### How NOT to do it

```c++
#include <functional>
#include <iostream>
#include <memory>

int main() {
    auto ptr = std::make_unique<int>(42);
    // The lambda now OWNS the unique_ptr, so the lambda is move-only.
    auto lambda = [p = std::move(ptr)]() { std::cout << *p << '\n'; };
    // ERROR: std::function requires the lambda to be copyable, even if it's never copied
    std::function<void()> f = std::move(lambda);
}
```

### Workarounds

#### 1. Keep ownership outside, capture a raw pointer

The `unique_ptr` stays where it is, and the lambda just borrows it. `p` must outlive `f`, otherwise `raw` dangles.

```c++
auto p = std::make_unique<int>(42);
// OK, copying a pointer is fine
std::function<void()> f = [raw = p.get()]() { std::cout << *raw << '\n'; };
```

#### 2. Let the lambda own it, switch to `shared_ptr`

Use this when the lambda must keep the object alive on its own.

```c++
auto p = std::make_shared<int>(42);
// OK, copies share ownership
std::function<void()> f = [p]() { std::cout << *p << '\n'; };
```

## The Solution: `std::move_only_function`

Simply swap the type. The syntax is identical to `std::function`:
`std::move_only_function<return_type(parameter_types)>`

```c++
#include <functional>
#include <iostream>
#include <memory>

int main() {
    auto ptr = std::make_unique<int>(42);

    std::move_only_function<void()> f = [p = std::move(ptr)]() {
        std::cout << "Value: " << *p << std::endl;
    };

    f(); // Output: Value: 42

    // The wrapper itself is move-only too.
    // std::move_only_function<void()> copy = f;                  // ERROR: no copy constructor
    std::move_only_function<void()> owner = std::move(f); // OK: ownership is transferred

    owner(); // Output: Value: 42

    if (!f)
        std::cout << "f is empty after the move" << std::endl;
}
```

Everything you know from `std::function` still works: it can hold regular functions, lambdas and functors, you can check it with `if (f)` and compare it with `nullptr`.

## Passing a `std::move_only_function` by value

Passing by value can only move, never copy. A named variable must be moved in explicitly; a temporary is moved automatically.

```c++
void run(std::move_only_function<void()> f) { f(); }

int main() {
    std::move_only_function<void()> task = [p = std::make_unique<int>(42)]() { std::cout << *p << '\n'; };

    run(task); // ERROR: task is a named variable, so this would be a copy
    run(std::move(task)); // OK, task is moved into the parameter and is now empty
    run([]() { std::cout << "temporary\n"; }); // OK, a temporary is moved automatically
}
```

## Practical Example: A Task Queue

The most common place you will meet `std::move_only_function` is a **queue of work** to run later, for example in an event loop or a thread pool. The tasks often own resources, so they need to be moved into the queue.

```c++
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class TaskQueue {
public:
    // Take the task by value and move it into the vector.
    void push(std::move_only_function<void()> task) {
        tasks.push_back(std::move(task));
    }

    void runAll() {
        for (auto& task : tasks)
            task();
        tasks.clear();
    }

private:
    std::vector<std::move_only_function<void()>> tasks;
};

int main() {
    TaskQueue queue;

    // A task that owns a heap-allocated buffer
    auto buffer = std::make_unique<std::string>("log line 1");
    queue.push([b = std::move(buffer)]() {
        std::cout << "Flushing: " << *b << std::endl;
    });

    // A plain, copyable lambda works too
    queue.push([]() { std::cout << "Simple task" << std::endl; });

    queue.runAll();
    // Output:
    // Flushing: log line 1
    // Simple task
}
```

## The Verdict: When to Use What?

1. **Default to `std::move_only_function` when you store a callable you own:** callback members, task queues and "run this later" APIs. It accepts everything `std::function` accepts, plus move-only callables.
2. **Use `std::function` when copies are required:** for example, when the same callback must be handed to several places, or when you are limited to C++20 or older.
3. **Use function pointers for C interoperability**, as described in [Function Pointers](./function_pointers.md).Why