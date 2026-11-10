# Object Slicing

Object slicing in C++ occurs when an object of a derived class is assigned to an object of the base class, resulting in the loss of any member variables or functionalities that are specific to the derived class. This phenomenon can also occur when passing derived objects by value to a function that accepts base objects.

Consider the following classes:

```c++
class Base {
public:
    int base_var;
    Base(int a) : base_var(a) {}
    virtual void show() { std::cout << "Base: " << base_var << std::endl; }
};

class Derived : public Base {
public:
    int derived_var;
    Derived(int a, int b) : Base(a), derived_var(b) {}
    void show() override { std::cout << "Derived: " << base_var << ", " << derived_var << std::endl; }
};

int main(){
	Derived derived(10, 20);
	Base base = derived; // Object slicing happens here
	base.show();
	return 0;
}
```

Output:

```
Base: 10
```

In this case, when `base = derived` is executed, the `Base` part of `Derived` is copied into `base`, but `derived_var` is lost. The `show` method will output "Base: 10", even though the original `Derived` object had additional data (`derived_var = 20`) and functionality.

### Why is it a Problem?

1. **Loss of Information**: When slicing occurs, all the data members and overridden methods in the derived class that aren't present in the base class are lost.

2. **Unexpected Behavior**: Object slicing can lead to bugs that are hard to diagnose because it's not always obvious that slicing is occurring.
### How to Avoid Object Slicing

**Use Pointers or References**: One way to avoid object slicing is to use pointers or references, which preserve the type information:

```c++
Derived derived(10, 20);
Base& ref = derived; // No slicing here
ref.show(); // Output: "Derived: 10, 20"
```