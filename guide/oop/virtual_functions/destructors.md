# Virtual Destructors

In C++, a destructor is a special member function that cleans up resources or performs other cleanup tasks when an object goes out of scope or is explicitly deleted. When dealing with inheritance and pointers to base classes, it's crucial to make your base class's destructor `virtual` if you ever intend to derive from that class.

Here's how you can fix this with a virtual destructor:

```c++
class Base {
public:
    virtual ~Base() {
        std::cout << "Base Destructor\n";
    }
};

class Derived : public Base {
public:
    virtual ~Derived() {
        std::cout << "Derived Destructor\n";
    }
};

int main() {
    Base* ptr = new Derived();
    delete ptr;  // Both Derived and Base destructors get called
    return 0;
}
```

Now, when you delete `ptr`, both the `Derived` destructor and the `Base` destructor will be called, in that order. This ensures that any resources allocated in the `Derived` class will be properly released.