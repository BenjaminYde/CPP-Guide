# Friend

The `friend` keyword in C++ declares that a non-member function or another class is granted special access to the `private` and `protected` members of the class making the declaration.

This feature does not break encapsulation. Rather, it extends the encapsulation boundary of a class to include its trusted friends. It's a mechanism to signal a tight coupling and a special relationship between two entities that are conceptually part of the same interface, even if they are not part of the same class definition. Misuse can lead to poor design, but when used correctly, `friend` is an elegant and powerful tool.

## Friend Function

A `friend` function is not a member of the class, but it can access the private and protected members of the class.

- It is **declared** inside the class with the `friend` keyword but is **defined** outside the class scope.
- As a non-member, it does not have a `this` pointer and is not called with the dot (`.`) or arrow (`->`) operators. (e.g., `myObject.myFunction()`).
- The object it needs to operate on must be passed as a parameter.

```c++
#include <iostream>

class Box {
private:
    int length;

public:
    Box(int l) : length(l) {}

    // The Box class grants friendship to the getLength function.
    friend int getLength(const Box& b);
};

// getLength is a non-member function, defined globally.
int getLength(const Box& b) {
    // It can access Box's private member because it's a friend.
    return b.length;
}

int main() {
    Box box(10);
    std::cout << "Length of box: " << getLength(box) << std::endl;
    return 0;
}
```

## Friend Class

A class can declare another class as its friend. This gives all member functions of the friend class access to the private and protected members of the declaring class.

- Friendship is a **one-way street**. If `A` declares `B` as a friend, `B `can access `A`'s private members, but A cannot access `B`'s.
- Friendship is **not transitive**. `A` friend of a friend is not a friend.
- Friendship is **not inherited**.

```c++
#include <iostream>

class Box {
private:
    int length;

public:
    Box(int l) : length(l) {}

    // All functions in BoxInspector are now friends of Box.
    friend class BoxInspector;
};

class BoxInspector {
public:
    int inspectLength(const Box& b) {
        // This works because BoxInspector is a friend of Box.
        return b.length;
    }
};

int main() {
    Box box(20);
    BoxInspector inspector;
    std::cout << "Inspector reports length: " << inspector.inspectLength(box) << std::endl;
    return 0;
}
```

## Friend Member Function

For even more fine-grained control, you can declare a specific member function from another class as a friend, rather than the entire class.

- This grants access only to the specified function, following the principle of least privilege.
- This requires careful ordering of declarations and definitions, often involving **forward declarations**.

```c++
#include <iostream>

// Forward declare Box so BoxInspector knows it exists.
class Box;

class BoxInspector {
public:
    int inspectLength(const Box& b);
};

class Box {
private:
    int length;

public:
    Box(int l) : length(l) {}

    // Only this specific member function is a friend, not the entire class.
    friend int BoxInspector::inspectLength(const Box& b);
};

// The definition must come after Box is fully defined.
int BoxInspector::inspectLength(const Box& b) {
    return b.length; // Access is granted.
}

int main() {
    Box box(30);
    BoxInspector inspector;
    std::cout << "Inspector's specific function reports length: " << inspector.inspectLength(box) << std::endl;
    return 0;
}
```

## `friend` and Operator Overloading

