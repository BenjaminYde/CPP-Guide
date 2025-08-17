# File Streams

File streams in C++ provide a mechanism for file manipulation, seamlessly integrating with the language's stream-based I/O system. They are a core part of the C++ Standard Library, found in the `<fstream>` header. The three primary classes for file operations are: `std::ifstream` for reading, std::ofstream for writing, and `std::fstream` for both reading and writing.

These classes are defined in the `<fstream>` header.

## Stream Types

### Input File Stream: `std::ifstream` 

**Purpose**: Used for reading data from files.

```c++
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream inputFile("example.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file for reading!" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::cout << line << std::endl;
    }

    // inputFile is closed automatically when it goes out of scope (RAII)
    return 0;
}
```

### Output File Stream: `std::ofstream`

**Purpose**: Used for writing data to files.

```c++
#include <fstream>
#include <iostream>

int main() {
    // std::ios::trunc is the default, which overwrites the file
    std::ofstream outputFile("example.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening file for writing!" << std.endl;
        return 1;
    }

    outputFile << "This is the first line.\n";
    outputFile << "This is the second line.\n";
    
    // outputFile is closed automatically at the end of its scope
    return 0;
}
```

### Input/Output File Stream: `std::fstream`

**Purpose**: Used for both reading and writing to/from files.

```c++
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::fstream file("example.txt", std::ios::in | std::ios::out | std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // Append to the file
    file << "Appending a new line.\n";

    // Move the read pointer to the beginning to read the whole file
    file.seekg(0, std::ios::beg);

    std::string line;
    std::cout << "File content after appending:\n";
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }

    return 0;
}
```

## RAII and File Streams

RAII (Resource Acquisition Is Initialization) is a core C++ concept that is fundamental to file streams. When a stream object is created, it acquires the file resource. When the object goes out of scope, its destructor is automatically called, which closes the file. This guarantees that files are closed, even if exceptions are thrown.

```c++
#include <fstream>
#include <iostream>

void process_file() {
    std::ofstream file("temp.txt"); // File is opened here
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    file << "Data";
    // No need to call file.close(). It's handled by the destructor.
} // file goes out of scope here and is automatically closed
```

## State Management

Every stream maintains a set of internal **state flags** to report on the health and status of stream operations. You should always check these flags after performing an I/O operation to ensure it succeeded.

The primary flags are:

| State Flag (`iostate`) | Member Function | Meaning | Common Cause |
| :--- | :--- | :--- | :--- |
| `goodbit` | `.good()` | No flags are set; the stream is ready for I/O operations. | The stream is newly opened or the last operation was successful. |
| `eofbit` | `.eof()` | The input operation reached the end of the file. | Attempting to read past the last character of a file. |
| `failbit` | `.fail()` | A logical error occurred; a formatting or conversion error prevented the operation from succeeding. The stream is potentially recoverable. | Trying to extract an `int` when the next characters in the file are letters (e.g., "abc"). |
| `badbit` | `.bad()` | A severe I/O error occurred, indicating a loss of integrity in the stream. The stream is likely unrecoverable. | A physical disk read error, or memory exhaustion while trying to buffer data. |

### Example: Checking Stream State

```c++
#include <fstream>
#include <iostream>

int main() {
    std::ifstream file("data.txt");

    // The idiomatic way to check if the file opened successfully.
    // This checks for failbit, which is set if the file doesn't exist.
    if (!file) { 
        std::cerr << "Error: Could not open file.\n";
        return 1;
    }

    int value;
    file >> value; // Attempt to read an integer

    if (file.eof()) {
        std::cout << "Finished reading: reached end of file.\n";
    } else if (file.fail()) {
        std::cout << "Error: Failed to read an integer from the file.\n";
        // The file might contain non-numeric data.
    } else if (file.good()) {
        std::cout << "Successfully read value: " << value << "\n";
    }
}
```

### Example: Using `clear()` and `exceptions()`

`clear()`: Resets the state flags. If a stream enters a fail state, all subsequent I/O operations will do nothing until you call `clear()` to restore the stream to a good state.

```c++
// Clearing an error state to continue reading
file.clear(); // Resets failbit/eofbit to goodbit
file.seekg(0, std::ios::beg); // Move back to the beginning
```

`exceptions()`: Configures the stream to automatically throw an `std::ios_base::failure` exception when a specific state flag is set. This allows you to centralize error handling using try/catch blocks instead of checking flags after every operation.


```c++
std::ifstream error_file;
// Tell the stream to throw an exception if failbit or badbit is set.
error_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

try {
    error_file.open("nonexistent.txt");
    // ... any other operations on error_file
} catch (const std::ios_base::failure& e) {
    // This block will catch the open failure or any subsequent I/O errors.
    std::cerr << "I/O Exception caught: " << e.what() << std::endl;
}
```

### What happens when you do NOT call `exceptions()`? (The Default Behavior)

If you do not enable exceptions, the stream operates in a "silent failure" mode. When an error occurs (like a file not opening, or a formatting error during a read), the following happens:

1. **No Exception is Thrown**: Your program's execution is not interrupted. The flow of control continues to the next line of code as normal.

1. **The Internal State Flag is Set**: The stream internally sets the appropriate flag (`failbit` or `badbit`).

1. **The Stream Enters a "Zombie" State**: Once a flag like `failbit` is set, the stream will refuse to perform any further I/O operations. It essentially becomes inert. All subsequent attempts to read from or write to it will do nothing.

