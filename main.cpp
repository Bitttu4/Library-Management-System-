#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Simple Book structure
struct Book {
    int id;
    string title;
    string author;
    string category;
    bool available;
    
    // Display book information
    void display() {
        cout << "-------------------------\n";
        cout << "Book ID: " << id << "\n";
        cout << "Title: " << title << "\n";
        cout << "Author: " << author << "\n";
        cout << "Category: " << category << "\n";
        cout << "Status: " << (available ? "Available" : "Checked Out") << "\n";
        cout << "-------------------------\n";
    }
};

// Library class to manage books
class Library {
private:
    vector<Book> books;
    string filename;
    int nextId;
    
    // Save books to file
    void saveToFile() {
        ofstream file(filename);
        
        if (!file) {
            cout << "Error: Could not open file for writing.\n";
            return;
        }
        
        for (Book book : books) {
            file << book.id << "," << book.title << "," << book.author << ",";
            file << book.category << "," << (book.available ? "1" : "0") << "\n";
        }
        
        file.close();
    }
    
    // Load books from file
    void loadFromFile() {
        ifstream file(filename);
        
        if (!file) {
            cout << "No existing book file found. Starting with empty library.\n";
            nextId = 1;
            return;
        }
        
        books.clear();
        string line, part;
        vector<string> parts;
        
        while (getline(file, line)) {
            parts.clear();
            
            // Split the line by commas
            size_t pos = 0;
            while ((pos = line.find(",")) != string::npos) {
                part = line.substr(0, pos);
                parts.push_back(part);
                line.erase(0, pos + 1);
            }
            parts.push_back(line); // Add the last part
            
            if (parts.size() == 5) {
                Book book;
                book.id = stoi(parts[0]);
                book.title = parts[1];
                book.author = parts[2];
                book.category = parts[3];
                book.available = (parts[4] == "1");
                
                books.push_back(book);
                
                if (book.id >= nextId) {
                    nextId = book.id + 1;
                }
            }
        }
        
        file.close();
    }
    
public:
    // Constructor
    Library(string file = "books.dat") {
        filename = file;
        nextId = 1;
        loadFromFile();
    }
    
    // Add a new book
    void addBook() {
        Book newBook;
        
        cout << "\n--- Add New Book ---\n";
        
        newBook.id = nextId++;
        
        cout << "Enter Title: ";
        cin.ignore(); // Clear input buffer
        getline(cin, newBook.title);
        
        cout << "Enter Author: ";
        getline(cin, newBook.author);
        
        cout << "Enter Category: ";
        getline(cin, newBook.category);
        
        newBook.available = true;
        
        books.push_back(newBook);
        saveToFile();
        
        cout << "Book added successfully with ID: " << newBook.id << "\n";
    }
    
