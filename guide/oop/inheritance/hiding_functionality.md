# Hiding Inherited Functionality

In C++, it is not possible to remove or restrict functionality from a base class other than by modifying the source code. However, in a derived class, it is possible to hide functionality that exists in the base class, so that it can not be accessed through the derived class. This can be done simply by changing the relevant access specifier.

For example, we can make a public member private:

```c++
#include <iostream>

class Base
{
public:
	int m_value{};
};

class Derived : public Base
{
private:
	using Base::m_value;

public:
	Derived(int value) : Base { value }
	{
	}
};

int main()
{
	Derived derived{ 7 };
	std::cout << derived.m_value; // error: m_value is private in Derived

	Base& base{ static_cast<Base&>(derived) };
	std::cout << base.m_value; // okay: m_value is public in Base

	return 0;
}
```

This allowed us to take a poorly designed base class and encapsulate its data in our derived class. Alternatively, instead of inheriting Base’s members publicly and making m_value private by overriding its access specifier, we could have inherited Base privately, which would have caused all of Base’s member to be inherited privately in the first place.

However, it is worth noting that while m_value is private in the Derived class, it is still public in the Base class. Therefore the encapsulation of m_value in Derived can still be subverted by casting to Base& and directly accessing the member.

Perhaps surprisingly, given a set of overloaded functions in the base class, there is no way to change the access specifier for a single overload. You can only change them all:

```c++
#include <iostream>

class Base
{
public:
	int m_value{};

    int getValue() { return m_value; }
    int getValue(int) { return m_value; }
};

class Derived : public Base
{
private:
	using Base::getValue; // make ALL getValue functions private

public:
	Derived(int value) : Base { value }
	{
	}
};

int main()
{
	Derived derived{ 7 };
	std::cout << derived.getValue();  // error: getValue() is private in Derived
	std::cout << derived.getValue(5); // error: getValue(int) is private in Derived

	return 0;
}
```

## Marking functions as deleted

You can also mark member functions as deleted in the derived class, which ensures they can’t be called at all through a derived object:

```c++
#include <iostream>
class Base
{
private:
	int m_value {};

public:
	Base(int value)
		: m_value { value }
	{
	}

	int getValue() const { return m_value; }
};

class Derived : public Base
{
public:
	Derived(int value)
		: Base { value }
	{
	}


	int getValue() const = delete; // mark this function as inaccessible
};

int main()
{
	Derived derived { 7 };

	// The following won't work because getValue() has been deleted!
	std::cout << derived.getValue();

	return 0;
}
```

In the above example, we’ve marked the getValue() function as deleted. This means that the compiler will complain when we try to call the derived version of the function.