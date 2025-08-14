# Copy and Move Semantics

In C++, the way we transfer the value of an object from one instance to another is fundamental to writing correct and efficient code. This is governed by two complementary mechanisms: copy semantics (making a deep, independent duplicate) and move semantics (efficiently transferring resource ownership). A deep understanding of both is essential for mastering resource management under the RAII paradigm.

## Copy Semantics: Creating Clones

Copying creates a new object that is a complete and independent duplicate of an existing object. Any subsequent changes to the copy do not affect the original, and vice-versa.

### Shallow vs. Deep Copy:

- **Shallow Copy (The Problem)**: When `object1` is destroyed, it deletes the memory. When `object2` is destroyed, it tries to delete the same memory again. This is a **double free** error and causes undefined behavior.
- **Deep Copy (The Solution)**: Each object manages its own independent resource.

### Copy Constructor

Called when a new object is initialized from an existing object of the same type.

- **Signature**: `ClassName(const ClassName& other)`
- **Core Task**: Perform a "deep copy" of all resources. If the object manages a pointer, it must allocate new memory and copy the contents.

> [!NOTE]
> The parameter must be a reference to avoid infinite recursion; if it were passed by value, the copy constructor would need to be called to create the parameter, which would in turn call the copy constructor, and so on.

This is called when a new object is created as a copy of an existing one (e.g., `ClassName object1 = object2;`).

### Copy Assignment Operator

Called when an existing object is assigned the value of another existing object.

- **Signature**: `ClassName& operator=(const ClassName& other)`
- **Core Task**: This function makes an existing object a deep copy of another existing object. It must handle cleaning up its old resource and protecting against self-assignment. The copy-and-swap idiom is a robust way to implement this.

This is called when you assign an existing object to another existing object (e.g., `object1 = object2;`)

### Example: `SimpleString` with Copy Semantics

```c++
#include <cstring>
#include <algorithm> // For std::swap
#include <iostream>

class SimpleString {
public:
    // Parameterized constructor
    SimpleString(const char* s = "") 
        : size_(std::strlen(s)), data_(new char[size_ + 1]) {
        std::memcpy(data_, s, size_ + 1);
        std::cout << "Constructor for '" << data_ << "'\n";
    }

    // 1. Copy Constructor
    SimpleString(const SimpleString& other)
        : size_(other.size_), data_(new char[size_ + 1]) {
        std::memcpy(data_, other.data_, size_ + 1);
        std::cout << "COPY CONSTRUCTOR from '" << other.data_ << "'\n";
    }

    // 2. Copy Assignment Operator
    SimpleString& operator=(const SimpleString& other) {
        std::cout << "COPY ASSIGNMENT from '" << other.data_ << "'\n";
        // Self-assignment check
        if (this == &other) {
            return *this;
        }

        // Allocate new memory before changing state (strong exception guarantee)
        char* new_data = new char[other.size_ + 1];
        std::memcpy(new_data, other.data_, other.size_ + 1);

        // Release old resource
        delete[] data_;

        // Assign new state
        data_ = new_data;
        size_ = other.size_;
        
        return *this;
    }

    ~SimpleString() {
        std::cout << "Destructor for '" << (data_ ? data_ : "") << "'\n";
        delete[] data_;
    }

private:
    size_t size_;
    char* data_;
};
```

Usage:

```c++
SimpleString s1("Hello");     // Constructor
SimpleString s2 = s1;         // Calls COPY CONSTRUCTOR
SimpleString s3("World");     // Constructor
s3 = s1;                      // Calls COPY ASSIGNMENT
```

## Move Semantics: Efficient Resource Transfer

Move semantics were introduced in C++11 to avoid expensive deep copies when the source object is a temporary (an rvalue) that is about to be destroyed anyway. Instead of copying, we "steal" its resources.

### Lvalues, Rvalues, and `std::move`

- **Lvalue**: An expression that refers to a memory location and can appear on the left-hand side of an assignment (e.g., `s1`, `*p`). You can take its address.

- **Rvalue**: An expression that is temporary and cannot have its address taken (e.g., the return value of a function `SimpleString("temp")`, `x + y`).

- `std::move`: This function does not move anything. It is a cast that unconditionally converts its argument into an rvalue reference `(&&),` signaling "I give you permission to treat this lvalue as a temporary; you may cannibalize it."

### Move Constructor

