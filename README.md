# CPP Guide 📚

### 👋 Welcome!

The CPP Guide repository is meant to be a **guide / quick reference** for C++ developers where I try to get to the point so you can spend more time programming. This guide is **Markdown**, so it is easily opened using **Visual Studio Code** or **Github**. It contains extensive documentation, code examples and best practices.

### 📝 A Note on Reading This Guide

Before you dive in, it's important to understand that this guide is **structured by topic**, not by difficulty. There is **no prescribed "beginner to advanced" path**. Feel free to jump to any section that is relevant to your needs. A developer needing to understand `std::vector` can go straight there, while another might need to brush up on "RAII" or "CMake Presets."

### 🚀 Execute Code Examples 

Explore the `📁 examples` directory for **interactive C++** code examples, easily executable inside a **Docker DevContainer** within **Visual Studio Code**.  This setup ensures a consistent and streamlined coding environment, ideal for learning and experimentation.

## 🗂️ Table of Contents 

<table width="100%" style="border: none;">
<tr valign="top">
<td width="33%">

🛠️ **CMake & Tooling**

- Compilation Process
  - [Compilation Process](./guide/tools/compilation/compilation_process.md)
  - [Compiler Overview](./guide/tools/compilation/compilers.md)
  - [Build Generators](./guide/tools/compilation/build_generators.md)
  - [Dependency Management](./guide/tools/compilation/dependency_management.md)
  - [ABI (Application Binary Interface)](./guide/tools/compilation/abi.md)
- CMake
  - [Introduction](./guide/tools/cmake/introduction.md)
  - [Generators](./guide/tools/cmake/generators.md)
  - [Variables](./guide/tools/cmake/variables.md)
  - [Targets](./guide/tools/cmake/targets.md)
  - [Presets](./guide/tools/cmake/cmakepresets.md)
  - Macros and Functions (todo)
  - [Best Practices](./guide/tools/cmake/best_practices.md)
- Tooling
  - [Common Tools](./guide/tools/clang/tooling.md)
  - [H and CPP Files](./guide/oop/h_and_cpp_files.md)
</td>

<td width="33%">

📜 **Strings & Streams**

- Strings
  - [String Literals](./guide/strings/string_literals.md)
  - [std::string](./guide/strings/std_string.md)
  - [Character Encoding](./guide/strings/character_encoding.md)
  - [C Style Strings](./guide/strings/c_style_strings.md)
  - [std::string_view](./guide/strings/string_view.md)
- Streams
  - [IO Streams](./guide/strings/io_streams.md)
  - [String Streams](./guide/strings/string_streams.md)
  - [std::format](./guide/strings/std_format.md)
  - [printf](./guide/strings/printf.md)
- Files
  - [File IO](./guide/strings/file_io.md)
  - [File Streams](./guide/strings/file_streams.md)

</td>

<td width="33%">

📦 **Containers**

- Sequence Containers
  - [C-Style Arrays](./guide/containers/sequence/c_style_arrays.md)
  - [Loops](./guide/containers/sequence/loops.md)
  - [std::array](./guide/containers/sequence/std_array.md)
  - [Dynamic Allocation](./guide/containers/sequence/dynamic_allocation.md)
  - [Iterators](./guide/containers/sequence/iterators.md)
  - [Standard Library Functions (STL)](./guide/containers/sequence/standard_library_functions.md)
  - [std::list](./guide/containers/sequence/std_list.md)
  - [std::deque](./guide/containers/sequence/std_deque.md)
- Associative Containers
  - [std::set](./guide/containers/associative/std_set.md)
  - [std::map](./guide/containers/associative/std_map.md)
  - [std::multiset](./guide/containers/associative/std_multiset.md)
  - [std::multimap](./guide/containers/associative/std_multimap.md)
- Unordered Associative Containers
  - [std::unordered_set](./guide/containers/unordered_associative/std_unordered_set.md)
  - [std::unordered_map](./guide/containers/unordered_associative/std_unordered_map.md)
  - [std::unordered_multiset](./guide/containers/unordered_associative/std_unordered_multiset.md)
  - [std::unordered_multimap](./guide/containers/unordered_associative/std_unordered_multimap.md)