    // Get book details by ID
    void getBook() {
        int id;
        bool found = false;
        
        cout << "\n--- Get Book Details ---\n";
        cout << "Enter Book ID: ";
        cin >> id;
        
        for (Book book : books) {
            if (book.id == id) {
                book.display();
                found = true;
                break;
            }
        }
        
        if (!found) {
            cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Print all books
    void printBooks() {
        cout << "\n--- All Library Books ---\n";
        
        if (books.empty()) {
            cout << "No books in the library.\n";
            return;
        }
        
        cout << "Total Books: " << books.size() << "\n\n";
        
        for (Book book : books) {
            book.display();
            cout << "\n";
        }
    }
    
    // Find books by title, author, or category
    void findBook() {
        if (books.empty()) {
            cout << "No books in the library.\n";
            return;
        }
        
        int choice;
        string searchTerm;
        bool found = false;
        
        cout << "\n--- Find Book ---\n";
        cout << "Search by:\n";
        cout << "1. Title\n";
        cout << "2. Author\n";
        cout << "3. Category\n";
        cout << "Enter your choice (1-3): ";
        cin >> choice;
        
        cin.ignore(); // Clear input buffer
        
        if (choice < 1 || choice > 3) {
            cout << "Invalid choice. Returning to main menu.\n";
            return;
        }
        
        cout << "Enter search term: ";
        getline(cin, searchTerm);
        
        // Convert search term to lowercase
        for (char &c : searchTerm) {
            c = tolower(c);
        }
        
        cout << "\n--- Search Results ---\n";
        
        for (Book book : books) {
            string value;
            
            // Get the appropriate field to search
            if (choice == 1) {
                value = book.title;
            } else if (choice == 2) {
                value = book.author;
            } else {
                value = book.category;
            }
            
            // Convert to lowercase for case-insensitive search
            string valueLower = value;
            for (char &c : valueLower) {
                c = tolower(c);
            }
            
            // Check if the search term is in the value
            if (valueLower.find(searchTerm) != string::npos) {
                book.display();
                cout << "\n";
                found = true;
            }
        }
        
        if (!found) {
            cout << "No matching books found.\n";
        }
    }
    
    // Update book information
    void updateBook() {
        int id, field;
        bool found = false;
        
        cout << "\n--- Update Book ---\n";
        cout << "Enter Book ID to update: ";
        cin >> id;
        
        for (int i = 0; i < books.size(); i++) {
            if (books[i].id == id) {
                found = true;
                
                books[i].display();
                
                cout << "\nWhat would you like to update?\n";
                cout << "1. Title\n";
                cout << "2. Author\n";
                cout << "3. Category\n";
                cout << "4. Availability\n";
                cout << "5. Cancel\n";
                cout << "Enter your choice (1-5): ";
                cin >> field;
                
                cin.ignore(); // Clear input buffer
                
                bool updated = false;
                
                switch (field) {
                    case 1:
                        cout << "Enter new title: ";
                        getline(cin, books[i].title);
                        updated = true;
                        break;
                    case 2:
                        cout << "Enter new author: ";
                        getline(cin, books[i].author);
                        updated = true;
                        break;
                    case 3:
                        cout << "Enter new category: ";
                        getline(cin, books[i].category);
                        updated = true;
                        break;
                    case 4:
                        books[i].available = !books[i].available;
                        cout << "Book status changed to: " 
                                  << (books[i].available ? "Available" : "Checked Out") << "\n";
                        updated = true;
                        break;
                    case 5:
                        cout << "Update canceled.\n";
                        break;
                    default:
                        cout << "Invalid choice.\n";
                        break;
                }
                
                if (updated) {
                    saveToFile();
                    cout << "Book updated successfully.\n";
                    books[i].display();
                }
                
                break;
            }
        }
        
        if (!found) {
            cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Remove a book
    void removeBook() {
        int id;
        bool found = false;
        
        cout << "\n--- Remove Book ---\n";
        cout << "Enter Book ID to remove: ";
        cin >> id;
        
        for (int i = 0; i < books.size(); i++) {
            if (books[i].id == id) {
                found = true;
                
                books[i].display();
                
                char confirm;
                cout << "Are you sure you want to remove this book? (y/n): ";
                cin >> confirm;
                
                if (confirm == 'y' || confirm == 'Y') {
                    books.erase(books.begin() + i);
                    saveToFile();
                    cout << "Book removed successfully.\n";
                } else {
                    cout << "Removal canceled.\n";
                }
                
                break;
            }
        }
        
        if (!found) {
            cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Display menu and process user selection
    void showMenu() {
        int choice;
        bool running = true;
        
        while (running) {
            cout << "\n==== LIBRARY MANAGEMENT SYSTEM ====\n";
            cout << "1. Add New Book\n";
            cout << "2. Get Book Details\n";
            cout << "3. Print All Books\n";
            cout << "4. Find Book\n";
            cout << "5. Update Book\n";
            cout << "6. Remove Book\n";
            cout << "7. Exit\n";
            cout << "Enter your choice (1-7): ";
            
            // Handle invalid input
            if (!(cin >> choice)) {
                cin.clear(); // Clear error flags
                cin.ignore(100, '\n'); // Discard invalid input
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }
            
            switch (choice) {
                case 1:
                    addBook();
                    break;
                case 2:
                    getBook();
                    break;
                case 3:
                    printBooks();
                    break;
                case 4:
                    findBook();
                    break;
                case 5:
                    updateBook();
                    break;
                case 6:
                    removeBook();
                    break;
                case 7:
                    cout << "Thank you for using the Library Management System. Goodbye!\n";
                    running = false;
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                    break;
            }
        }
    }
};

int main() {
    // Create and run the library management system
    Library lms;
    lms.showMenu();
    
    return 0;
}
