# Multiple Inheritance

Multiple inheritance in C++ is a feature that allows a class to inherit from more than one base class. This is different from languages like Java or C#, where a class can inherit from only one parent class (single inheritance).

### Syntax

Here's a simple example to demonstrate the syntax of multiple inheritance in C++:

```c++
class A {
public:
    void functionA() {
        // code
    }
};

class B {
public:
    void functionB() {
        // code
    }
};

class C : public A, public B {
public:
    void functionC() {
        // code
    }
};
```

In this example, class `C` inherits from both class `A` and class `B`.
### Accessing Members

You can access members from all parent classes as if they belong to the derived class:

```c++
C obj;
obj.functionA();  // From class A
obj.functionB();  // From class B
obj.functionC();  // From class C
```

### The Diamond Problem

Multiple inheritance can sometimes lead to ambiguities, known as the "Diamond Problem." This occurs when a class inherits from two or more classes that have a common ancestor.

```c++
class A {
public:
    void foo() { /* ... */ }
};

class B : public A { /* ... */ };
class C : public A { /* ... */ };

class D : public B, public C {
public:
    void bar() {
        foo();  // Error: Ambiguous
    }
};
```

In this case, calling `foo()` in `D` is ambiguous because `D` has two paths to `A`. C++ provides a way to resolve this issue using virtual inheritance.

Virtual inheritance can be used to solve the Diamond Problem. When virtual inheritance is used, only one copy of the common ancestor is kept, resolving the ambiguity.

Here's how to modify the previous example using virtual inheritance:

```c++
class A {
public:
    void foo() { /* ... */ }
};

class B : virtual public A { /* ... */ };
class C : virtual public A { /* ... */ };

class D : public B, public C {
public:
    void bar() {
        foo();  // No ambiguity
    }
};
```

By declaring the inheritance from `A` as `virtual` in both `B` and `C`, we ensure that only one copy of `A` is inherited by `D`.

## Protected members

If both `B` and `C` have a `protected` member variable of the same name and type, and they are virtually inherited, then `D` will inherit a single, shared instance of that member variable, just like it does with the methods. There won't be a name collision or ambiguity in this situation when accessing that member variable.

Here's an example to illustrate:

```c++
#include <iostream>

class A {
public:
    A() : member(1) {}
protected:
    int member;
};

class B : virtual public A {
public:
    B() {
        member += 10;
    }
};

class C : virtual public A {
public:
    C() {
        member += 100;
    }
};

class D : public B, public C {
public:
    D() {
        std::cout << "Value of member: " << member << std::endl;
    }
};

int main() {
    D d;  // Output will be "Value of member: 111"
    return 0;
}
```

In this example, there is a single `protected` member variable called `member` in class `A`. Both `B` and `C` virtually inherit from `A`, so `D` ends up with a single, shared `member` variable inherited from `A`.

The constructors of `B` and `C` modify `member`, but they're working on the same instance of `member`. Hence, `D`'s constructor will output `111` for `member` (`1` (initial value) `+ 10` (from `B`) `+ 100` (from `C`)).

So, when `D` accesses `member`, it's actually accessing the single shared instance from `A`. There is no ambiguity.