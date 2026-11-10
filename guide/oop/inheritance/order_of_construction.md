# Order of Construction

When an object of a derived class is created, the constructors are called in the following order:

- Base class constructor(s) are called first, in the order in which they are listed in the derived class declaration.
- Member objects' constructors are called next, in the order in which they appear in the class declaration.
- Derived class constructor is called last.

This order ensures that the base parts of the object are fully constructed before the derived parts are constructed.

Consider the following C++ code:

```c++
#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base Constructor" << std::endl;
    }
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived Constructor" << std::endl;
    }
};

int main() {
	Base b;
	std::cout << "---" << std::endl;
    Derived d;
    return 0;
}

```

Output
```
Base Constructor
---
Base Constructor 
Derived Constructor
```

As you can see, when we constructed Derived, the Base portion of Derived got constructed first. This makes sense: logically, a child can not exist without a parent. It’s also the safe way to do things: the child class often uses variables and functions from the parent, but the parent class knows nothing about the child. Instantiating the parent class first ensures those variables are already initialized by the time the derived class is created and ready to use them.
## Order of destruction

When an object of a derived class is destroyed, the destructors are called in the reverse order of construction:

- Derived class destructor is called first.
- Member objects' destructors are called next, in the reverse order in which they appear in the class declaration.
- Base class destructor(s) are called last, in the reverse order in which they are listed in the derived class declaration.

This order ensures that the derived parts of the object are fully destroyed before the base parts are destroyed.

```c++
#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base Constructor" << std::endl;
    }
    ~Base() {
        std::cout << "Base Destructor" << std::endl;
    }
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived Constructor" << std::endl;
    }
    ~Derived() {
        std::cout << "Derived Destructor" << std::endl;
    }
};

int main() {
    Derived d;
    return 0;
}
```

Output:

```
Base Constructor
Derived Constructor
Derived Destructor
Base Destructor
```

As you can see from the output, the Derived class destructor is called first, followed by the Base class destructor.

Note: If there are multiple base classes or multiple levels of inheritance, the constructors of base classes are called in the order in which they are listed in the derived class declaration, and their destructors are called in the reverse order.

## Parameterized Constructors

Both base and derived classes can have parameterized constructors. You can explicitly call the base class constructor in the initializer list of the derived class constructor.

```c++
class Base {
public:
    Base(int x) {
        std::cout << "Base class constructor called with value: " << x << std::endl;
    }
};

class Derived : public Base {
public:
    Derived(int y) : Base(y) {
        std::cout << "Derived class constructor called with value: " << y << std::endl;
    }
};

int main() {
    Derived d(10);
    return 0;
}
```

Output:

```
Base class constructor called with value: 10
Derived class constructor called with value: 10
```

### Member Initialization

Derived classes may also include additional member variables. These can be initialized in the derived class constructor's initializer list.

```c++
class Base {
public:
    int x;
    Base(int val) : x(val) {}
};

class Derived : public Base {
public:
    int y;
    Derived(int val1, int val2) : Base(val1), y(val2) {}
};

int main() {
    Derived d(10, 20);
    return 0;
}
```