Called to initialize a new object by "stealing" resources from a temporary (rvalue) object.

- **Signature**: `ClassName(ClassName&& other) noexcept`
- **Purpose**: Move operations are significantly faster than copy operations for objects that manage large resources because they simply transfer ownership of the resource instead of performing a deep copy.
  - **Memory Impact**: Instead of allocating new memory, it simply copies the pointer from the source object. The new object now points to the same memory block the source object was managing. No new memory is allocated for the data, making this operation extremely fast. The source object's pointer is then nullified.
- **How it works**: It performs a shallow copy of the pointer and then nullifies the source pointer. This is a very fast operation that just involves moving an address, not allocating and copying large amounts of data.
- **Rules**:
  - Copy pointers/handles from `other` to `*this`.
  - Set the pointers/handles in `other` to a null or empty state. This prevents other's destructor from releasing the resources now owned by `*this`.
  - Must be noexcept: This is a critical guarantee. Standard library containers like `std::vector` check this. If a move constructor can throw, `std::vector` will use the (slower) copy constructor instead when reallocating, to maintain its strong exception guarantee.

### Move Assignment Operator

Called when an existing object is assigned the value of a temporary (rvalue) object.

- **Signature**: `ClassName& operator=(ClassName&& other) noexcept`
- **Memory Impact**: First, it frees its own memory. Then, it takes over ownership of the existing memory block from the source object. Again, no new memory is allocated for the data itself.
- **Core Task**:
  - Release any resources the current object (`*this`) is holding.
  - Steal the resources from `other`.
  - Leave `other` in a valid, destructible state.
  - Return a reference to `*this`.

```c++
#include <utility>   // For std::move
#include <cstddef>   // For size_t

// A simple class that manages a dynamic memory resource.
class Buffer {
public:
    // Constructor allocates memory.
    explicit Buffer(size_t size)
        : m_data(new int[size]), m_size(size) {}

    // Destructor frees the allocated memory.
    ~Buffer() {
        delete[] m_data;
    }

    // --- Move Semantics ---

    // Move Constructor: Steals the pointer and nullifies the source.
    Buffer(Buffer&& other) noexcept
        : m_data(other.m_data), m_size(other.m_size) {
        // Leave the source object in a valid, empty state.
        other.m_data = nullptr;
        other.m_size = 0;
    }

    // Move Assignment Operator
    Buffer& operator=(Buffer&& other) noexcept {
        // Self-assignment check.
        if (this != &other) {
            // Free the existing resource this object holds.
            delete[] m_data;

            // Steal the resources from the source object.
            m_data = other.m_data;
            m_size = other.m_size;

            // Leave the source object empty.
            other.m_data = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    // For simplicity, explicitly prevent copying.
    Buffer(const Buffer& other) = delete;
    Buffer& operator=(const Buffer& other) = delete;

private:
    int* m_data;
    size_t m_size;
};


int main() {
    // --- 1. Move Constructor Demonstration ---
    
    // `source` allocates a resource.
    Buffer source(100);

    // The move constructor is called. `destination` takes ownership of the
    // resource originally held by `source`. This is a fast pointer swap.
    // `source` is now empty.
    Buffer destination = std::move(source);

    // --- 2. Move Assignment Demonstration ---

    // `another_source` allocates a new resource.
    Buffer another_source(50);

    // The move assignment operator is called.
    // 1. The memory `destination` was holding is freed.
    // 2. `destination` takes ownership of `another_source`'s resource.
    // 3. `another_source` is now empty.
    destination = std::move(another_source);

    return 0; // Destructors are called. Deleting nullptr is safe.
}
```

- The move constructor transfers ownership of the `data` pointer from `other` to the new object.
- It sets `other.data` to `nullptr`, ensuring that the destructor of `other` doesn't delete the transferred memory.

## Real-World Use Cases & Memory Impact

### Returning Large Objects from Functions

Returning by value is now highly efficient because it uses move semantics.

```c++
std::vector<std::string> load_data_from_database() {
    std::vector<std::string> data;
    // ... load thousands of strings ...
    return data; // This is a move, not a copy
}

int main() {
    // The move constructor is called, stealing the internal memory buffer
    // of the temporary vector returned from the function.
    // No expensive memory allocation or element-by-element copy occurs.
    std::vector<std::string> all_data = load_data_from_database();
}
```

## References

- https://cbarrete.com/move-from-scratch.html