One of the most important uses of `friend` is in operator overloading. When overloading binary operators (like `+`, `-`, `<<,` `>>),` we must decide whether to make the operator a member function, a `friend` function, or a regular non-member function.

### Case Study: A `Cents` Class

Let's use a simple `Cents` class to explore the options.

```c++
class Cents {
private:
    int m_cents;
public:
    Cents(int cents) : m_cents(cents) {}
    int getCents() const { return m_cents; }
};
```

### Option 1: Member Function Overload

The operator can be a member of the class. This works well when the left-hand operand of the operator is an object of the class.

```c++
class Cents {
private:
    int m_cents;
public:
    Cents(int cents) : m_cents(cents) {}
    int getCents() const { return m_cents; }

    // operator+ is a member of Cents
    Cents operator+(const Cents& other) const {
        return Cents(m_cents + other.m_cents);
    }
};

// Usage:
Cents c1(5), c2(7);
Cents sum = c1 + c2; // Works perfectly, translated as c1.operator+(c2)
```

**The Limitation**: A member function requires the class object to be on the left. This means some intuitive operations will fail to compile:

```c++
Cents sum = 5 + c1; // COMPILE ERROR! Translated as 5.operator+(c1), and int has no such member.
```

### Option 2: `friend` Function Overload

A `friend` function is a non-member, so it doesn't have the left-hand operand restriction. It's the perfect solution when you need to support operations with different types on the left and also need access to `private` members.

```c++
class Cents {
private:
    int m_cents;
public:
    Cents(int cents) : m_cents(cents) {}
    int getCents() const { return m_cents; }

    // Grant friendship to these non-member functions
    friend Cents operator+(const Cents& c1, const Cents& c2);
    friend Cents operator+(const Cents& cents, int value);
    friend Cents operator+(int value, const Cents& cents);
};

// Definitions (outside the class)
Cents operator+(const Cents& c1, const Cents& c2) {
    return Cents(c1.m_cents + c2.m_cents); // Accesses private m_cents
}
Cents operator+(const Cents& cents, int value) {
    return Cents(cents.m_cents + value); // Accesses private m_cents
}
Cents operator+(int value, const Cents& cents) {
    return Cents(value + cents.m_cents); // Accesses private m_cents
}

// Usage:
Cents c1(5);
Cents sum1 = c1 + 10; // Works! Calls operator+(Cents, int)
Cents sum2 = 5 + c1;  // Works! Calls operator+(int, Cents)
```

### Option 3: Regular Non-Member Function Overload

If your class provides sufficient `public` accessors (like `.getCents()`), you may not need `friend` access at all. You can implement the operator as a regular non-member function.

```c++
// Cents class definition is the same, but without the friend declaration.
class Cents { /*...*/ };

// Note: this function is not a friend!
Cents operator+(const Cents& c1, const Cents& c2) {
    // It uses the public getCents() method, no private access needed.
    return Cents(c1.getCents() + c2.getCents());
}
```

## A Practical Example: Overloading Stream Operators

In C++, the `<<` (insertion) and `>>` (extraction) operators are the conventional way to handle output and input for built-in types like `int`, `double`, and `std::string`. When you create a custom, user-defined type (like a `class` or `struct`), C++ doesn't automatically know how to stream its data to an output (like `std::cout`) or read its state from an input (like `std::cin`).

A common use of `friend` is for overloading the stream insertion (`<<`) and extraction (`>>`) operators. This example clearly shows why a function is declared as a friend inside the class but defined outside.

```c++
#include <iostream>

class Point {
private:
    int m_x, m_y; // Private members for good encapsulation

public:
    Point(int x = 0, int y = 0) : m_x(x), m_y(y) {}

    // 1. Declaration inside the class:
    // The Point class GRANTS friendship status to the two operator functions.
    friend std::ostream& operator<<(std::ostream& out, const Point& p);
    friend std::istream& operator>>(std::istream& in, Point& p);
};

// 2. Definition outside the class:
// These functions are NOT members of Point. They are defined externally
// because the left-hand operand (std::ostream or std::istream) is not a Point object.
// They can, however, access p.m_x and p.m_y because Point declared them as friends.
std::ostream& operator<<(std::ostream& out, const Point& p) {
    out << "(" << p.m_x << ", " << p.m_y << ")";
    return out;
}

std::istream& operator>>(std::istream& in, Point& p) {
    in >> p.m_x >> p.m_y;
    return in;
}

int main() {
    Point p1(5, 10);
    Point p2;

    // The friendly operator<< is called
    std::cout << "Point p1 is " << p1 << std::endl;

    // The friendly operator>> is called
    std::cout << "Enter new coordinates for p2 (e.g., 3 7): ";
    std::cin >> p2;
    std::cout << "Point p2 is now " << p2 << std::endl;

    return 0;
}
```

Of course. Here is a to-the-point explanation of how friendship interacts with the rules of inheritance.

## Friendship and Inheritance

There are three fundamental rules to remember:

1. Friendship is Not Inherited
2. Friendship is Not Transitive
3. A Friend of a Derived Class Cannot Access the Base's Private Members

### Rule 1: Friendship is Not Inherited

- **Question**: "I made a friend of my `Base` class. Can it touch my `Derived` class's new private stuff?"
- **Awnser**: No

If a base class `Base` declares `FriendlyClass` as a `friend`, that friendship status is **not** inherited by any class that derives from `Base`. The derived class's private members remain private to that `friend`.

This is a critical feature for encapsulation. It ensures that a new derived class doesn't unintentionally expose its private details to friends of its parent class.

```cpp
#include <iostream>

class FriendlyClass {
public:
    // This function can access Base's private members
    void accessBase(class Base& b);

    // This function will ATTEMPT to access Derived's private members
    void accessDerived(class Derived& d);
};

class Base {
private:
    int base_private = 10;

public:
    // Grant friendship ONLY to FriendlyClass
    friend void FriendlyClass::accessBase(Base& b);
};

class Derived : public Base {
private:
    int derived_private = 20;

public:
    // Friendship is NOT inherited, so FriendlyClass
    // cannot access derived_private.
};

void FriendlyClass::accessBase(Base& b) {
    std::cout << "FriendlyClass accessing Base::base_private: " << b.base_private << std::endl; // OK
}

void FriendlyClass::accessDerived(Derived& d) {
    // COMPILER ERROR HERE!
    // Friendship was with Base, not Derived.
    // std::cout << d.derived_private << std::endl;
    std::cout << "FriendlyClass cannot access Derived's private members." << std::endl;
}

int main() {
    Base b;
    Derived d;
    FriendlyClass f;

    f.accessBase(b);
    f.accessDerived(d);

    return 0;
}
```

Output:

```
FriendlyClass accessing Base::base_private: 10
FriendlyClass cannot access Derived's private members.
```

- **Pro:** This rule strengthens encapsulation. A derived class starts with a clean slate regarding friendships, preventing accidental exposure of its internal state.
- **Real-world case:** If `Base` is a generic `Shape` class and its friend is a `ShapeDebugger`, a new `SpecializedShape` derived from `Shape` can be created with confidence that the `ShapeDebugger` cannot accidentally access new private members specific to `SpecializedShape` without being explicitly granted friendship again.

### Rule 2: Friendship is Not Transitive

A "**friend of a friend is not a friend.**" If class `A` is a friend of `B`, and `B` is a friend of `C`, class `A` has no special access to `C`.

```cpp
#include <iostream>

// Forward declarations are needed because classes refer to each other.
class C;
class B;

class A {
public:
    // A is a friend of B. This function will access B's private members.
    void accessB(B& b_obj);

    // A will attempt to access C's private members.
    void tryToAccessC(C& c_obj);
};

class B {
private:
    int b_private = 20;

public:
    // B is a friend of C. This function will access C's private members.
    void accessC(C& c_obj);

    // A is a friend of B, granting it access to b_private.
    friend void A::accessB(B& b_obj);
};

class C {
private:
    int c_private = 30;

public:
    // C grants friendship to B's member function.
    friend void B::accessC(C& c_obj);
};

// --- Definitions ---

void B::accessC(C& c_obj) {
    // This is OK because C granted friendship to this specific function.
    std::cout << "B successfully accessed C::c_private: " << c_obj.c_private << std::endl;
}

void A::accessB(B& b_obj) {
    // This is OK because B granted friendship to this specific function.
    std::cout << "A successfully accessed B::b_private: " << b_obj.b_private << std::endl;
}

void A::tryToAccessC(C& c_obj) {
    // COMPILER ERROR HERE if you uncomment the line below!
    // A is a friend of B, and B is a friend of C.
    // But friendship is NOT transitive. A has no access to C.
    // std::cout << c_obj.c_private << std::endl;
    std::cout << "A cannot access C's private members." << std::endl;
}

int main() {
    A a;
    B b;
    C c;

    std::cout << "Demonstrating direct friendship:" << std::endl;
    a.accessB(b); // A can access B.
    b.accessC(c); // B can access C.

    std::cout << "\nDemonstrating non-transitive friendship:" << std::endl;
    a.tryToAccessC(c); // A CANNOT access C.

    return 0;
}
```

Output:

```
Demonstrating direct friendship:
A successfully accessed B::b_private: 20
B successfully accessed C::c_private: 30

Demonstrating non-transitive friendship:
A cannot access C's private members.
```

### Rule 3: A Friend of a Derived Class Cannot Access the Base's Private Members

- **Question**: "I made a friend of my `Derived` class. Can it touch my `Base` class's old private stuff?"
- **Awnser**: No

Granting friendship to a function or class in a derived class gives it access to the members of the *derived class only*. It does not grant access to the `private` members of the base class.

```cpp
#include <iostream>

class Base {
private:
    int base_private = 40;
protected:
    int base_protected = 50;
};

class Derived; // Forward declaration

class DerivedFriend {
public:
    void inspect(Derived& d);
};

class Derived : public Base {
private:
    int derived_private = 60;
public:
    // Derived grants friendship to DerivedFriend
    friend void DerivedFriend::inspect(Derived& d);
};

void DerivedFriend::inspect(Derived& d) {
    // Access derived_private: OK
    std::cout << "DerivedFriend can see Derived::derived_private: " << d.derived_private << std::endl;

    // Access base_protected: OK (because Derived can access it)
    std::cout << "DerivedFriend can see Base::base_protected: " << d.base_protected << std::endl;

    // Access base_private: COMPILER ERROR!
    // DerivedFriend is a friend of Derived, not Base.
    // It has no special access to Base's private members.
    // std::cout << d.base_private << std::endl;
}

int main() {
    Derived d;
    DerivedFriend df;
    df.inspect(d);
    return 0;
}
```

Output:

```
DerivedFriend can see Derived::derived_private: 60
DerivedFriend can see Base::base_protected: 50
```

- **Why it works this way:** The `friend` only gets the same access rights as the class that granted the friendship. Since `Derived` can access its own members and its parent's `protected` members, so can its `friend`. However, `Derived` has no access to `Base`'s `private` members, so it cannot grant that privilege to its friends.
