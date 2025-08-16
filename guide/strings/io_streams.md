# I/O Streams

At its core, I/O (Input/Output) is how your program communicates with the outside world. In C++, this is primarily handled through a concept called streams. Think of a stream as a sequence of bytes flowing from a source to a destination.

> [!TIP]
> Make sure to checkout the runnable code examples: `code_examples/strings/io_streams` here!

## Basics

- **Header**: For standard console I/O, you must include` <iostream>`.
-  **Namespace**: All standard stream objects reside in the `std` namespace.

```c++
#include <iostream>

// Good practice: Be explicit with the namespace.
std::cout << "Hello, World!" << std::endl;
```

C++ provides four global stream objects to interact with the console by default:

- `std::cout`: Output (Buffered)
- `std::cin`: Input
- `std::cerr`: Error (Unbuffered)
- `std::clog`: Log (Buffered)

## Buffered vs Unbuffered IO

An I/O operation (like writing to the console) can be slow. To improve efficiency, many systems use a buffer, which is a temporary block of memory.

### 1. Buffered IO 

When you write to a buffered stream, your data isn't sent directly to the destination (e.g., the console). Instead, it's quickly placed in the buffer. The system then "flushes" this buffer—writing all its contents at once—only when it's full or when explicitly told to do so.

Flushing occurs automatically when:

1. **Buffer Full**: When the buffer is full, it automatically flushes its content.
2. **Manual Flush**: You can manually flush the buffer using `std::flush` or `std::endl`.
3. **Program Termination**: Buffers usually flush automatically when the program terminates successfully.

#### Pro's and Con's:

- **Efficiency**: Reading or writing large chunks at once is generally faster than one character at a time.
- **Control**: You have the opportunity to manipulate the data while it’s in the buffer.
- **Data Loss**: If your program crashes, any data still in the buffer might be lost forever without ever being displayed.

### 2. Unbuffered IO

When you write to an unbuffered stream, the data is sent directly to the destination immediately. There is no delay and no intermediate buffer.

#### Pro's and Con's:

- **Immediacy**: Output is immediate, which is useful for time-sensitive data.
- **Simplicity**: There is no need to manage a buffer, which makes the code somewhat simpler for small tasks.
- **Performance**: Can be less efficient for large volumes of output, as each write operation can trigger a separate, slow system call.

## Output: `std::cout`

### What is it?

- **Basic Usage**: Output text or variables to the console using the insertion operator `(<<)` to send data of any standard type to its buffer.

- **Buffered** by default, which means the output may be held in a buffer until the buffer is flushed. You can manually flush it using `std::flush` or `std::endl`.

### Example

```c++
#include <iostream>
#include <string>

std::cout << "Hello, World!" << std::endl; // Flushes the buffer automatically

std::string name = "Alice";
int age = 30;
// Chaining operators to build a single output line in the buffer
std::cout << "User: " << name << ", Age: " << age << std::endl;
```

### Performance Impact: The `\n` vs. `std::endl` Impact

A common but inefficient practice is to use `std::endl` everywhere. 

`std::endl` does two things: it inserts a newline character (`\n`) and it forces a buffer flush. In loops, this is a performance killer.

```c++
// Inefficient: This forces a slow flush operation on every single iteration.
for (int i = 0; i < 1000; ++i) {
    std::cout << "Processing item " << i << std::endl;
}
```

The Right Way:

Use the '`\n`' character for newlines and let the buffer manage itself. The performance difference is significant.

```c++
// Efficient: Data is buffered and written in larger, faster chunks.
for (int i = 0; i < 1000; ++i) {
    std::cout << "Processing item " << i << '\n';
}
std::cout << std::flush;  // Manually flush the buffer
```

## Input: `std::cin`

### What is it?

The standard input stream for reading from the keyboard.

The `>>` operator reads from the keyboard until it hits a space, tab, or the Enter key (whitespace). It only reads one "word" or one "number" at a time.

### Example: Reading a Full Name

This is the most common beginner mistake.

```c++
#include <iostream>
#include <string>

int main() {
    std::string fullName;
    std.cout << "Please enter your full name: ";
    // If you type "John Smith", cin >> only reads "John".
    std::cin >> fullName;
    std::cout << "Hello, " << fullName << "!" << std::endl; // Prints "Hello, John!"
    
    // The " Smith" is left behind, waiting to mess up the next cin!
    return 0;
}
```

### Example: Taking Multiple Inputs

The most common way to read data is with the extraction operator `(>>)`. It skips any leading whitespace and then reads characters until it encounters the next whitespace (a space, tab, or newline).

