# `std::unordered_multiset`

### Characteristics

- **Unordered Container**: Unlike its ordered counterpart `std::multiset`, elements in an `std::unordered_multiset` are not sorted. They are organized into buckets based on hash values.

- **Allows Duplicates**: Just like `std::multiset`, it allows multiple instances of equal elements.

- **Dynamic Size**: The container can grow or shrink dynamically.

- **Hash-Based**: It uses a hash table for underlying storage, which usually allows for faster look-up times at the expense of ordering.

- **Average Constant Time Complexity**: For most operations like insertion, deletion, and search, the time complexity is `O(1)` on average.
## Basic usage

```c++
#include <iostream>
#include <unordered_set>

int main() {
    std::unordered_multiset<int> numbers;

    // Insert elements
    numbers.insert(5);
    numbers.insert(5);  // Duplicates allowed
    numbers.insert(3);

    // Print the elements
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    // Output could be: 5 5 3

    // Remove an element
    numbers.erase(numbers.find(5));  // Removes one instance of 5

    return 0;
}
```

## When to Use `std::unordered_multiset`

- When you need a collection of elements where duplicates are allowed.
- When you don't need the elements to be sorted.
- When you want average constant-time complexity for look-ups, additions, and deletions.
## Limitations

- No direct access by index.
- All elements must be hashable, meaning you have to provide a hash function for custom types.
## Common Functions

### Element Management

- `insert(value)`: Inserts a value.
- `emplace(args)`: Constructs and inserts a value in-place.
- `erase(value)`: Removes all instances of a specific value.
- `clear()`: Clears all elements.
### Element Lookup

- `find(value)`: Returns an iterator to the first occurrence of the value, or `end()` if not found.
- `count(value)`: Returns the number of occurrences of a value.
### Set Properties

- `size()`: Returns the number of elements.
- `empty()`: Checks if the set is empty.
### Iteration

- `begin()`, `end()`: Provide iterators to traverse through the elements.