# References

## What is a reference?

The core idea behind references is to work directly with an existing object without creating a copy. You create a reference using the `&` symbol in the declaration. This is useful when you want to pass large objects to a function without copying them, or when you want to modify the original object within the function.

```c++
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    int x = 10, y = 20;
    swap(x, y);
    std::cout << "x: " << x << ", y: " << y << std::endl; // Output: x: 20, y: 10
}
```

### Key Characteristics of References

#### 1. Must be initialized

 A reference must be initialized to refer to an existing object when it is declared. It cannot be `null` or uninitialized.

```c++
int num = 10;
int &refNum = num; // refNum is now an alias for num
// int &refNum;  // Error! Reference must be initialized.
```

#### 2. Cannot be reassigned

Once a reference is bound to an object, it cannot be changed to refer to a different object. It is a permanent alias for its entire lifetime.

```c++
int num1 = 10;
int num2 = 20;
int &refNum = num1;
refNum = num2;  // This does NOT make refNum refer to num2.
                // It assigns the VALUE of num2 to num1.
                // num1 is now 20.
```

#### 3. Implicitly dereferenced

You do not need a special operator to access the value of the object a reference refers to. It's used just like the original variable.

```c++
int value = 5;
int &refValue = value;
refValue = 15;  // Modifies 'value' directly
std::cout << value << std::endl; // Output: 15
```

## Lvalue and Rvalue References

In modern C++, expressions are categorized as either an **lvalue** or an **rvalue**. Understanding this distinction is crucial for understanding move semantics and performance.

- **Lvalue (Locator Value)**: An expression that refers to an object with a stable memory location. Because it has a location, you can take its address (`&`). It persists beyond a single expression.
  - **Examples**: Variables like `int x`; or array elements like `arr[0]`.
- **Rvalue (Read Value)**: An expression that is temporary and does not have a stable memory location. It's a value that's about to disappear. You cannot take its address.
  - **Examples**: Literals like `42`, the result of an arithmetic operation like `x + 5`, or a temporary object returned by a function.

The references you've seen so far are **lvalue references** (`&`). They can only bind to lvalues.

C++11 introduced the **rvalue reference** (`&&`), which is designed to bind to temporary objects (rvalues). Its primary purpose is to identify objects that are about to be destroyed so we can safely "steal" their resources instead of copying them.

```c++
void processData(const std::string& str) { // const lvalue reference
    std::cout << "Lvalue processed: " << str << std::endl;
}

void processData(std::string&& str) { // rvalue reference
    std::cout << "Rvalue processed, can be moved from: " << str << std::endl;
}

int main() {
    std::string var = "my variable";
    processData(var);                 // Calls the lvalue reference overload
    processData("temporary string");  // Calls the rvalue reference overload
    processData(std::move(var));      // std::move casts var to an rvalue, forcing the rvalue overload
}
```

> [!NOTE]  
> The `std::string&& parameter` is not `const` because the entire purpose of an rvalue reference overload is to **enable modification** of the source object. It signals: "I've been given a temporary object that's about to be destroyed, so I am allowed to tear it apart and steal its internal resources for my own use." This "stealing" is called a **move operation**, and it is inherently a mutating action.

### Why do we care? For Optimization.

Rvalue references allow us to create function overloads that treat lvalues and rvalues differently. For an lvalue, we must copy it. For an rvalue, we can steal its data because we know it's a temporary that's about to be discarded anyway.

```c++
class Buffer {
public:
    // Copy constructor (for lvalues)
    Buffer(const Buffer& other) {
        // Expensive: Allocate new memory and copy data
        std::cout << "Copy constructor called.\n";
    }

    // Move constructor (for rvalues)
    Buffer(Buffer&& other) noexcept {
        // Cheap: "Steal" the pointer and nullify the temporary source.
        std::cout << "Move constructor called.\n";
        this->data = other.data;
        other.data = nullptr;
    }
    
    int* data;
};

Buffer b1;
Buffer b2 = b1;            // Calls copy constructor (b1 is an lvalue)
Buffer b3 = std::move(b1); // Calls move constructor (std::move casts b1 to an rvalue)
Buffer b4 = Buffer();      // Calls move constructor (Buffer() is a temporary rvalue)
```

## Other Use Cases 

### Range-Based for Loops

References can be used in range-based for loops to modify elements of a container directly.

```c++
std::vector<int> numbers = {1, 2, 3, 4, 5};

for (int &num : numbers) {
    num *= 2; // Doubles each element in the vector
}
```

### Const References

To prevent accidental modifications, you can create const references. This ensures that the reference cannot be used to change the value of the object it refers to.

```c++
void printValue(const int &value) {
    std::cout << value << std::endl;
    // value = 10; // Error! Cannot modify a const reference
}
```

`const` references are great for passing objects to functions when you only need to read the object's data and want to guarantee that the function won't change it.

## Pitfalls

### Dangling References From Functions

Never return a reference to a local variable. The variable is destroyed when the function exits, leaving the caller with a **dangling reference** that points to invalid memory. Accessing it is undefined behavior.

```c++
// DANGER: DO NOT DO THIS!
int& createAnInteger() {
    int local_val = 10;
    return local_val; // Returning a reference to local_val
} // 'local_val' is destroyed here.

int main() {
    int& dangling_ref = createAnInteger();
    // 'dangling_ref' now points to garbage memory.
    std::cout << dangling_ref << std::endl; // UNDEFINED BEHAVIOR (might crash, might print garbage)
}
```