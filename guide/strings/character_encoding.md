# Character Encoding

Character encoding is the bridge between the human concept of text (letters, numbers, symbols) and the binary world of computers. It's a set of rules for mapping characters to a sequence of bytes. Getting this right is crucial for any application that handles text, especially in our globalized world.

## Unicode

Before discussing C++ strings, you must understand the standard they support: Unicode.

**What it is**: Unicode is a universal character set. Think of it as a giant table that assigns a unique number, called a code point, to every character, symbol, and emoji.

- `A` is `U+0041`
- `€` is `U+20AC`
- `👍` is `U+1F44D`

**What it solves**: It eliminates the chaos of older, conflicting character sets (like various "extended ASCII" code pages) where the same number could mean different characters on different systems.

Unicode is the what (the character map). The encodings below are the how (the rules for storing those characters in bytes).

## Encodings: Storing Unicode Characters

### 1. ASCII

At the most basic level, we have ASCII (American Standard Code for Information Interchange).

**What it is**: A 7-bit encoding, meaning it can represent 128 characters. This includes the uppercase and lowercase English alphabet, numbers, and some control characters.

**How it works in C++**: The `char` type was designed with ASCII in mind.

By default, `std::string` is designed to store ASCII characters. ASCII characters are 7-bit characters that cover basic Latin letters, digits, and various control characters. Extended ASCII uses 8 bits and can represent additional characters, but it's not standardized across different systems.

Example:

```c++
std::string s = "Hello";
std::cout << "Length in bytes: " << s.length() << std::endl; // Output: 5
```

- Simple and efficient for English text.
- Extremely limited. It cannot represent characters from other languages, emojis, or many common symbols.

**Extended ASCII** is an 8-bit version, adding another 128 characters. However, there are many different versions of extended ASCII (called code pages), leading to a common problem: text created on one system might not display correctly on another.

### 2. UTF-8

UTF-8 is the dominant encoding for the web, files, and network communication. This should be your default choice.

**How it works**: It's a variable-width encoding.

- ASCII characters take 1 byte.
- Most common European and Middle Eastern characters take 2 bytes.
- Most common Asian characters take 3 bytes.
- Other characters, including most emojis, take 4 bytes.

#### Using UTF-8 in C++

You can and should store UTF-8 text in a standard `std::string`. However, you must be aware of the "byte-oriented" nature of `std::string`.

The Golden Rule: Functions like `size()` and `length()` on a `std::string` return the number of bytes, not the number of visual characters.

#### Example: Counting Bytes

```c++
// "Hello, " (7 chars) + "世界" (2 chars) + "!" (1 char) = 10 characters
std::string utf8_str = "Hello, 世界!"; 

// Byte count: 7 (for "Hello, ") + 6 (3+3 for "世界") + 1 (for "!") = 14 bytes
std::cout << "Character count is 10, but byte count is: " 
          << utf8_str.length() << std::endl; // Output: 14
```

- **Universal**: Can represent any character.
- **Backward Compatible with ASCII**: Any valid ASCII text is also valid UTF-8.

#### Example: The Indexing Trap

Accessing a `std::string` by index `[i]` gives you a single byte, which is often just a fragment of a multi-byte character.

```c++
std::string utf8_str = "你好"; // "Hello" in Chinese

// This is incorrect! It will only print the first byte of the first character.
// It will likely result in a garbled or incomplete character being displayed.
std::cout << utf8_str[0] << std::endl;

// This is also incorrect! It will create a substring of the first 3 bytes,
// which happens to be the first character. But this is not a reliable way
// to get a single character.
std::cout << utf8_str.substr(0, 3) << std::endl; // Might print "你"
```

#### Pro's and Con's of UTF-8

- **Universal**: Represents every Unicode character.
- **Efficient**: No memory is wasted for common English text.
- **Backwards-compatible with ASCII**: A huge advantage for legacy systems.
- **Manipulation requires care**: Naively indexing or splitting a std::string will break multi-byte characters.

### UTF-16 and UTF-32: The Wide Encodings

These use larger, fixed-size units to store characters.

- `UTF-32 (char32_t)`: The simplest encoding. Every character is a single 32-bit unit that directly corresponds to its Unicode code point.

- `UTF-16 (char16_t)`: A compromise. It uses one 16-bit unit for most common characters but requires two units (a "surrogate pair") for less common ones, like many emojis. It is also variable-width, just like UTF-8.

