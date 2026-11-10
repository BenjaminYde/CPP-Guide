# `std::unordered_multimap`

### Characteristics

- **Unordered Container**: `std::unordered_multimap` does not maintain any ordering of its key-value pairs. They are organized into buckets based on their hash values.

- **Allows Duplicate Keys**: Unlike `std::unordered_map`, this container allows for multiple key-value pairs with identical keys.

- **Dynamic Size**: The container can grow or shrink dynamically.

- **Hash-Based**: It uses a hash table for its underlying implementation. This typically provides average constant-time complexity for search, insertion, and deletion.

## Basic usage

```c++
#include <iostream>
#include <unordered_map>

int main() {
    std::unordered_multimap<int, std::string> mmap;

    // Insert elements
    mmap.insert({1, "apple"});
    mmap.insert({1, "orange"});  // Duplicate keys allowed
    mmap.insert({2, "banana"});

    // Access and print elements
    auto range = mmap.equal_range(1);
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    // Output could be:
    // 1: apple
    // 1: orange

    // Remove an element
    mmap.erase(1);  // Removes all key-value pairs with key 1

    return 0;
}
```

## When to Use `std::unordered_multimap`

- When you need a hash table-based key-value store with fast look-ups.
- When you need to allow duplicate keys.
- When you don't need the key-value pairs to be sorted.
## Limitations

- No direct access by index.
- Keys must be hashable, meaning you have to provide a hash function for custom types.
## Common Functions

### Element Management

- `insert({key, value})`: Inserts a key-value pair.
- `emplace(args)`: Constructs and inserts a key-value pair in-place.
- `erase(key)`: Removes all key-value pairs with a specific key.
- `clear()`: Clears all key-value pairs.
### Element Lookup

- `find(key)`: Returns an iterator to the first occurrence of the key, or `end()` if not found.
- `count(key)`: Returns the number of key-value pairs with a specific key.
### Map Properties

- `size()`: Returns the number of key-value pairs.
- `empty()`: Checks if the map is empty.
### Iteration

- `begin()`, `end()`: Provide iterators to traverse through the elements.