- Other
  - [std::span](./guide/containers/std_span.md)
  - [Container Adaptors](./guide/containers/container_adaptors.md)
</td>
</tr>
</table>

🏛️ **Object Oriented Programming OOP**

<table width="100%" style="border: none;">
<tr valign="top">
<td width="25%" style="padding-bottom: 20px;">

**General**

- [Classes vs Structs](./guide/oop/classes_vs_structs.md)
- [Constructors](./guide/oop/constructors.md)
- [Destructors](./guide/oop/destructors.md)
- [Move and Copy](./guide/oop/move_and_copy.md)
- [Default and Delete](./guide/oop/default_delete.md)
- [Rule of Five](./guide/oop/rule_of_five.md)
- [Static](./guide/oop/static.md)
- [Friend](./guide/oop/friend.md)
- [Nesting Classes](./guide/oop/nesting_classes.md)
- [Object Relationships](./guide/oop/object_relationships.md)
</td>

<td width="25%" style="padding-bottom: 20px;">

**Operator Overloading**

- [Introduction](./guide/oop/operator_overloading/introduction.md)
- [Friend, Member and Normal Function Overloading](./guide/oop/operator_overloading/friend_vs_member_vs_normal_function_overloading.md)
- [In and Out operators](./guide/oop/operator_overloading/in_and_out_operators.md)
- [Assignment Operator](./guide/oop/operator_overloading/assignment_operator.md)
</td>

<td width="25%" style="padding-bottom: 20px;">

**Inheritance**

- [Introduction](./guide/oop/inheritance/introduction.md)
- [Order of Construction](./guide/oop/inheritance/order_of_construction.md)
- [Access Specifiers](./guide/oop/inheritance/access_specifiers.md)
- [Functions and Overriding Behavior](./guide/oop/inheritance/functions_and_overriding_behavior.md)
- [Hiding Functionality](./guide/oop/inheritance/hiding_functionality.md)
</td>

<td width="25%" style="padding-bottom: 20px;">

**Virtual Functions**

- [Introduction](./guide/oop/virtual_functions/virtual_functions.md)
- [Override and Final Specifier](./guide/oop/virtual_functions/override_and_final_specifier.md)
- [Covariant Return Types](./guide/oop/virtual_functions/covariant_return_types.md)
- [Destructors](./guide/oop/virtual_functions/destructors.md)
- [Pure Virtual](./guide/oop/virtual_functions/pure_virtual.md)
- [Multiple Inheritance](./guide/oop/virtual_functions/multiple_inheritance.md)
- [Object Slicing](./guide/oop/virtual_functions/object_slicing.md)
</td>
</tr>
</table>

<table width="100%" style="border: none;">
<tr valign="top">
<td width="33%">

🧠 **Memory Management**

- [References](./guide/memory/references.md)
- [Pointers](./guide/memory/pointers.md)
- [Smart Pointers](./guide/memory/smart_pointers.md)
- [How To Use Smart Pointers With Functions](./guide/memory/smart_pointers_in_functions.md)
- [Shallow Copy vs Deep Copy](./guide/memory/shallow_copy_vs_deep_copy.md)
</td>

<td width="33%">

🎭 **Functions**

- [Function Pointers](./guide/functions/function_pointers.md)
- [Lambdas](./guide/functions/lambdas.md)
- [std::function](./guide/functions/std_function.md)
</td>

<td width="33%">

🔗 **Typing, Casting & ConstExpr**

- [Auto](./guide/typing/auto.md)
- Casting
  - [implicit_casting](./guide/typing/casting/implicit_casting.md)
  - [explicit_casting](./guide/typing/casting/explicit_casting.md)
  - [static_cast](./guide/typing/casting/static_cast.md)
  - [dynamic_cast](./guide/typing/casting/dynamic_cast.md)
