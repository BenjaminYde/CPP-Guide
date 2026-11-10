# `std::async` and `std::future`

## Basic Asynchronous Task

The `std::async` function is a higher-level, more convenient way to launch asynchronous tasks. It runs a function asynchronously (potentially in a new thread) and returns a `std::future` representing the result of that function.

```c++
#include <iostream>
#include <future>

int add(int a, int b) {
    return a + b;
}

int main() {
    auto result = std::async(add, 10, 5);  
    std::cout << "Result: " << result.get() << std::endl; 
    // Output: Result: 15
    return 0;
}
```

#### Waiting for Results

- `get()`: Blocks until the result becomes available.
- `wait()`: Waits for the result to become available, but doesn't retrieve it.
- `wait_for()`: Waits for a specific time duration.
- `wait_until()`: Waits until a specific time point.

### Asynchronous Task with std::launch::deferred

```c++
#include <iostream>
#include <future>
#include <thread>

void deferred_function() {
    std::cout << "Deferred function executed\n";
}

int main() {
    auto handle = std::async(std::launch::deferred, deferred_function);
    std::cout << "Do something else...\n";
    
    handle.get();  // Deferred function will only execute here
    return 0;
}
```

By default, if you don't specify a launch policy, the system is allowed to choose between `std::launch::async` and `std::launch::deferred`.

- **std::launch::async**: If chosen, the `add` function will be executed as soon as `std::async` is called, potentially in a separate thread.

- **std::launch::deferred**: If this policy is chosen, the function will be executed lazily, i.e., it will only be invoked when you call `handle.get()`.