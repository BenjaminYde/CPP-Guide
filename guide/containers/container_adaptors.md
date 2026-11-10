# Container Adaptors

## std::queue

The `std::queue` is a container adapter in C++ Standard Library that provides a First-In, First-Out (FIFO) data structure, often used in scenarios that require elements to be processed in the order they were added. The underlying data structure for a `std::queue` is `std::deque`.

Here's a simple example to demonstrate the usage of `std::queue`:

```c++
#include <iostream>
#include <queue>

int main() {
    std::queue<int> myQueue;

    // Adding elements
    myQueue.push(1);
    myQueue.push(2);
    myQueue.push(3);

    // Accessing front element
    std::cout << "Front element: " << myQueue.front() << std::endl;

    // Removing elements
    myQueue.pop();
    std::cout << "Front element after pop: " << myQueue.front() << std::endl;

    // Checking size and emptiness
    std::cout << "Queue size: " << myQueue.size() << std::endl;
    std::cout << "Is empty: " << (myQueue.empty() ? "Yes" : "No") << std::endl;

    return 0;
}
```

```
Front element: 1
Front element after pop: 2
Queue size: 2
Is empty: No
```

Explanation:

1. After pushing `1`, `2`, and `3`, the queue would look like this internally: [1, 2, 3].
2. `myQueue.front()` should return `1`, which is the element at the front of the queue.
3. `myQueue.pop()` would remove the front element (`1`), making the queue look like: [2, 3].
4. `myQueue.front()` after the `pop()` should return `2`, the new front element.
5. `myQueue.size()` should return `2`, as there are two elements left in the queue.
6. `myQueue.empty()` should return `false` (or "No" as per your output), as the queue is not empty.
### Member Functions

Here are some commonly used member functions:

- `push(const T& value)`: Adds a new element to the end of the queue.
- `pop()`: Removes the front element from the queue.
- `front()`: Returns a reference to the front element in the queue.
- `back()`: Returns a reference to the last element in the queue.
- `empty()`: Checks whether the queue is empty.
- `size()`: Returns the number of elements in the queue.
### Notes

- `std::queue` doesn't provide iterator support.
- It doesn't provide direct access to the elements apart from the front and the back.
- Being a container adapter, it inherits the characteristics and limitations of the underlying container in terms of performance. For example, the time complexity for operations like `push`, `pop`, etc., would depend on what the underlying container provides.

By using `std::queue`, you make it explicit that you're using a FIFO data structure, which can make the code easier to understand and potentially reduce errors.

## std::stack

The `std::stack` is a container adapter in the C++ Standard Library. It provides a Last-In, First-Out (LIFO) data structure, which is often useful in scenarios that require elements to be processed in the reverse order of their arrival. 

Here's a simple example to demonstrate how to use `std::stack`:

```c++
#include <iostream>
#include <stack>

int main() {
    std::stack<int> myStack;

    // Adding elements
    myStack.push(1);
    myStack.push(2);
    myStack.push(3);

    // Accessing top element
    std::cout << "Top element: " << myStack.top() << std::endl;

    // Removing top element
    myStack.pop();
    std::cout << "Top element after pop: " << myStack.top() << std::endl;

    // Checking size and emptiness
    std::cout << "Stack size: " << myStack.size() << std::endl;
    std::cout << "Is empty: " << (myStack.empty() ? "Yes" : "No") << std::endl;

    return 0;
}
```

```
Top element: 3
Top element after pop: 2
Stack size: 2
Is empty: No
```

Explanation:

1. After pushing `1`, `2`, and `3`, the stack would look like this internally (from bottom to top): `[1, 2, 3]`.
2. `myStack.top()` should return `3`, which is the element at the top of the stack.
3. `myStack.pop()` would remove the top element (`3`), making the stack look like: `[1, 2]`.
4. `myStack.top()` after the `pop()` should return `2`, which is the new top element.
5. `myStack.size()` should return `2`, indicating there are two elements left in the stack.
6. `myStack.empty()` should return `false` (or "No" as per your output), as the stack is not empty.
### Member Functions

Here are some key member functions for `std::stack`:

- `push(const T& value)`: Adds a new element to the top of the stack.
- `pop()`: Removes the top element from the stack.
- `top()`: Returns a reference to the top element in the stack.
- `empty()`: Checks whether the stack is empty.
- `size()`: Returns the number of elements in the stack.
### Notes

- `std::stack` doesn't provide iterator support.
- It doesn't allow direct access to the elements apart from the top one.
- It inherits the characteristics and limitations of the underlying container. For instance, the time complexity of various operations like `push`, `pop`, etc., will depend on the underlying container's implementation.

By using `std::stack`, you can clearly communicate your intent of using a LIFO data structure, which can make your code easier to read and maintain.

## std::priority_queue

The `std::priority_queue` is a container adapter in the C++ Standard Library that provides a priority queue. In a priority queue, the element with the highest priority (or the lowest, depending on the comparator used) is always at the front. Elements are not sorted in ascending or descending order but are rather organized in a "heap" data structure, which enables efficient insertion and retrieval of the highest-priority element.

Here is a simple example demonstrating how to use `std::priority_queue`:

```c++
#include <iostream>
#include <queue>

int main() {
    std::priority_queue<int> myPQueue;

    // Adding elements
    myPQueue.push(1);
    myPQueue.push(3);
    myPQueue.push(2);

    // Accessing top (highest-priority) element
    std::cout << "Top element: " << myPQueue.top() << std::endl;

    // Removing top element
    myPQueue.pop();
    std::cout << "Top element after pop: " << myPQueue.top() << std::endl;

    // Checking size and emptiness
    std::cout << "Priority queue size: " << myPQueue.size() << std::endl;
    std::cout << "Is empty: " << (myPQueue.empty() ? "Yes" : "No") << std::endl;

    return 0;
}
```

```
Top element: 3
Top element after pop: 2
Priority queue size: 2
Is empty: No
```

Explanation:

1. After pushing `1`, `3`, and `2`, the priority queue arranges the elements based on their priority. In the case of the default max-heap, it would arrange them internally to make sure the highest element (i.e., `3`) is at the top.
2. `myPQueue.top()` returns `3`, which is the highest-priority (in this case, the largest) element.
3. After calling `myPQueue.pop()`, the top element (`3`) is removed, and the next highest element (`2`) takes its place at the top.
4. `myPQueue.top()` after the `pop()` returns `2`, which is the new highest-priority element.
5. `myPQueue.size()` returns `2` as there are two elements left in the priority queue.
6. `myPQueue.empty()` returns `false` (or "No" in your output), indicating that the priority queue is not empty.
### Member Functions

Here are some key member functions for `std::priority_queue`:

- `push(const T& value)`: Inserts a new element into the priority queue. The element is positioned to maintain the heap property.
- `pop()`: Removes the top element from the priority queue.
- `top()`: Returns a constant reference to the top element (highest-priority) in the priority queue.
- `empty()`: Checks whether the priority queue is empty.
- `size()`: Returns the number of elements in the priority queue.
### Notes

- `std::priority_queue` doesn't provide iterator support.
- It doesn't allow direct access to the elements apart from the top one.
- It inherits the characteristics and limitations of the underlying container, affecting the time complexity of various operations.

Using `std::priority_queue` allows you to manage a collection of elements where the order of processing is determined by some priority, without worrying about the underlying heap management.