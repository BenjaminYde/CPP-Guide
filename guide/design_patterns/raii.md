# RAII (Resource Acquisition Is Initialization)

RAII (Resource Acquisition Is Initialization) is a programming idiom in C++ that ties the lifecycle of resources (e.g., memory, file handles, sockets, mutexes) to the lifetime of objects. It's a powerful and elegant way to manage resources, ensuring that they are properly acquired and released without the need for explicit cleanup code scattered throughout your application.

## Why Use RAII? (The Problem It Solves)

Without RAII, managing resources can lead to bugs:

- Forgetting to release a resource (e.g., not closing a file).
- Leaks when exceptions happen or you exit early.
- Code cluttered with cleanup statements.

RAII handles this automatically through C++'s object lifetimes.

## Key Concepts of RAII

- **Acquire in Constructor**:
  - Grab the resource when creating the object (in the constructor).
  - If acquisition fails, throw an exception to avoid half-built objects.
- **Release in Destructor**:
  - Free the resource when the object is destroyed (in the destructor).
  - This happens automatically when the object goes out of scope, even if an exception is thrown.
- **Scope-Based Lifetime**:
  - The resource lives as long as the object is in scope (e.g., inside curly braces `{}`).
  - No manual cleanup needed.

RAII works best with stack-allocated objects, but it can pair with smart pointers for heap resources.

## Examples

### 1. Managing Memory (Dynamic Allocation)

Without RAII, you must manually delete memory, which is error-prone.

#### Bad (Non-RAII):

```c++
void bad() {
    int* ptr = new int(42); // Acquire memory
    if (my_function()) { // Use ptr...
        // Forgot to delete? Leak!
        return;
    }
    delete ptr; // Manual release – easy to forget!
}
```

#### Good (RAII with `std::unique_ptr`):

```c++
#include <memory> 

void good() {
    std::unique_ptr<int> ptr(new int(42)); // Acquire in constructor
    if (my_function()) { // Use ptr...
        return; // No leak! Destructor auto-deletes when scope ends
    }
}  // ptr destroyed here, memory released automatically
```

## 2. Managing Files

Opening files manually risks forgetting to close them, especially with exceptions.

### Bad (Non-RAII):

```c++
#include <fstream>
#include <stdexcept>
#include <string>

void bad(const std::string& filename) {
    std::ifstream file;
    file.open(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    // Read from file...
    if (error_occurred()) {
        // Forgot to close? File handle leaks until program ends!
        return; // BAD: file not closed!
    }

    file.close(); // Manual release, easy to forget!
}
```

### Good

`std::ifstream file(filename);` automatically opens the file during construction.
It’s the heart of RAII — acquire in constructor, release in destructor.

```c++
#include <fstream>
#include <stdexcept>
#include <string>

void good(const std::string& filename) {
    std::ifstream file(filename);  // Acquire in constructor

    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    // Read from file...
    if (error_occurred()) {
        return;  // GOOD: destructor closes file automatically
    }
}  // file closed here, no matter what!
```

### Still using `.open()` and `.close()` and use RAII

`.open()` and `.close()` exist for a reason! They are good when using for performance reasons! For example, a long-lived object (e.g., Logger)

```c++
class FileLogger {
public:
    void open(const std::string& path) {
        file.open(path, std::ios::app);
        if (!file) throw std::runtime_error("log open failed");
    }

    void close() {
        if (file.is_open()) file.close();
    }

    void log(const std::string& msg) {
        if (file.is_open()) {
            file << msg << '\n';
        }
    }

    ~FileLogger() { close(); }  // RAII cleanup

private:
    std::ofstream file;
};
```

```c++
FileLogger logger;
logger.open("app.log");
logger.log("Starting...");
logger.close();  // or let destructor do it
```

## 3. Managing Locks (Mutexes for Thread Safety)

A Mutex is a lock that prevents multiple threads from accessing shared data at once to avoid data races, crashes or corruption. Without RAII, unlocks can be missed.

### Bad (Non-RAII):

```c++
#include <mutex>

std::mutex m;

void bad() {
    m.lock();  // Acquire lock
    // Critical section...
    if (!everything_ok()) {
        return;  // Forgot unlock? Deadlock or issues!
    }
    m.unlock();  // Manual release
}
```

### Good (RAII with `std::lock_guard`):

- `std::lock_guard` is a RAII wrapper for a mutex.  
- **Locks in constructor**, **unlocks in destructor**.  
- **Why RAII?** Cleanup is automatic, even on `return` or exception.  
- **Why use it?** No forgotten `unlock()`, so no deadlocks, no bugs.

```c++
#include <mutex>

std::mutex m;

void good() {
    std::lock_guard<std::mutex> lock(m);  // Acquire in constructor
    if (!everything_ok()) {
        return; // No problem! Destructor unlocks
    }
}  // Unlocked here automatically
```