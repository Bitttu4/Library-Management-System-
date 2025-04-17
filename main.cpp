#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Simple Book structure
struct Book {
    int id;
    std::string title;
    std::string author;
    std::string category;
    bool available;
    
    // Display book information
    void display() {
        std::cout << "-------------------------\n";
        std::cout << "Book ID: " << id << "\n";
        std::cout << "Title: " << title << "\n";
        std::cout << "Author: " << author << "\n";
        std::cout << "Category: " << category << "\n";
        std::cout << "Status: " << (available ? "Available" : "Checked Out") << "\n";
        std::cout << "-------------------------\n";
    }
};

// Library class to manage books
class Library {
private:
    std::vector<Book> books;
    std::string filename;
    int nextId;
    
    // Save books to file
    void saveToFile() {
        std::ofstream file(filename);
        
        if (!file) {
            std::cout << "Error: Could not open file for writing.\n";
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
        std::ifstream file(filename);
        
        if (!file) {
            std::cout << "No existing book file found. Starting with empty library.\n";
            nextId = 1;
            return;
        }
        
        books.clear();
        std::string line, part;
        std::vector<std::string> parts;
        
        while (getline(file, line)) {
            parts.clear();
            
            // Split the line by commas
            size_t pos = 0;
            while ((pos = line.find(",")) != std::string::npos) {
                part = line.substr(0, pos);
                parts.push_back(part);
                line.erase(0, pos + 1);
            }
            parts.push_back(line); // Add the last part
            
            if (parts.size() == 5) {
                Book book;
                book.id = std::stoi(parts[0]);
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
    Library(std::string file = "books.dat") {
        filename = file;
        nextId = 1;
        loadFromFile();
    }
    
    // Add a new book
    void addBook() {
        Book newBook;
        std::string temp;
        
        std::cout << "\n--- Add New Book ---\n";
        
        newBook.id = nextId++;
        
        std::cout << "Enter Title: ";
        std::cin.ignore(); // Clear input buffer
        getline(std::cin, newBook.title);
        
        std::cout << "Enter Author: ";
        getline(std::cin, newBook.author);
        
        std::cout << "Enter Category: ";
        getline(std::cin, newBook.category);
        
        newBook.available = true;
        
        books.push_back(newBook);
        saveToFile();
        
        std::cout << "Book added successfully with ID: " << newBook.id << "\n";
    }
    
    // Get book details by ID
    void getBook() {
        int id;
        bool found = false;
        
        std::cout << "\n--- Get Book Details ---\n";
        std::cout << "Enter Book ID: ";
        std::cin >> id;
        
        for (Book book : books) {
            if (book.id == id) {
                book.display();
                found = true;
                break;
            }
        }
        
        if (!found) {
            std::cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Print all books
    void printBooks() {
        std::cout << "\n--- All Library Books ---\n";
        
        if (books.empty()) {
            std::cout << "No books in the library.\n";
            return;
        }
        
        std::cout << "Total Books: " << books.size() << "\n\n";
        
        for (Book book : books) {
            book.display();
            std::cout << "\n";
        }
    }
    
    // Find books by title, author, or category
    void findBook() {
        if (books.empty()) {
            std::cout << "No books in the library.\n";
            return;
        }
        
        int choice;
        std::string searchTerm;
        bool found = false;
        
        std::cout << "\n--- Find Book ---\n";
        std::cout << "Search by:\n";
        std::cout << "1. Title\n";
        std::cout << "2. Author\n";
        std::cout << "3. Category\n";
        std::cout << "Enter your choice (1-3): ";
        std::cin >> choice;
        
        std::cin.ignore(); // Clear input buffer
        
        if (choice < 1 || choice > 3) {
            std::cout << "Invalid choice. Returning to main menu.\n";
            return;
        }
        
        std::cout << "Enter search term: ";
        getline(std::cin, searchTerm);
        
        // Convert search term to lowercase
        for (char &c : searchTerm) {
            c = tolower(c);
        }
        
        std::cout << "\n--- Search Results ---\n";
        
        for (Book book : books) {
            std::string value;
            
            // Get the appropriate field to search
            if (choice == 1) {
                value = book.title;
            } else if (choice == 2) {
                value = book.author;
            } else {
                value = book.category;
            }
            
            // Convert to lowercase for case-insensitive search
            std::string valueLower = value;
            for (char &c : valueLower) {
                c = tolower(c);
            }
            
            // Check if the search term is in the value
            if (valueLower.find(searchTerm) != std::string::npos) {
                book.display();
                std::cout << "\n";
                found = true;
            }
        }
        
        if (!found) {
            std::cout << "No matching books found.\n";
        }
    }
    
    // Update book information
    void updateBook() {
        int id, field;
        bool found = false;
        
        std::cout << "\n--- Update Book ---\n";
        std::cout << "Enter Book ID to update: ";
        std::cin >> id;
        
        for (int i = 0; i < books.size(); i++) {
            if (books[i].id == id) {
                found = true;
                
                books[i].display();
                
                std::cout << "\nWhat would you like to update?\n";
                std::cout << "1. Title\n";
                std::cout << "2. Author\n";
                std::cout << "3. Category\n";
                std::cout << "4. Availability\n";
                std::cout << "5. Cancel\n";
                std::cout << "Enter your choice (1-5): ";
                std::cin >> field;
                
                std::cin.ignore(); // Clear input buffer
                
                std::string newValue;
                bool updated = false;
                
                switch (field) {
                    case 1:
                        std::cout << "Enter new title: ";
                        getline(std::cin, books[i].title);
                        updated = true;
                        break;
                    case 2:
                        std::cout << "Enter new author: ";
                        getline(std::cin, books[i].author);
                        updated = true;
                        break;
                    case 3:
                        std::cout << "Enter new category: ";
                        getline(std::cin, books[i].category);
                        updated = true;
                        break;
                    case 4:
                        books[i].available = !books[i].available;
                        std::cout << "Book status changed to: " 
                                  << (books[i].available ? "Available" : "Checked Out") << "\n";
                        updated = true;
                        break;
                    case 5:
                        std::cout << "Update canceled.\n";
                        break;
                    default:
                        std::cout << "Invalid choice.\n";
                        break;
                }
                
                if (updated) {
                    saveToFile();
                    std::cout << "Book updated successfully.\n";
                    books[i].display();
                }
                
                break;
            }
        }
        
        if (!found) {
            std::cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Remove a book
    void removeBook() {
        int id;
        bool found = false;
        
        std::cout << "\n--- Remove Book ---\n";
        std::cout << "Enter Book ID to remove: ";
        std::cin >> id;
        
        for (int i = 0; i < books.size(); i++) {
            if (books[i].id == id) {
                found = true;
                
                books[i].display();
                
                char confirm;
                std::cout << "Are you sure you want to remove this book? (y/n): ";
                std::cin >> confirm;
                
                if (confirm == 'y' || confirm == 'Y') {
                    books.erase(books.begin() + i);
                    saveToFile();
                    std::cout << "Book removed successfully.\n";
                } else {
                    std::cout << "Removal canceled.\n";
                }
                
                break;
            }
        }
        
        if (!found) {
            std::cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Display menu and process user selection
    void showMenu() {
        int choice;
        bool running = true;
        
        while (running) {
            std::cout << "\n==== LIBRARY MANAGEMENT SYSTEM ====\n";
            std::cout << "1. Add New Book\n";
            std::cout << "2. Get Book Details\n";
            std::cout << "3. Print All Books\n";
            std::cout << "4. Find Book\n";
            std::cout << "5. Update Book\n";
            std::cout << "6. Remove Book\n";
            std::cout << "7. Exit\n";
            std::cout << "Enter your choice (1-7): ";
            
            // Handle invalid input
            if (!(std::cin >> choice)) {
                std::cin.clear(); // Clear error flags
                std::cin.ignore(100, '\n'); // Discard invalid input
                std::cout << "Invalid input. Please enter a number.\n";
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
                    std::cout << "Thank you for using the Library Management System. Goodbye!\n";
                    running = false;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
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
