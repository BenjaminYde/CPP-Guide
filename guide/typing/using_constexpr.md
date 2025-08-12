## Runtime Costs

Consider the following short program:

```c++
int x { 3 + 4 };
std::cout << x << '\n';
```

If this program were compiled exactly as it was written (with no optimizations), the compiler would generate an executable that calculates the result of `3 + 4` at runtime (when the program is run). If the program were executed a million times, `3 + 4` would be evaluated a million times, and the resulting value of `7` produced a million times. But note that the result of `3 + 4` never changes -- it is always `7`. So re-evaluating `3 + 4` every time the program is run is wasteful.

## const

When you declare a const variable, the compiler will implicitly keep track of whether it’s a runtime or compile-time constant. In most cases, this doesn’t matter for anything other than optimization purposes, but there are a few cases where C++ requires a constant expression (we’ll cover these cases later as we introduce those topics). And only compile-time constant variables can be used in a constant expression.

Because compile-time constants also allow for better optimization (and have little downside), we typically want to use compile-time constants wherever possible.

When using `const`, our variables could end up as either a compile-time const or a runtime const, depending on whether the initializer is a compile-time expression or not. In some cases, it can be hard to tell whether a const variable is a compile-time const (and usable in a constant expression) or a runtime const (and not usable in a constant expression).

For example:

```c++
int x { 5 };       // not const at all
const int y { x }; // obviously a runtime const (since initializer is non-const)
const int z { 5 }; // obviously a compile-time const (since initializer is a constant expression)
const int w { getValue() } // not obvious whether this is a runtime or compile-time const
```
In the above example, `w` could be either a runtime or a compile-time const depending on how `getValue()` is defined. It’s not at all clear!

Fortunately, we can enlist the compiler’s help to ensure we get a compile-time const where we desire one. To do so, we use the `constexpr` keyword instead of `const` in a variable’s declaration. A constexpr (which is short for “constant expression”) variable can only be a compile-time constant. If the initialization value of a constexpr variable is not a constant expression, the compiler will error.

## constexpr

`Constant folding` is a compiler optimization technique that aims to calculate the results of constant expressions during compile time rather than runtime. It can speed up the program by performing calculations in advance, thus reducing the work that the program has to do when it runs.

C++ compilers are generally very good at constant folding. When they see expressions involving only literal constants or `constexpr` variables, they calculate the results of these expressions during compilation and substitute the results in the generated code.

```c++
constexpr int a = 2;
constexpr int b = 3;
constexpr int c = a * b + 5;
```

The compiler will see that `a * b + 5` can be calculated at compile time (since `a` and `b` are `constexpr`, their values are known at compile time). Therefore, it will perform the calculation itself and replace the expression with its result. 

`constexpr` variable is implicitly `const`, meaning that its value cannot be changed after it has been initialized. This is because `constexpr` is used to declare constants that can be evaluated at compile time. Since these values are known and fixed during the compilation process, it would not make sense to allow them to be modified during runtime.

```c++
constexpr int a = 5;
a = 10; // Error: cannot assign to variable 'a' with const-qualified type 'const int'
```

`constexpr` has the requirement that all variables / functions need to be `constexpr`.

```c++
constexpr double gravity { 9.8 }; // ok: 9.8 is a constant expression
constexpr int sum { 4 + 5 };      // ok: 4 + 5 is a constant expression
constexpr int something { sum };  // ok: sum is a constant expression

int age{24};
constexpr int myAge { age }; // compile error: age is not a constant expression
constexpr int f { five() }; // compile error: return value of five() is not a constant expression
```

## const vs constexpr

The difference is that `constexpr` is used for values that are determined at compile time, which can be used in contexts that require constant expressions, like array sizes or template arguments. While `const` variables can also be initialized with compile-time constants, they are not required to be. They are mainly used to specify that the variable should not be modifiable throughout its scope after its initialization.