```c++
#include <iostream>
#include <string>

int main() {
    std::string name;
    int age;

    // Prompt the user for their first name and age
    std::cout << "Please enter your first name and age: ";

    // cin reads the first word into 'name', then the number into 'age'
    // Example Input: "Alice 30"
    std::cin >> name >> age;

    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;

    return 0;
}
```

### Example: Reading Full Lines with `std::getline`

Because `operator>>` stops at the first whitespace character, it is fundamentally unsuited for reading text that contains spaces, like a full name.

To reliably read an entire line of text, including spaces, you must use the `std::getline` function. It reads everything until it finds a newline character.


```c++
#include <iostream>
#include <string>

int main() {
    std::string fullName;
    std::cout << "Enter your full name: ";
    // Reads the entire line, including spaces, into fullName.
    std::getline(std::cin, fullName);
    std::cout << "Hello, " << fullName << "!" << std::endl; // Prints "Hello, John Smith!"
    return 0;
}
```

### Example: Error Handling

#### Success vs. Failure

`cin` isn't always looking for an integer. It looks at the type of your variable and tries to match the input to that type.

Let's see what happens when we try to read into an int.

```c++
#include <iostream>

int main() {
    int number;
    std::cout << "Enter an integer: ";
    std::cin >> number;
    
    // The if statement checks if cin is in a "good" state.
    if (std::cin) { // This is true if the read was successful.
        std::cout << "Success! You entered the number: " << number << std::endl;
    } else { // This is true if the read failed.
        std::cout << "Error! That was not a valid integer." << std::endl;
    }
    
    return 0;
}
```

- If you enter `123`: cin successfully reads the integer. The `if (std::cin)` check is `true`.
- If you enter `hello`: cin fails to read an integer. It enters a "fail state." The `if (std::cin)` check is `false`.

When `cin` fails, it stops working until you do two things:

- Reset the error flag.
- Clear out the bad input.

See the following complete example:


```c++
#include <iostream>
#include <limits> // Required for the .ignore() line

int main() {
    int number;
    std::cout << "Enter an integer: ";

    // The loop runs as long as the input operation fails.
    while (!(std::cin >> number)) {
        std::cout << "That's not a number! Please try again: ";
        
        // Step 1: Reset the error flag on cin.
        std::cin.clear();
        
        // Step 2: Clear the bad input from the buffer.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "You entered the valid number: " << number << std::endl;
    return 0;
}
```

## Error: `std::cerr`

### What is it?

- **Basic Usage**: It’s generally used to display error messages or diagnostic information.

- **Unbuffered**, meaning it writes data to the console immediately, making it ideal for reporting errors that might cause the program to terminate prematurely.

- **It Writes to Standard Error (`stderr`)**: By default, `cerr` directs its output to the **standard error channel**. This is a crucial feature that allows you to easily separate critical messages from your main program output (`std::cout`), which writes to **standard output (stdout)**.
  - For example, the following program `./my_program 1> result.txt 2> task.log` writes the console output to the `results.txt` file and the errors to the `task.log` file. Each number represents a channel where `1>` redirects standard output (`stdout`) and `2>` redirects standard error (`stderr`).

### Example

```c++
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::string filename = "non_existent_file.txt";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file '" << filename << "'\n";
        return 1;
    }
    return 0;
}
```

## Console Log: `std::clog`

### What is it?

- **Basic Usage**: `std::clog` is the standard C++ stream object designated for logging program information. It provides a dedicated channel for diagnostic messages that are not critical errors, such as progress updates or debug traces.

- **Buffered**, similar to `std::cout`. Output may be held until the buffer is flushed.

- **It Writes to Standard Error (`stderr`)**: By default, `cerr` directs its output to the **standard error channel**. This is a crucial feature that allows you to easily separate critical messages from your main program output (`std::cout`), which writes to **standard output (stdout)**.
  - For example, the following program `./my_program 1> result.txt 2> task.log` writes the console output to the `results.txt` file and the errors to the `task.log` file. Each number represents a channel where `1>` redirects standard output (`stdout`) and `2>` redirects standard error (`stderr`).

### Example

```c++
#include <iostream>
#include <chrono>
#include <thread>

void run_long_task() {
    // Use clog for detailed, non-critical progress updates.
    std::clog << "LOG: Task started. Preparing stage 1...\n";
    std::clog << "LOG: computation...\n";
    std::clog << "LOG: Task finished.\n";

    // The final, important result is sent to cout.
    std::cout << "Final Result: 42\n";
}

int main() {
    run_long_task();
    return 0;
}
```

When you run this program, you can easily separate the clean result from the verbose log using shell redirection:

`./my_program 1> result.txt 2> task.log`

`result.txt` will contain:

```
Final Result: 42
```

`task.log` will contain:

```
LOG: Task started. Preparing stage 1...
LOG: computation...
LOG: Task finished.
```
