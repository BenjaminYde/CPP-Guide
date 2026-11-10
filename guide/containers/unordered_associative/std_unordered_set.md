# `std::unordered_set`

A hash-based dynamic container that stores unique elements and allows for fast average-time look-up, addition, and deletion operations. Unlike `std::set`, it does not sort the elements and generally has higher memory overhead.
## Characteristics

- **Hash-Based**: Utilizes a hash table for its underlying implementation.
- **Unique Elements**: Only unique elements are stored, similar to `std::set`.
- **Dynamic Size**: The container can grow or shrink as needed.
- **Average Constant-Time Operations**: Average time complexity for insertions, deletions, and lookups is `O(1)`.
- **Higher Memory Overhead**: Generally requires more memory than tree-based containers like `std::set`.
- **Unsorted**: The elements in an `std::unordered_set` are not sorted.
- **Custom Hash Function**: Allows specifying custom hash functions for complex data types.

## Basic usage

```c++
#include <iostream>
#include <unordered_set>

int main() {
    std::unordered_set<int> numbers;

    // Insert elements
    numbers.insert(5);
    numbers.insert(3);
    numbers.insert(8);

    // Print elements (order not guaranteed)
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    // Output: 5 3 8
}
```
## When to Use

- When you require fast average-time complexity for insertions, deletions, and lookups.
- When the ordering of elements is not important.
## Limitations

- No sorted order of elements.
- Higher memory overhead than `std::set`.
- All elements must be hashable or you must provide a custom hash function.
## Common Functions

### Element Management

- `insert(value)`: Inserts a unique value.
- `emplace(args)`: Constructs and inserts a unique value in-place.
- `erase(value)`: Removes a specific value.
- `clear()`: Clears all elements.
### Element Lookup

- `find(value)`: Returns an iterator to the found element, or `end()` if not found.
- `count(value)`: Returns 1 if the element exists, otherwise 0 (since duplicates aren't allowed).
### Set Properties

- `size()`: Returns the number of elements in the set.
- `empty()`: Checks if the set is empty.
- `max_size()`: Returns the maximum number of elements the set can hold.
### Iteration

- `begin()`, `end()`: Provide iterators to traverse the set.