This means it is your **responsibility to manually check the state of the stream** after any operation that might fail.

This is the workflow without `exceptions()`:

```c++
#include <fstream>
#include <iostream>

int main() {
    // DEFAULT BEHAVIOR: No exceptions are enabled.
    std::ifstream file("nonexistent.txt");

    // 1. Manual Check after opening
    if (!file) { // Checks if failbit was set during open
        std::cerr << "File failed to open. Program continues, but stream is unusable.\n";
        // The program doesn't crash here.
    }

    // 2. The stream is already in a failed state, so this operation does nothing.
    int value;
    file >> value; 

    // 3. Manual Check after reading
    if (file.fail()) {
        // This will be true because the open failed, and the failbit is still set.
        std::cerr << "Failed to read integer.\n";
    }
    
    // The program continues until it ends.
    return 0;
}
```

### Common Methods

- `open(filename, mode)`: Opens a file with the specified mode (e.g., `std::ios::in`, `std::ios::out`, `std::ios::app`, etc.)
- `close()`: Closes the file.
- `eof()`: Checks if the end of the file has been reached.
- `getline()`: Reads a line from the file into a string.
- `>>` and `<<` operators for reading and writing.
- `seekg()` and `seekp()` for moving the read and write pointers, respectively.
### Error Handling

File streams set flags that can be checked to see if operations succeeded.

- `fail()`: Returns true if the last operation failed.
- `bad()`: Returns true if the stream is in an invalid state.
- `good()`: Returns true if the stream is in a valid state and no flags are set.

Example:

```c++
std::ifstream file("nonexistent.txt");
if (file.fail()) {
    std::cerr << "File could not be opened!";
}
```
### Stream State Reset

You can clear the stream's error state using `clear()` and reset the get/put position using `seekg()` and `seekp()`.

```c++
file.clear();
file.seekg(0, std::ios::beg);
```

## Example: Exception Handling

```c++
#include <fstream>
#include <iostream>

int main() {
    std::ifstream file;
    // Make the stream throw an exception on failure
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open("nonexistent.txt");
        // ... operations on the file
    } catch (const std::ifstream::failure& e) {
        std::cerr << "Exception opening/reading file: " << e.what() << std::endl;
    }
    return 0;
}
```

## Example: Writing a Formatted Log File

A common requirement is to write formatted log messages or reports to a file. You can combine `std::format_to` with `std::ofstream` to achieve this efficiently.

**How it works**: You create a file stream object (`std::ofstream`) and then use an iterator, like `std::ostream_iterator`, to act as the bridge between `std::format_to` and the file.

```c++
#include <fstream>
#include <format>
#include <string>
#include <iterator> // Required for std::ostream_iterator

struct LogEntry {
    int event_id;
    std::string user;
    std::string message;
};

int main() {
    LogEntry entry1 = {101, "admin", "User logged in successfully."};
    LogEntry entry2 = {205, "guest", "Failed login attempt."};

    std::ofstream log_file("app.log");
    if (!log_file) {
        // Use std::println for console error messages
        std::println(stderr, "Error: Could not open log file.");
        return 1;
    }

    // Use an ostream_iterator to direct the output to the file
    auto file_iter = std::ostream_iterator<char>(log_file);

    // Write the first log entry using std::format_to
    std::format_to(file_iter, "[ID: {}] User '{}': {}\n", 
                   entry1.event_id, entry1.user, entry1.message);

    // Write the second log entry
    std::format_to(file_iter, "[ID: {}] User '{}': {}\n", 
                   entry2.event_id, entry2.user, entry2.message);

    return 0;
}
```

Contents of `app.log`:

```
[ID: 101] User 'admin': User logged in successfully.
[ID: 205] User 'guest': Failed login attempt.
```

## Example: Building a Dynamic SQL Query

```c++
#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <iterator> // Required for std::back_inserter

int main() {
    std::string query = "SELECT id, name FROM users WHERE ";
    std::vector<std::string> conditions;

    conditions.push_back("status = 'active'");
    conditions.push_back("level > 5");
    
    // Use std::format_to with a back_inserter to append to the string
    auto string_inserter = std::back_inserter(query);

    for (size_t i = 0; i < conditions.size(); ++i) {
        if (i > 0) {
            std::format_to(string_inserter, " AND ");
        }
        std::format_to(string_inserter, "({})", conditions[i]);
    }
    std::format_to(string_inserter, ";");

    // Print final string
    std::cout << "Generated Query: " << query << std::endl;
    return 0;
}
```

```
Generated Query: SELECT id, name FROM users WHERE (status = 'active') AND (level > 5);
```

## Example: Binary File Operations

File streams can also handle binary data, which is crucial for reading and writing non-text files like images or executables, or for storing data structures efficiently.

```c++
#include <fstream>
#include <iostream>

struct Person {
    char name[50];
    int age;
};

int main() {
    Person person = {"John Doe", 30};

    // Write to a binary file
    std::ofstream outFile("person.bin", std::ios::binary);
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(&person), sizeof(Person));
    }
    outFile.close();

    // Read from a binary file
    Person readPerson;
    std::ifstream inFile("person.bin", std::ios::binary);
    if (inFile.is_open()) {
        inFile.read(reinterpret_cast<char*>(&readPerson), sizeof(Person));
        std::cout << "Name: " << readPerson.name << ", Age: " << readPerson.age << std::endl;
    }
    inFile.close();
    
    return 0;
}
```