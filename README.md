# Library Management System

A console application in C++ for managing a small library's books and members. Data is saved to text files after every change, so nothing is lost when the program closes.

## Features

- Add books and members (IDs are assigned automatically)
- List all books with their status, and all members with how many books they hold
- Search books by title or author (case-insensitive, partial match)
- Issue and return books with validation:
  - a book cannot be issued twice
  - a member can hold at most 3 books at a time
  - unknown book or member IDs are rejected
- Safe input handling: invalid numbers or empty text are asked again instead of crashing

## Build and run

You need any C++17 compiler.

**Windows (Visual Studio):** open "Developer PowerShell for VS 2022" in this folder, then:

```bash
cl /EHsc /std:c++17 main.cpp /Fe:library.exe
```

```bash
.\library.exe
```

**Linux, macOS, or Windows with MinGW:**

```bash
g++ -std=c++17 -o library main.cpp
```

```bash
./library
```

## Example

```
===== Library Menu =====
1. Add book
2. Show all books
3. Search books
4. Add member
5. Show all members
6. Issue book
7. Return book
0. Exit
Choose: 2
  [1] Clean Code by Robert Martin - available
  [2] The Pragmatic Programmer by Hunt & Thomas - issued to Rahim
  [3] Effective C++ by Scott Meyers - issued to Rahim
```

## How it works

- `Book` and `Member` are simple structs. A book stores the ID of the member who has it, or 0 if it is on the shelf.
- The `Library` class keeps both lists in `std::vector`s and holds all the rules. Its data is private, so the menu can only change it through methods like `issueBook()`.
- The number of books a member holds is counted from the book list rather than stored separately, so it can never get out of sync.
- Data is stored in `books.txt` and `members.txt`, one record per line with fields separated by `|`.

## Concepts used

Classes and encapsulation, structs, `std::vector`, file input and output with `fstream`, string processing, pointers, and input validation with exceptions.
