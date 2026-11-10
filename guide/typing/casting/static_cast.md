# `static_cast`

`static_cast` is a type of casting operator used for conversions between types. It can perform conversions between pointers to related classes, not only from the derived class to its base, but also from base to derived. This ensures that if the type you are casting is not capable of being converted to the destination type, it will cause a compile error.

One key thing to note is that `static_cast` is a compile-time cast. It doesn't perform any runtime checks. This makes it faster, but also means that misuse can lead to undefined behavior.

In general, using `static_cast` is considered a best practice when you know a certain conversion is safe and you want to make the intention of your code clear. It's much safer than C-style casting and more constrained than other C++ casts, helping you avoid mistakes.

**What you can do with `static_cast`:**

1. **Convert between numeric types** such as int to float, float to double, etc.
2. **Convert from enum types to integers and vice versa**.
3. **Perform upcasting**: Convert a pointer or reference from a derived class to its base class.
4. **Perform downcasting** (with caution): Convert a pointer or reference from a base class to its derived class. The onus is on the programmer to ensure the object being cast is indeed of the derived type.
5. **Convert `void*` to any pointer type.**
6. **Convert pointer types within an inheritance hierarchy** to unrelated types that share a common base.
7. **Force implicit conversions**, for example, converting a non-const object to a const object, or invoking an overloaded operator.

**What you cannot do with `static_cast`:**

1. **Cannot convert from const to non-const** or vice versa. For such conversions, `const_cast` must be used.
2. **Cannot convert between unrelated types** such as unrelated classes, int to pointer, etc. `reinterpret_cast` is used for such conversions.
3. **Cannot safely convert a base class pointer into a derived class pointer** unless you are certain that the base class pointer or reference is actually pointing to an object of the derived class.
4. **Cannot cast away the `volatile` qualifier**. For such operations, `const_cast` is needed.

Here are some common scenarios where `static_cast` can be used:

### Convert numeric data types such as enum to int, float to int, etc

```c++
float f = 3.14;
int i = static_cast<int>(f);  // i is now 3
```

### Convert pointer or reference types within an inheritance hierarchy

```c++
class Base { /* ... */ };
class Derived : public Base { /* ... */ };

Derived d;
Base* pb = static_cast<Base*>(&d); // Upcast: always safe

Base b;
Derived* pd = static_cast<Derived*>(&b); // Downcast: NOT always safe
```

Here `static_cast` performs a downcast, which is only safe if the object being cast really is a `Derived`. No runtime check is performed to ensure this, and if the cast is invalid, the resulting behavior is undefined.

### Convert `void*` to any pointer type

```c++
int i = 10;
void* v = &i;
int* pi = static_cast<int*>(v);  // pi now points to i
```

This was used often in C-style code, especially before `reinterpret_cast` was introduced.

### Convert pointers to one of two unrelated classes that share a common base class

```c++
class Base { /* ... */ };
class Derived1 : public Base { /* ... */ };
class Derived2 : public Base { /* ... */ };

Derived1 d1;
Base* pb = static_cast<Base*>(&d1);

Derived2* pd2 = static_cast<Derived2*>(pb);  // Undefined behavior if pb doesn't actually point to a Derived2
```

if `pb` actually pointed to a `Derived2`, the `static_cast` would work as expected. However, in this case, `pb` points to a `Derived1`, so the cast results in undefined behavior.

### Convert `enum` class to int or vice versa

```c++
enum class Color { Red, Green, Blue };

Color c = Color::Red;
int i = static_cast<int>(c);  // i is now 0

i = 2;
c = static_cast<Color>(i);  // c is now Color::Blue
```

