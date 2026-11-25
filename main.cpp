#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const int MAX_BOOKS_PER_MEMBER = 3;
const string BOOKS_FILE = "books.txt";
const string MEMBERS_FILE = "members.txt";

struct Book {
    int id;
    string title;
    string author;
    int issuedTo;  // member id, or 0 if the book is on the shelf
};

struct Member {
    int id;
    string name;
};


// Reads a whole line and makes sure it is not empty.
string readText(const string& prompt) {
    string text;
    while (true) {
        cout << prompt;
        getline(cin, text);
        if (!cin) exit(0);  // input closed (Ctrl+Z / Ctrl+D)
        replace(text.begin(), text.end(), '|', '/');  // '|' separates fields in the files
        if (!text.empty()) return text;
        cout << "  Please type something.\n";
    }
}

// Keeps asking until the user types a valid whole number.
int readNumber(const string& prompt) {
    while (true) {
        string line = readText(prompt);
        try {
            size_t used = 0;
            int value = stoi(line, &used);
            if (used == line.size()) return value;
        } catch (...) {
        }
        cout << "  Please enter a number.\n";
    }
}

string toLower(string text) {
    for (char& c : text) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return text;
}



class Library {
public:
    // Reads saved data. A damaged line is skipped instead of crashing the program.
    void load() {
        ifstream booksIn(BOOKS_FILE);
        string line;
        while (getline(booksIn, line)) {
            vector<string> f = split(line);
            try {
                if (f.size() == 4) books.push_back({stoi(f[0]), f[1], f[2], stoi(f[3])});
            } catch (...) {
            }
        }
        ifstream membersIn(MEMBERS_FILE);
        while (getline(membersIn, line)) {
            vector<string> f = split(line);
            try {
                if (f.size() == 2) members.push_back({stoi(f[0]), f[1]});
            } catch (...) {
            }
        }
    }

    void save() const {
        ofstream booksOut(BOOKS_FILE);
        for (const Book& b : books)
            booksOut << b.id << '|' << b.title << '|' << b.author << '|' << b.issuedTo << '\n';
        ofstream membersOut(MEMBERS_FILE);
        for (const Member& m : members) membersOut << m.id << '|' << m.name << '\n';
    }

    void addBook() {
        Book b;
        b.id = books.empty() ? 1 : books.back().id + 1;
        b.title = readText("Title: ");
        b.author = readText("Author: ");
        b.issuedTo = 0;
        books.push_back(b);
        save();
        cout << "Book added with ID " << b.id << ".\n";
    }

    void showBooks() const {
        if (books.empty()) {
            cout << "No books yet.\n";
            return;
        }
        for (const Book& b : books) printBook(b);
    }

    void searchBooks() const {
        string query = toLower(readText("Search title or author: "));
        int found = 0;
        for (const Book& b : books) {
            if (toLower(b.title).find(query) != string::npos ||
                toLower(b.author).find(query) != string::npos) {
                printBook(b);
                found++;
            }
        }
        cout << found << " book(s) found.\n";
    }

    void addMember() {
        Member m;
        m.id = members.empty() ? 1 : members.back().id + 1;
        m.name = readText("Member name: ");
        members.push_back(m);
        save();
        cout << "Member added with ID " << m.id << ".\n";
    }

    void showMembers() const {
        if (members.empty()) {
            cout << "No members yet.\n";
            return;
        }
        for (const Member& m : members) {
            cout << "  [" << m.id << "] " << m.name << " - borrowed " << countBorrowed(m.id)
                 << "/" << MAX_BOOKS_PER_MEMBER << '\n';
        }
    }

    void issueBook() {
        Book* book = findBook(readNumber("Book ID: "));
        if (!book) {
            cout << "No book with that ID.\n";
            return;
        }
        if (book->issuedTo != 0) {
            cout << "That book is already issued.\n";
            return;
        }
        Member* member = findMember(readNumber("Member ID: "));
        if (!member) {
            cout << "No member with that ID.\n";
            return;
        }
        if (countBorrowed(member->id) >= MAX_BOOKS_PER_MEMBER) {
            cout << member->name << " already has " << MAX_BOOKS_PER_MEMBER << " books.\n";
            return;
        }
        book->issuedTo = member->id;
        save();
        cout << "\"" << book->title << "\" issued to " << member->name << ".\n";
    }

    void returnBook() {
        Book* book = findBook(readNumber("Book ID: "));
        if (!book) {
            cout << "No book with that ID.\n";
            return;
        }
        if (book->issuedTo == 0) {
            cout << "That book is not issued.\n";
            return;
        }
        book->issuedTo = 0;
        save();
        cout << "\"" << book->title << "\" returned.\n";
    }

private:
    vector<Book> books;
    vector<Member> members;

    static vector<string> split(const string& line) {
        vector<string> fields;
        string field;
        stringstream ss(line);
        while (getline(ss, field, '|')) fields.push_back(field);
        return fields;
    }

    Book* findBook(int id) {
        for (Book& b : books)
            if (b.id == id) return &b;
        return nullptr;
    }

    Member* findMember(int id) {
        for (Member& m : members)
            if (m.id == id) return &m;
        return nullptr;
    }

    string memberName(int id) const {
        for (const Member& m : members)
            if (m.id == id) return m.name;
        return "member " + to_string(id);
    }

    // Counted from the books, so it can never get out of sync.
    int countBorrowed(int memberId) const {
        int count = 0;
        for (const Book& b : books)
            if (b.issuedTo == memberId) count++;
        return count;
    }

    void printBook(const Book& b) const {
        cout << "  [" << b.id << "] " << b.title << " by " << b.author << " - ";
        if (b.issuedTo == 0) {
            cout << "available\n";
        } else {
            cout << "issued to " << memberName(b.issuedTo) << '\n';
        }
    }
};


int main() {
    Library library;
    library.load();

    while (true) {
        cout << "\n Library Menu...................\n"
             << "1. Add book\n"
             << "2. Show all books\n"
             << "3. Search books\n"
             << "4. Add member\n"
             << "5. Show all members\n"
             << "6. Issue book\n"
             << "7. Return book\n"
             << "0. Exit\n";

        switch (readNumber("Choose: ")) {
            case 1: library.addBook(); break;
            case 2: library.showBooks(); break;
            case 3: library.searchBooks(); break;
            case 4: library.addMember(); break;
            case 5: library.showMembers(); break;
            case 6: library.issueBook(); break;
            case 7: library.returnBook(); break;
            case 0: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }
}