## `char8_t` (C++20): Type-Safe UTF-8

The single most important reason for `std::u8string` (`char8_t`) is safety. It helps you use the C++ compiler as a safety net to automatically catch common and nasty text-encoding bugs.

**Purpose**: To create a distinct type for UTF-8 text. This lets you write functions that only accept UTF-8 strings, preventing encoding-related bugs at compile time.

```c++
// The u8 prefix creates a UTF-8 string literal of type const char8_t*.
const char8_t* utf8_literal = u8"Hello, 世界!";

// std::u8string is a string of char8_t.
std::u8string utf8_string = u8"你好";
```

### Example: Username

Now, imagine you write a function to handle usernames, and you need the username to be in the modern UTF-8 format.

```c++
void log_username_unsafe(const std::string& username) {
    write_to_utf8_log_file(username);
}
```

Later, a coworker needs to log a username they got from an old system. Their `std::string` `username` contains text, but it's in an old, non-UTF-8 format.

```c++
// Coworker's code:
std::string legacy_str = get_username_from_old_database(); // Contains "René" in a legacy format.
log_username_unsafe(legacy_str);
```

The code compiles perfectly. There are no errors. But when the program runs, your log file gets corrupted with garbled text like `RenÃ©`. This is a silent runtime bug—the worst kind, because you might not notice it for weeks, and it's hard to trace back to the source. The function's assumption was violated, and nothing warned you.

#### Solution:`std::u8string`

Now think of `std::u8string` as a special, clearly labeled box: "FOR UTF-8 TEXT ONLY".

You rewrite your function to require this specific box. The function's purpose is now unmistakable.

```c++
// This function demands a string that is GUARANTEED to contain UTF-8 text.
void log_username_safe(const std::u8string& utf8_username) {
    write_to_utf8_log_file(utf8_username);
}

// They try to pass their plain string to your function that demands a labeled one.
std::string legacy_box = get_username_from_old_database(); 
log_username_safe(legacy_box); // COMPILE-TIME ERROR!
```

## Non-UTF-8 Characters

It's not about specific characters being "non-UTF-8". Any character you can imagine has a valid UTF-8 representation.

A "non-UTF-8" issue happens when the **sequence of bytes** used to store the text is broken.

Think of it like spelling rules in English. The letters 't', 'c', 'a' are all valid. But the sequence "tca" is not a valid English word.

It is invalid UTF-8 if the rules of the byte sequence is wrong. It refers to a corrupted or incorrectly encoded sequence of bytes that fails to follow the simple rules of the encoding.

## Wide Strings: A Different Approach

For encodings with fixed-size characters, like UTF-16 and UTF-32, C++ provides "wide" string types.

- `std::u16string` (`char16_t`): For UTF-16. This is the encoding used by the Windows API.
- `std::u32string` (`char32_t`): For UTF-32. Each character is a fixed 32-bit value.

```c++
#include <string>

// C++11 and later
std::u16string utf16_str = u"Hello, 世界!"; // u prefix for UTF-16
std::u32string utf32_str = U"Hello, 世界!"; // U prefix for UTF-32
```

`std::u32string`: Very simple to work with. Every character is a single, fixed-size code unit. `my_string.length()` gives you the number of characters, and `my_string[i]` gives you the i-th character.

- **Memory Usage**: u16string and u32string can use significantly more memory than u8string, especially for text that is mostly ASCII.

### The Problem with `std::wstring`

You'll often see `std::wstring` (`wchar_t`) used for wide characters. Avoid `std::wstring` for portable code.

- **On Windows**: `wchar_t` is 16 bits (for UTF-16).
- **On Linux/macOS**: `wchar_t` is 32 bits (for UTF-32).

This difference makes `std::wstring` a portability nightmare. Stick to `std::u16string` or `std::u32string` for explicit control over the encoding.

## Best Practices for Character Encoding in C++

- **Use UTF-8 Everywhere**. Use std::string (or std::u8string in C++20) to store text internally. Treat it as an opaque sequence of bytes and don't make assumptions about its contents.
- **Save Source Files as UTF-8**. Configure your editor (VS Code, Visual Studio, etc.) to save all .cpp and .h files with UTF-8 encoding. This ensures your string literals const char* s = "你好"; are encoded correctly.
- **For cross-platform applications, avoid** `std::wstring`. Use std::string with UTF-8 for your internal logic and convert to/from `std::u16string` only when calling platform APIs that require it (like the Windows API).