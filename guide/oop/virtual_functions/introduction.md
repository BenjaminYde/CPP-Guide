# Introduction to Virtual Functions

## What are virtual functions

A **virtual function** is a special type of member function that, when called, resolves to the most-derived version of the function for the actual type of the object being referenced or pointed to. 

This capability is known as **polymorphism**. A derived function is considered a match if it has the same signature (name, parameter types, and whether it is const) and return type as the base version of the function. Such functions are called **overrides**.

To make a function virtual, simply place the “virtual” keyword before the function declaration.

```c++
class Animal {
public:
    virtual void makeSound() {
        std::cout << "Animal sound\n";
    }
};

class Dog : public Animal {
public:
    void makeSound() override {
        std::cout << "Woof\n";
    }
};

int main() {
    Animal* animalPtr;  // Base class pointer
    Dog dogObj;         // Derived class object

    animalPtr = &dogObj;  // Pointing to derived class object
    animalPtr->makeSound();  // Output: "Woof"

    return 0;
}
```

In this example, the base class (`Animal`) pointer (`animalPtr`) is pointing to an object of its derived class (`Dog`). When the `makeSound` method is invoked through the base class pointer, the derived class's version of `makeSound` is executed, demonstrating polymorphism.
### References to Base Class

Just like pointers, references can also be used for achieving polymorphism. The mechanism is very similar.

Here's a simple example using references:

```c++
void animalSound(Animal& animalRef) {
    animalRef.makeSound();
}

int main() {
    Dog dogObj;
    animalSound(dogObj);  // Output: "Woof"

    return 0;
}
```

In this example, `animalSound` accepts a reference to an `Animal`. When you pass an object of the derived class `Dog`, it ends up calling the `Dog` version of `makeSound`.

## Complex Example:

```c++
#include <iostream>
#include <string>

class Animal {
public:
    std::string name;
    Animal(std::string n) : name(n) {}

    virtual std::string speak() const {
        return "???";
    }
};

class Cat : public Animal {
public:
    Cat(std::string n) : Animal(n) {}

    std::string speak() const override {
        return "Meow";
    }
};

class Dog : public Animal {
public:
    Dog(std::string n) : Animal(n) {}

    std::string speak() const override {
        return "Woof";
    }
};

void report(const Animal& animal) {
    std::cout << animal.name << " says " << animal.speak() << '\n';
}

int main() {
    Cat cat("Fred");
    Dog dog("Garbo");

    report(cat);
    report(dog);

    return 0;
}
```

Output: 
```
Fred says ???
Garbo says ???
```

Here’s the equivalent class with the speak() function made virtual:

```c++
#include <iostream>
#include <string>

class Animal {
public:
    std::string name;
    Animal(std::string n) : name(n) {}

    virtual std::string speak() const {
        return "???";
    }
};

class Cat : public Animal {
public:
    Cat(std::string n) : Animal(n) {}

    virtual std::string speak() const override {
        return "Meow";
    }
};

class Dog : public Animal {
public:
    Dog(std::string n) : Animal(n) {}

    virtual std::string speak() const override {
        return "Woof";
    }
};

void report(const Animal& animal) {
    std::cout << animal.name << " says " << animal.speak() << '\n';
}

int main() {
    Cat cat("Fred");
    Dog dog("Garbo");

    report(cat);
    report(dog);

    return 0;
}
```

Output
```
Fred says Meow
Garbo says Woof
```

It works!

When animal.speak() is evaluated, the program notes that Animal::speak() is a virtual function. In the case where animal is referencing the Animal portion of a Cat object, the program looks at all the classes between Animal and Cat to see if it can find a more derived function. In that case, it finds Cat::speak(). In the case where animal references the Animal portion of a Dog object, the program resolves the function call to Dog::speak().

Note that we didn’t make Animal::getName() virtual. This is because getName() is never overridden in any of the derived classes, therefore there is no need.