# File IO

The C++17 standard introduced the `<filesystem>` library, which provides a robust and platform-independent way of handling file and directory operations. This library is largely about manipulating file paths, directories, and file attributes, rather than reading or writing file content. For content manipulation, you should use file streams (`<fstream>`).

## Core Concepts

At its heart, the `<filesystem>` library revolves around the `std::filesystem::path` object. This object represents a path on a filesystem and provides a consistent way to manipulate and query paths, regardless of the underlying operating system.

- `std::filesystem::path:` Represents a file or directory path. It's the central component of the library.

- `std::filesystem::directory_entry`: Represents an entry within a directory. It holds a path object and provides access to its status and attributes.

- `std::filesystem::directory_iterator`: Allows you to iterate over the entries in a directory.

- `std::filesystem::recursive_directory_iterator`: Allows you to iterate over the entries in a directory and its subdirectories.

## Path Object

### Path Manipulations

The `path` object provides a rich set of methods for deconstruction and manipulation.

**Concatenation**: Use the `/` operator to append to a path. This is the preferred, platform-independent way to build paths.

**Decomposition**:

- `root_name()`: Returns the root name of the path (e.g., "C:" on Windows).
- `root_directory()`: Returns the root directory of the path (e.g., "/").
- `root_path()`: Returns the full root path (e.g., "C:/").
- `relative_path()`: Returns the path relative to the root.
- `parent_path()`: Returns the parent directory of the path.
- `filename()`: Returns the filename component.
- `stem()`: Returns the filename without the extension.
- `extension()`: Returns the file extension.

Example:

```c++
#include <iostream>
#include <filesystem>

int main() {
    std::filesystem::path p("/foo/bar.txt");
    std::cout << "path: " << p << std::endl;
    std::cout << "parent_path: " << p.parent_path() << std::endl;
    std::cout << "filename: " << p.filename() << std::endl;
    std::cout << "stem: " << p.stem() << std::endl;
    std::cout << "extension: " << p.extension() << std::endl;
    return 0;
}
```

### File Attributes

- `file_size(p)`: Returns the size of a file in bytes.
- `is_regular_file(p)`, `is_directory(p)`, `is_symlink(p)`, etc.: Check the type of the filesystem entry.
- `permissions(p, perms, options)`: Manages file permissions.
- `last_write_time(p, new_time)`: Gets or sets the last modification time.
- `status(p)`: Returns the status of a path, which includes its type and permissions.

### Filesystem Queries

- `current_path()`: Gets or sets the current working directory.
- `absolute(p)`: Returns the absolute path for a given path `p`.
- `relative(p, base)`: Returns the path of `p` relative to a base path.
- `space(p)`: Queries free and available space on the filesystem where `p` resides.

### File and Directory Operations

These are the functions you'll use to interact with the filesystem.

- `exists(p)`: Checks if a file or directory exists at path `p`.
- `create_directory(p)`: Creates a new directory. Fails if the parent directory doesn't exist.
- `create_directories(p)`: Creates a directory and any necessary parent directories.
- `remove(p)`: Removes a file or an empty directory.
- `remove_all(p)`: Removes a file, or a directory and all its contents recursively. Use with caution!
- `rename(from, to)`: Renames or moves a file or directory.
- `copy(from, to, options)`: Copies files or directories.
- `copy_file(from, to, options)`: Copies a single file.
- `resize_file(p, new_size)`: Changes the size of a file.

### Copy Options

The `copy` functions can take `copy_options` to control their behavior:

- `copy_options::recursive`: Copies subdirectories and their contents.
- `copy_options::overwrite_existing`: Replaces the destination file if it already exists.
- `copy_options::skip_existing`: Skips the copy if the destination file already exists.

### Iterating Directories

You can easily iterate through the contents of a directory.

`directory_iterator(p)`: Iterates through the immediate contents of a directory.
`recursive_directory_iterator(p)`: Iterates through a directory and all its subdirectories.

## Basic Examples

### Checking if a File or Directory Exists

```c++
#include <iostream>
#include <filesystem>

int main() {
    std::filesystem::path p("some_file.txt");
    if (std::filesystem::exists(p)) {
        std::cout << p << " exists.\n";
    } else {
        std::cout << p << " does not exist.\n";
    }
    return 0;
}
```

### Creating a Directory

```c++
#include <iostream>
#include <filesystem>

int main() {
    std::filesystem::path dir("new_directory");
    if (std::filesystem::create_directory(dir)) {
        std::cout << "Directory created successfully.\n";
    } else {
        std::cout << "Failed to create directory.\n";
    }
    return 0;
}
```

