# `std::thread`

C++11 introduced native support for multi-threading via the Standard Library. Prior to this, you would need to use platform-specific libraries like Pthreads on Unix or Win32 threads on Windows. The `<thread>` header includes classes and functions required to create and manage threads.

To create a new thread, you usually construct a `std::thread` object and provide the function you want to run in the thread as an argument.

```c++
#include <iostream>
#include <thread>

void print_hello() {
    std::cout << "Hello from thread" << std::endl;
}

int main() {
    std::thread t1(print_hello);  // Create a thread and run print_hello
    t1.join();  // Wait for the thread to finish
    return 0;
}
```

The `std::thread` class has several constructors, but the most commonly used one takes a function to execute as its argument. You can also pass additional arguments that will be forwarded to the function.

```c++
std::thread t1(my_function, arg1, arg2);
```
###  Member Functions

- `join()`: When you `join` a thread, you're telling the operating system that you want the calling thread (often the main thread) to wait for the thread represented by the `std::thread` object to finish its task before moving on.

	**Advantages:**
	
	- Guarantees that the thread will finish its execution.
	- Any resources used by the thread are cleaned up.
	
	**Drawbacks:**
	
	- Blocks the calling thread, which might not be ideal in real-time systems or responsive applications.

- `detach()`: This function separates the thread from the `std::thread` object, allowing the thread to execute independently. The resources of the detached thread will be released when it terminates. 

	**Advantages:**
	
	- Does not block the calling thread.
	- Useful for "fire-and-forget" tasks.
	
	**Drawbacks:**
	
	- You can't guarantee when the thread will finish its work.
	- If main thread finishes executing while the detached thread is still running, the program will terminate, and the detached thread might not complete its work.
		- The behavior of detached threads when the main program terminates is platform-dependent, but generally, all threads are terminated when the main program ends, regardless of whether they've been detached or not.

- `joinable()`: member function checks whether the thread object is associated with an active thread of execution. Essentially, it tells you whether you can join or detach the thread without encountering a runtime error.

	**What makes a thread joinable?**
	
	- A thread is joinable when it represents a valid, active thread.
	- A thread is _not_ joinable if it was just constructed without a function or if it has been moved from.
	- A thread becomes non-joinable once `join()` or `detach()` has been called on it.

	It's a good practice to check whether a thread is joinable before calling `join()` or `detach()` to avoid unnecessary runtime errors. This is especially useful in more complex codebases where the state of a thread object might not be immediately clear.

- `hardware_concurrency()`: A static member function that returns the number of concurrent threads supported by the system. Useful for deciding how many threads to create for optimal performance.

- `get_id()`: Returns the ID of the thread.

```c++
#include <iostream>
#include <thread>

void my_function() {
    std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
}

int main() {
    std::thread t1(my_function);
    std::cout << "Thread t1 ID: " << t1.get_id() << std::endl;

    t1.join();
    return 0;
}
```

### Example

Here's a simple example to demonstrate some of these aspects:

```c++
#include <iostream>
#include <thread>
#include <vector>

void print_nums(int start, int end) {
    for (int i = start; i < end; ++i) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

int main() {
    // Get the number of hardware threads available
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << "Number of hardware threads: " << n << std::endl;

    // Create a vector to hold the thread objects
    std::vector<std::thread> threads;

    // Launch multiple threads
    for (int i = 0; i < n; ++i) {
        threads.push_back(std::thread(print_nums, i * 10, (i + 1) * 10));
    }

    // Wait for all threads to complete
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}
```