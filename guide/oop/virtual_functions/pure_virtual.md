# Pure Virtual Functions

In C++, a pure virtual function is a virtual function that you declare in a base class but don't define. A class containing a pure virtual function is called an abstract class, and you can't create objects of an abstract class. The primary purpose of a pure virtual function (and, by extension, an abstract class) is to define an interface that derived classes must implement.
### Syntax

To declare a pure virtual function, you append `= 0` to the declaration, like this:

```c++
class Shape {
public:
    virtual void draw() = 0; // pure virtual function
};
```
### Abstract Class

Here, `Shape` is an abstract class because it has a pure virtual function. You can't create an object of type `Shape`. You can only create pointers or references of type `Shape`.
### Derived Classes

Any derived class must implement all of the base class's pure virtual functions. If it doesn't, the derived class is also considered abstract, and you can't create objects of that class either.

```c++
class Circle : public Shape {
public:
    void draw() override {
        // draw a circle
    }
};

class Square : public Shape {
public:
    void draw() override {
        // draw a square
    }
};
```

Both `Circle` and `Square` are concrete classes (i.e., non-abstract) because they provide implementations for the `draw` function.
### Usage

Pure virtual functions are commonly used for defining interfaces in C++. For instance, you can define a `Drawable` interface with a `draw` method, and any class that needs to be drawable would derive from this interface and implement the `draw` method.

```c++
Shape* shape = new Circle();
shape->draw(); // Calls Circle's draw implementation
```