# Loops

Loops are a fundamental control structure in programming, allowing you to execute a block of code repeatedly. C++ offers several ways to loop, each with its own specific use case, advantages, and potential pitfalls.

## The Classic for Loop

The traditional `for` loop is the most basic and versatile loop, inherited from C. It works by manually managing a counter. It consists of three parts, separated by semicolons:

1. **Initialization**: Executed once at the beginning. Typically declares and initializes a counter variable (`int i = 0`).
2. **Condition**: Checked before each iteration. If it's `true`, the loop body executes. If it's `false`, the loop terminates.
3. **Update**: Executed after each iteration. Typically increments the counter (`i++`).

```c++
#include <iostream>

int main() {
    int numbers[] = {10, 20, 30, 40, 50};
    // The C-style way to get the array size.
    int size = sizeof(numbers) / sizeof(numbers[0]);
    //int size = 5;

    // Classic for loop
    for (int i = 0; i < size; ++i) {
        std::cout << numbers[i] << " ";
    }
}
```

## Range-Based for Loop

Introduced in C++11, the range-based `for` loop (or "for-each" loop) is the preferred way to iterate over an entire container. It's cleaner, safer, and less error-prone because it handles the iteration logic for you. You don't have to manage an index or worry about the container's size.

```c++
#include <iostream>
#include <vector>
#include <string>

int main() {
    std::string words[] = {"peter", "likes", "frozen", "yogurt"};

    // The modern way. No index, no size, no off-by-one errors.
    for (const std::string& word : words) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
}
```

## The `auto` keyword

Because element declaration should have the same type as the array elements, this is an ideal case in which to use the `auto` keyword, and let C++ deduce the type of the array elements for us.

### Loop by Value

In the following for-each example, our element declarations are declared by value:

```c++
std::string array[]{ "peter", "likes", "frozen", "yogurt" };
for (auto element : array) // element will be a copy of the current array element
{
    std::cout << element << ' ';
}
```

- **What it does**: For each iteration, a copy of the container's element is made and stored in element.
- **Pros**: Simple. Changes to element will not affect the original array.
- **Cons**: Inefficient for large objects like strings or complex classes. Copying can be expensive.

### References

Each array element iterated over will be copied into variable element. Copying array elements can be expensive, and most of the time we really just want to refer to the original element. Fortunately, we can use references for this:

```c++
std::vector<int> numbers = {1, 2, 3};
for (auto& num : numbers) {
    num *= 2; // This doubles the values in the original vector.
}
// `numbers` is now {2, 4, 6}
```

- **What it does**: element is a reference to the original element in the container. No copy is made.
- **Pros**: Efficient. It avoids a potentially expensive copy.
- **Cons**: Changes made to element will modify the original container.

### Loop by const Reference

```c++
for (const auto& element : array) { 
    /* ... */ 
}
```

- **What it does**: This is the best of both worlds. element is a reference (so it's efficient), but it's const, so the compiler will give you an error if you try to modify it.

## Loops and pointers

In order to iterate through the array, for-each needs to know how big the array is, which means knowing the array size. Because arrays that have decayed into a pointer do not know their size, for-each loops will not work with them!

```c++
// This function receives a pointer, not an array.
int sumArray(const int array[])
{
    int sum{ 0 };
    for (auto number : array) // COMPILE ERROR: cannot iterate over a pointer.
    {
        sum += number;
    }
    return sum;
}

```

## while loops

### `while` Loop

Use a `while` loop when you don't know the number of iterations beforehand, and the loop should continue as long as a condition is true.

```c++
int i = 0;
while (i < 5) {
    std::cout << i << " "; // Prints 0 1 2 3 4
    i++;
}
```

### `do-while` Loop

This is a less common variant of `while`. The condition is checked after the loop body, which means the body is guaranteed to execute at least once.

```c++
int input;
do {
    std::cout << "Enter a negative number: ";
    std::cin >> input;
} while (input >= 0); // Keep asking until the condition is false.
```

## Loop Control Statements

- `break`: Immediately terminates the innermost loop it's in.
- `continue`: Skips the rest of the current iteration and jumps to the start of the next one.

```c++
for (int i = 0; i < 10; ++i) {
    if (i % 2 != 0) {
        continue; // Skip the odd numbers
    }
    if (i == 8) {
        break; // Stop the loop entirely when i is 8
    }
    std::cout << i << " "; // Prints: 0 2 4 6
}
```