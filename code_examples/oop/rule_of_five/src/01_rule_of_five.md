# Awnsers

## The Basics of Copying

### `exercise_1_1`

```
🚀 Exercise 1.1
  t1: Constructor
  t1: COPY Constructor from t1
...end of scope
  t1: Destructor
  t1: Destructor
```

The line `Tracer t2 = t1;` is an initialization, not an assignment. Since `t2` is being created and initialized at the same time from an existing object (`t1`), the copy constructor is called.

### `exercise_1_2`

```
🚀 Exercise 1.2
  t1: Constructor
  t2: Constructor
  t2: COPY Assignment from t1
...end of scope
  t1: Destructor
  t1: Destructor
```

## The Efficiency of Moving

Here, `t2` already exists. We are assigning a new value to it from `t1`. This is an assignment operation between two existing objects, so the copy assignment operator `operator=` is invoked.

### `exercise_2_1`

```
🚀 Exercise 2.1
  temp: Constructor
...end of scope
  temp: Destructor
```

#### Wait, what?! Where is the move constructor?

Explanation: **Return Value Optimization (RVO)**
This is a trick question to introduce a vital concept. The C++ standard allows compilers to perform an optimization called Return Value Optimization (RVO).

Instead of creating a `Tracer` object inside `createTracer`, then move-constructing `t3` from it, the compiler is smart enough to construct the object directly in the memory location of `t3`. It elides (omits) the move constructor call entirely.

If RVO were disabled, you would see this:

```
🚀 Exercise 2.2
  t1: Constructor
  t1: MOVE Constructor from t1
State of t1: name = 't1 [moved]'
  t1: Destructor
  t1 [moved]: Destructor
```

### `exercise_2_2`

```
🚀 Exercise 2.3
  t5: Constructor
  temp: Constructor
  t5: MOVE Assignment from temp
  temp [moved]: Destructor
  temp: Destructor
```

- `std::move(t1)`: This is a cast. It tells the compiler to treat the lvalue `t1` as if it were a temporary (an rvalue). It signals, "I am done with `t1`, its resources can be stolen." (move contstructor)
- `t4` is created by stealing `t1`'s resources. The line State of `t1: name = ''` confirms that `t1` is now empty.
- The fact `t4` is destroyed first is because of the local variable called "the stack", a fundamental rule in C++ (variables declared inside a function): Last In, First Out (LIFO).

### `exercise_2_3`

```
🚀 Exercise 3.1
  t1: Constructor
Pushing back t1 (lvalue)...
  t1: COPY Constructor from t1
Pushing back temporary (rvalue)...
  temp: Constructor
  temp: MOVE Constructor from temp
  t1: MOVE Constructor from t1
  t1 [moved]: Destructor
  temp [moved]: Destructor
  t1: Destructor
  t1: Destructor
  temp: Destructor
```

- `createTracer("temp")`: This function call produces a temporary object (an rvalue).
- `t5 = ...`: The assignment is from this rvalue. The compiler chooses the overload for assigning from an rvalue: the move assignment operator `operator=(Tracer&&)`.
- `t5`'s original content is discarded, and it takes ownership of the resources from the temporary object. The temporary object, now empty, is destroyed immediately after the assignment.

### `exercise_3_1`

```
🚀 Exercise 3.1
  t1: Constructor
Pushing back t1 (lvalue)...
  t1: COPY Constructor from t1
Pushing back temporary (rvalue)...
  temp: Constructor
  temp: MOVE Constructor from temp
  t1: MOVE Constructor from t1
  t1 [moved]: Destructor
  temp [moved]: Destructor
  t1: Destructor
  t1: Destructor
  temp: Destructor
```

This exercise demonstrates how std::vector uses both copy and move semantics, and reveals its internal growth strategy.

- `vec.push_back(t1);`
    - `t1` is an lvalue (we might use it again). The vector cannot steal from it.
    - To add it to its internal storage, the vector must make a copy, calling the copy constructor. The vector now holds one element.
    `vec.push_back(createTracer("temp"));`
- This is a multi-step process. The vector had a capacity of 1, but now it needs to hold 2 elements. It must grow.
    - **Step A (Reallocation)**: The vector allocates a new, larger block of memory. To move its existing elements from the old memory to the new, it uses the move constructor. This is the line t1: MOVE CONSTRUCTOR. It is moving the first element that was already in the vector.
    - **Step B (Push Back)**: Now that there is space, it handles the new element. The argument `createTracer("temp")` is an rvalue. The vector uses the move constructor again to efficiently move this new element into its new memory block. This is the line `temp: MOVE CONSTRUCTOR`.
      - The vector tries to add a new element, but its memory block is full. It cannot simply add it to the end. It must perform a reallocation. This is the critical event. It allocates a completely new, larger block of memory somewhere else. Let's say the new capacity is 2 (or 4, it's up to the implementation).
        - Old Memory: `[ {"t1"} ]`
        - New Memory: `[ empty, empty ]`
      - It must move all of its existing elements from the old memory block to the new one.
        - This is where the line you asked about comes from! The vector calls the move constructor to move the {"`t1`"} object from the old location to the new one because moving is faster than copying.
        - Output: `t1`:` MOVE CONSTRUCTOR (into a new object)`
              

### `exercise_4_1`

```
🚀 Exercise 4.1
  temp: Constructor
...end
  temp: Destructor
```

Zero.
Even though the function returns by value, C++17+ mandatory copy elision (previously NRVO) constructs the object directly in the caller’s storage. No copy or move constructor is called — not even the move one.

### `exercise_5_1`

```
🚀 Exercise 5.1
  Alice: Constructor
  Bob: Constructor
Before move: a = Alice, b = Bob
  Alice: MOVE Assignment from Bob
After move:  a = Bob, b = Bob [moved]
  Bob [moved]: Destructor
  Bob: Destructor
```

The move assignment operator (`operator=(Tracer&&)`) is called.
`a`’s previously owned resource is deleted first, then `a` steals `b`’s pointer. `b` is left in a valid but moved-from state (`resource == nullptr`).

### `exercise_6_1`

```
🚀 Exercise 6.1
Adding 4 elements...
  v0: Constructor
  v1: Constructor
  v2: Constructor
  v3: Constructor
Forcing reallocation...
  v4: Constructor
  v0: MOVE Constructor from v0
  v0 [moved]: Destructor
  v1: MOVE Constructor from v1
  v1 [moved]: Destructor
  v2: MOVE Constructor from v2
  v2 [moved]: Destructor
  v3: MOVE Constructor from v3
  v3 [moved]: Destructor
  v0: Destructor
  v1: Destructor
  v2: Destructor
  v3: Destructor
  v4: Destructor
```

Existing elements are move-constructed into the new buffer (never copied if a `noexcept` move constructor exists).
If the move constructor is not `noexcept`, the vector falls back to copy construction for strong exception safety. This is why real resource-managing types must declare move operations `noexcept`.