# `std::unordered_map`

The `std::unordered_map` container in the C++ Standard Library serves as a collection of key-value pairs, similar to `std::map`. However, the underlying data structure for `std::unordered_map` is a hash table. This allows for average-time complexity of `O(1)` for search, insert, and delete operations, compared to the `O(log n)` time complexity of `std::map`.

The elements are not stored in any sorted order within the container but are organized into buckets depending on their hash values to allow fast access to individual elements directly by their key values (which are also used to uniquely identify the elements).

While this provides excellent average-case time complexities, it can have poor worst-case time complexities due to possible hash collisions. Also, the hash table data structure typically uses more memory than a balanced search tree.

In summary, `std::unordered_map` is your go-to container if you don't need the keys to be sorted but do need fast, average-case performance for insertion, deletion, and search operations.
## Characteristics

- **Hash-Based**: `std::unordered_map` uses a hash table for its underlying implementation, which allows for fast access but does not maintain order.

- **Unique Keys**: `std::unordered_map` only stores one value per unique key. If you try to insert a value with a key that already exists, the value for that key will be updated.

- **Dynamic Size**: `std::unordered_map` can grow or shrink dynamically. Memory management is automatic.

- **Unsorted**: The elements in an `std::unordered_map` are not sorted, unlike `std::map`.

- **Fast Average Time Complexity**: Operations like insertion, deletion, and search have a fast average-time complexity of `O(1)` but can degrade to `O(n)` in the worst-case scenario due to hash collisions.

- **Higher Memory Overhead**: Typically has more memory overhead compared to tree-based containers like `std::map`.
## Basic usage

```c++
#include <iostream>
#include <unordered_map>

int main() {
    std::unordered_map<std::string, int> age_map;

    // Insert key-value pairs
    age_map["Alice"] = 25;
    age_map["Bob"] = 30;

    // Access and print elements
    std::cout << "Alice's age: " << age_map["Alice"] << std::endl;  
    // Output: Alice's age: 25
}
```

## When to use `std::unordered_map`

- When you need fast look-ups but do not require sorting by key.
- When you need to associate values with unique keys.
## Limitations

- Unlike `std::vector` or `std::deque`, random access by index is not available; you have to use keys to access values.
- The hash function can lead to uneven distribution, which can degrade performance in some cases.
## Common Functions

### Element Management

- `insert({key, value})`: Inserts a unique key-value pair.
- `emplace(args)`: Constructs and inserts a unique key-value pair in-place.
- `erase(key)`: Removes a key-value pair by its key.
- `clear()`: Removes all key-value pairs.
### Element Lookup

- `find(key)`: Returns an iterator to the found pair, or `end()` if not found.
- `at(key)`: Accesses the value at a given key, throws an exception if key is not found.
- `operator[key]`: Returns a reference to the value at a given key, inserts default if key is not found.
### Map Properties

- `size()`: Returns the number of key-value pairs.
- `empty()`: Checks if the map is empty.
### Iteration

- `begin()`, `end()`: Provide iterators to the beginning and end of the map. Note that the order is not sorted.