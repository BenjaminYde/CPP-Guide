# `dynamic_cast`

  
Dynamic casting in C++ is performed using the `dynamic_cast` operator. It is primarily used for safely downcasting pointers or references within a hierarchy of classes involving polymorphism. Unlike other casting operators, `dynamic_cast` checks the validity of the cast at runtime.

### When to Use

1. **Downcasting**: You can use `dynamic_cast` to convert a pointer (or reference) from a base class to a derived class.
2. **Polymorphism**: It works only with pointers or references to polymorphic types (i.e., classes with at least one virtual function).

### Syntax

Pointers:
```c++
Derived* d = dynamic_cast<Derived*>(base_ptr);
```

References:
```c++
Derived& d = dynamic_cast<Derived&>(base_ref);
```

### Example

Consider the following example involving a base class `Animal` and a derived class `Dog`.

```c++
class Animal {
public:
    virtual void makeSound() {} // Making it polymorphic
};

class Dog : public Animal {
public:
    void makeSound() { std::cout << "Woof" << std::endl; }
    void fetchStick() { std::cout << "Fetching stick" << std::endl; }
};

int main() {
    Animal* animalPtr = new Animal();
    Animal* dogPtr = new Dog();

    Dog* d;

    // Successful dynamic cast
    d = dynamic_cast<Dog*>(dogPtr);
    if(d) {
        d->fetchStick(); // Output: "Fetching stick"
    }

    // Unsuccessful dynamic cast
    d = dynamic_cast<Dog*>(animalPtr);
    if(d == nullptr) {
        std::cout << "Failed to cast Animal to Dog." << std::endl; // Output: "Failed to cast Animal to Dog."
    }

    return 0;
}
```