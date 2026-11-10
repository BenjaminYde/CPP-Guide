# Functions and Overriding Behavior

When a derived class inherits from a base class, it gains access to the members and methods of that base class, depending on the access specifiers. Inherited member functions can be called just like any other member functions of the derived class.

Here's an example to illustrate calling an inherited function:

```c++
#include <iostream>

class Base
{
protected:
    int m_value {};

public:
    Base(int value)
        : m_value { value }
    {
    }

    void identify() const { 
	    std::cout << "I am a Base\n"; 
    }
};

class Derived: public Base
{
public:
    Derived(int value)
        : Base { value }
    {
    }
};

int main()
{
    Base base { 5 };
    base.identify();

    Derived derived { 7 };
    derived.identify();

    return 0;
}
```

Output:
```
I am a Base
I am a Base
```

When derived.identify() is called, the compiler looks to see if function identify() has been defined in the Derived class. It hasn’t. Then it starts looking in the inherited classes (which in this case is Base). Base has defined an identify() function, so it uses that one. In other words, Base::identify() was used because Derived::identify() doesn’t exist.

### Redefining behaviors

However, if we had defined Derived::identify() in the Derived class, it would have been used instead. This means that we can make functions work differently with our derived classes by redefining them in the derived class!

```c++
class Derived: public Base
{
public:
    Derived(int value)
        : Base { value }
    {
    }

    int getValue() const { return m_value; }

    // Here's our modified function
    void identify() const { 
	    std::cout << "I am a Derived\n"; 
    }
};
```

Output:
```
I am a Base
I am a Derived
```

Note that when you redefine a function in the derived class, the derived function does not inherit the access specifier of the function with the same name in the base class.

## Adding existing behavior

Sometimes we don’t want to completely replace a base class function, but instead want to add additional functionality to it. In the above example, note that Derived::identify() completely hides Base::identify()! This may not be what we want. It is possible to have our derived function call the base version of the function of the same name (in order to reuse code) and then add additional functionality to it.

```c++
#include <iostream>

class Derived: public Base
{
public:
    Derived(int value)
        : Base { value }
    {
    }

    int getValue() const  { return m_value; }

    void identify() const
    {
        Base::identify(); // call Base::identify() first
        std::cout << "I am a Derived\n"; // then identify ourselves
    }
};
```

Output:
```
I am a Base
I am a Base
I am a Derived
```