- [Using constexpr](./guide/typing/using_constexpr.md)
- [Using constval](./guide/typing/using_constval.md)
</td>
</tr>

<tr valign="top">
<td width="33%">

⚡ **Concurrency**

- [Introduction](./guide/concurrency/introduction.md)
- STL Threads
  - [std::thread](./guide/concurrency/stl_threads/thread.md)
  - [std::async and std::future](./guide/concurrency/stl_threads/async_and_future.md)
- [POSIX vs STL Threads](./guide/concurrency/posix_vs_stl_threads.md)
</td>

<td width="33%">

🎨 **Design Patterns & Idioms**

- Idioms
  - [RAII](./guide/design_patterns/raii.md)
  - [PIMPL](./guide/design_patterns/pimpl.md)
</td>

<td width="33%">

🚧 **Todo**

- Design patterns
- Templates
- Error handling

</td>
</tr>
</table>

## 📍 References

<table width="100%" style="border: none;">
<tr valign="top">
<td width="50%">

### General

- **CPP Reference**
    - This is more of a reference but contains examples and explanations that can serve as a learning resource.
    - [Link here](https://en.cppreference.com/w/)
- **Online C++ Compiler**
    - [Link here](https://www.tutorialspoint.com/compile_cpp_online.php)
- **C++ Core Guidelines**
  - [Link here](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- **Google C++ Style Guide**
  - [Link here](https://google.github.io/styleguide/cppguide.html)

---

### Tutorials

- **LearnCpp**
    - Devoted to teaching you how to program in C++. Whether you’ve had any prior programming experience or not, the tutorials on this site will walk you through all the steps to write, compile, and debug your C++ programs, all with plenty of examples.
    - [Link here](https://www.learncpp.com/)
- **GeeksforGeeks**
    - Offers a variety of tutorials and exercises ranging from basic to advanced topics.
    - [Link here](https://www.geeksforgeeks.org/c-plus-plus/?ref=shm_outind)

---

### Interviews

- **C++ Interview Questions and Answers** (geeksforgeeks.org)
  - [Link here](https://www.geeksforgeeks.org/cpp-interview-questions/)

</td>
<td width="50%">

### Books

- **Software Architecture with C++** (Paid)
  - Design modern systems using effective architecture concepts, design patterns, and techniques with C++20. By Adrian Ostrowski & Piotr Gaczkowski, April 2021.
  - [Link here](https://www.packtpub.com/product/software-architecture-with-c/9781838554590)
- **Professional C++** (Paid)
  - Professional C++, 5th Edition raises the bar for advanced programming manuals. By Marc Gregoire, Feb 2021.
  - [Link here](https://www.amazon.com.be/-/en/Marc-Gregoire/dp/1119695406)
- **Modern CMake for C++** (Paid)
  - Write comprehensive, professional-standard CMake projects. By Rafal Świdziński, May 2024.
  - [Link here](https://www.packtpub.com/en-be/product/modern-cmake-for-c-9781805123361)
- **CMake Best Practices** (Paid)
  - Write comprehensive, professional-standard CMake projects. By Dominik Berner & Mustafa Kemal Gilor, Aug 2024.
  - [Link here](https://www.packtpub.com/en-be/product/cmake-best-practices-9781835880654)
- **Advanced C and C++ Compiling** (Paid)
  - Understand the structure and purpose of the binary files produced by the compiler.
  - [Link here](https://www.amazon.com/Advanced-C-Compiling-Milan-Stevanovic/dp/1430266678)
- **Learning C++** (Free)
  - Free unaffiliated eBook created from Stack Overflow contributors.
  - [Link here](https://riptutorial.com/Download/cplusplus.pdf)
- **More C++ Idioms** (Free)
  - [Link here](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms)
- **C++ Programming: Code patterns design** (Free)
  - [Link here](https://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns)
- **C++ Notes for Professionals book** (Free)
  - [Link here](https://goalkicker.com/CPlusPlusBook/)

</td>
</tr>
</table>