### Listing All Files in a Directory

```c++
#include <iostream>
#include <filesystem>

int main() {
    std::filesystem::path dir(".");
    for (const auto &entry : std::filesystem::directory_iterator(dir)) {
        std::cout << entry.path() << std::endl;
    }
    return 0;
}
```

### Deleting a File

```c++
#include <iostream>
#include <filesystem>

int main() {
    std::filesystem::path file_to_remove("some_file.txt");
    if (std::filesystem::remove(file_to_remove)) {
        std::cout << "File removed successfully.\n";
    } else {
        std::cout << "Failed to remove file.\n";
    }
    return 0;
}
```

### File Size and File Type

```c++
#include <iostream>
#include <filesystem>

int main() {
    std::filesystem::path p("some_file.txt");
    if (std::filesystem::exists(p)) {
        auto fsize = std::filesystem::file_size(p);
        std::cout << "File size: " << fsize << " bytes.\n";
        if (std::filesystem::is_regular_file(p)) {
            std::cout << p << " is a regular file.\n";
        }
    }
    return 0;
}
```

## Error Handling

Filesystem operations can fail for many reasons: permissions, non-existent files, full disks, etc. The `<filesystem> `library provides two ways to handle these errors:

- `std::error_code`: Most functions in `<filesystem>` have an overload that takes a `std::error_code` object as a parameter. If an error occurs, the `error_code` object is set, and the function does not throw an exception. This is the preferred method for handling expected errors.

- `std::filesystem::filesystem_error`: If you use an overload that does not take an `error_code` and an error occurs, a `filesystem_error` exception is thrown. This exception contains information about the error, including the function that failed, the paths involved, and an `error_code`.

```c++
#include <iostream>
#include <filesystem>

int main() {
    // Using error_code
    std::error_code ec;
    std::filesystem::remove("non_existent_file.txt", ec);
    if (ec) {
        std::cerr << "Error removing file: " << ec.message() << std::endl;
    }

    // Using exceptions
    try {
        std::filesystem::remove("another_non_existent_file.txt");
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "  path1: " << e.path1() << std::endl;
        std::cerr << "  path2: " << e.path2() << std::endl;
        std::cerr << "  code: " << e.code().message() << std::endl;
    }

    return 0;
}
```

## Symbolic and Hard Links

Links are pointers to files or directories. They allow a single file to be referenced by multiple names or in multiple locations. There are two main types of links: hard links and symbolic (or soft) links.

Creating Links: 

- `create_hard_link(target, link)`: Creates a hard link at link that points to target.
- `create_symlink(target, link)`: Creates a symbolic link at link that points to target.
- `create_directory_symlink(target, link)`: A portable way to create a symlink to a directory.

```c++
#include <iostream>
#include <filesystem>
#include <fstream>

int main() {
    // Create a target file
    std::ofstream("target.txt") << "This is the target file.";

    // Create a hard link and a symlink
    std::filesystem::create_hard_link("target.txt", "hard_link.txt");
    std::filesystem::create_symlink("target.txt", "symlink.txt");

    // Check if they are links
    std::cout << "is_symlink(symlink.txt): " << std::filesystem::is_symlink("symlink.txt") << std::endl;
    std::cout << "is_regular_file(hard_link.txt): " << std::filesystem::is_regular_file("hard_link.txt") << std::endl;

    return 0;
}
```

## Managing Permissions

The `<filesystem>` library provides a way to manage file permissions in a portable way. The permissions are modeled after POSIX permissions, which have three sets of permissions: for the owner, the group, and others. Each set has read, write, and execute permissions.

- `std::filesystem::perms`: An enum that represents the permission bits (e.g., `perms::owner_read`, `perms::group_write`).
- `permissions(path, prms, options)`: A function to set or get the permissions of a file.
- `std::filesystem::perm_options`: An enum to control how permissions are set (`perm_options::add`, `perm_options::remove`, `perm_options::replace`).

```c++
#include <iostream>
#include <filesystem>
#include <fstream>

int main() {
    std::ofstream("myfile.txt");

    // Set permissions: owner can read/write, group can read, others can read.
    std::filesystem::permissions("myfile.txt",
        std::filesystem::perms::owner_read | std::filesystem::perms::owner_write |
        std::filesystem::perms::group_read | std::filesystem::perms::others_read,
        std::filesystem::perm_options::replace);

    // Add execute permission for the owner
    std::filesystem::permissions("myfile.txt",
        std::filesystem::perms::owner_exec,
        std::filesystem::perm_options::add);

    return 0;
}
```