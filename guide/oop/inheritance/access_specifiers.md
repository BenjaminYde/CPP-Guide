# Access Specifiers in Inheritance

Inheritance allows a class (derived class) to inherit attributes and methods from another class (base class). It enables code reusability and can be used to implement relationships between classes like "is-a" or "kind-of".

```c++
// Base class
class Animal {
public:
    void eat() {
        cout << "I can eat." << endl;
    }
};

// Derived class
class Dog : public Animal {
public:
    void bark() {
        cout << "I can bark." << endl;
    }
};

int main() {
    Dog myDog;
    myDog.eat();  // inherited from Animal
    myDog.bark(); // belongs to Dog
    return 0;
}
```

- **Public Inheritance**: 
	- The public members of the base class become public members
	- The protected members of the base class become protected members 
	- Private members of the base class are not accessible in the derived class.

- **Protected Inheritance**: 
	- Both the public and protected members of the base class become protected members of the derived class. 
	- Private members of the base class are not accessible in the derived class.

- **Private Inheritance**: 
	- Both the public and protected members of the base class become private members of the derived class. 
	- Private members of the base class are not accessible in the derived class.

Here's an example illustrating this:

```c++
#include <iostream>
using namespace std;

class Base {
public:
    int pub_var;
    Base() : pub_var(1), prot_var(2), priv_var(3) {}
protected:
    int prot_var;
private:
    int priv_var;
};

// Public Inheritance
class DerivedPublic : public Base {
public:
    void show() {
        cout << "DerivedPublic:\n";
        cout << "Public Variable: " << pub_var << endl;
        cout << "Protected Variable: " << prot_var << endl;
        // cout << "Private Variable: " << priv_var << endl; // Compilation error
    }
};

// Protected Inheritance
class DerivedProtected : protected Base {
public:
    void show() {
        cout << "DerivedProtected:\n";
        cout << "Public Variable: " << pub_var << endl;
        cout << "Protected Variable: " << prot_var << endl;
        // cout << "Private Variable: " << priv_var << endl; // Compilation error
    }
};

// Private Inheritance
class DerivedPrivate : private Base {
public:
    void show() {
        cout << "DerivedPrivate:\n";
        cout << "Public Variable: " << pub_var << endl;
        cout << "Protected Variable: " << prot_var << endl;
        // cout << "Private Variable: " << priv_var << endl; // Compilation error
    }
};

int main() {
    DerivedPublic d1;
    DerivedProtected d2;
    DerivedPrivate d3;

    // Access via DerivedPublic object
    cout << "d1.pub_var (DerivedPublic): " << d1.pub_var << endl;
    // cout << "d1.prot_var: " << d1.prot_var << endl; // Compilation error
    // cout << "d1.priv_var: " << d1.priv_var << endl; // Compilation error
    
    // Access via DerivedProtected object
    // cout << "d2.pub_var: " << d2.pub_var << endl; // Compilation error
    // cout << "d2.prot_var: " << d2.prot_var << endl; // Compilation error
    // cout << "d2.priv_var: " << d2.priv_var << endl; // Compilation error
    
    // Access via DerivedPrivate object
    // cout << "d3.pub_var: " << d3.pub_var << endl; // Compilation error
    // cout << "d3.prot_var: " << d3.prot_var << endl; // Compilation error
    // cout << "d3.priv_var: " << d3.priv_var << endl; // Compilation error

    // Access via show() method
    d1.show();
    d2.show();
    d3.show();

    return 0;
}
```