#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iomanip>

// Book class to store book information
class Book {
private:
    int id;
    std::string title;
    std::string author;
    std::string category;
    bool isAvailable;

public:
    // Default constructor
    Book() : id(0), title(""), author(""), category(""), isAvailable(true) {}

    // Parameterized constructor
    Book(int _id, const std::string& _title, const std::string& _author, 
         const std::string& _category, bool _isAvailable = true) 
        : id(_id), title(_title), author(_author), category(_category), isAvailable(_isAvailable) {}

    // Getters
    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getCategory() const { return category; }
    bool getAvailability() const { return isAvailable; }

    // Setters
    void setId(int _id) { id = _id; }
    void setTitle(const std::string& _title) { title = _title; }
    void setAuthor(const std::string& _author) { author = _author; }
    void setCategory(const std::string& _category) { category = _category; }
    void setAvailability(bool _isAvailable) { isAvailable = _isAvailable; }

    // Convert Book to string for file storage
    std::string toString() const {
        return std::to_string(id) + "," + title + "," + author + "," + 
               category + "," + (isAvailable ? "1" : "0");
    }

    // Display book information
    void display() const {
        std::cout << "┌───────────────────────────────────────────────────────┐\n";
        std::cout << "│ ID: " << std::setw(52) << std::left << id << "│\n";
        std::cout << "│ Title: " << std::setw(48) << std::left << title << "│\n";
        std::cout << "│ Author: " << std::setw(47) << std::left << author << "│\n";
        std::cout << "│ Category: " << std::setw(45) << std::left << category << "│\n";
        std::cout << "│ Status: " << std::setw(47) << std::left << (isAvailable ? "Available" : "Checked Out") << "│\n";
        std::cout << "└───────────────────────────────────────────────────────┘\n";
    }
};

// Library Management System class
class LibraryManagementSystem {
private:
    std::vector<Book> books;
    std::string filename;
    int nextId;

    // Save all books to file
    bool saveToFile() {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return false;
        }

        for (const auto& book : books) {
            outFile << book.toString() << std::endl;
        }

        outFile.close();
        return true;
    }

    // Load books from file
    bool loadFromFile() {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Warning: Could not open file for reading: " << filename << std::endl;
            std::cerr << "Starting with an empty library. File will be created when adding books." << std::endl;
            nextId = 1;
            return false;
        }

        books.clear();
        nextId = 1;

        std::string line;
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string item;
            std::vector<std::string> tokens;

            while (std::getline(ss, item, ',')) {
                tokens.push_back(item);
            }

            if (tokens.size() == 5) {
                int id = std::stoi(tokens[0]);
                bool availability = (tokens[4] == "1");
                
                Book book(id, tokens[1], tokens[2], tokens[3], availability);
                books.push_back(book);

                if (id >= nextId) {
                    nextId = id + 1;
                }
            }
        }

        inFile.close();
        return true;
    }

public:
    // Constructor
    LibraryManagementSystem(const std::string& _filename = "books.dat") 
        : filename(_filename), nextId(1) {
        loadFromFile();
    }

    // Add a new book to the library
    void addBook() {
        std::string title, author, category;
        
        std::cout << "\n===== Add New Book =====\n";
        
        std::cout << "Enter Title: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, title);
        
        std::cout << "Enter Author: ";
        std::getline(std::cin, author);
        
        std::cout << "Enter Category: ";
        std::getline(std::cin, category);
        
        Book newBook(nextId++, title, author, category);
        books.push_back(newBook);
        
        if (saveToFile()) {
            std::cout << "Book added successfully with ID: " << newBook.getId() << std::endl;
        } else {
            std::cout << "Error saving book to file." << std::endl;
        }
    }

    // Find book by ID and return a reference
    Book* findBookById(int id) {
        for (auto& book : books) {
            if (book.getId() == id) {
                return &book;
            }
        }
        return nullptr;
    }

    // Get and display a book by ID
    void getBook() {
        int id;
        std::cout << "\n===== Get Book Details =====\n";
        std::cout << "Enter Book ID: ";
        std::cin >> id;
        
        Book* book = findBookById(id);
        if (book) {
            book->display();
        } else {
            std::cout << "Book with ID " << id << " not found." << std::endl;
        }
    }

    // Print all books or filtered books
    void printBooks() {
        std::cout << "\n===== Library Books =====\n";
        
        if (books.empty()) {
            std::cout << "No books in the library." << std::endl;
            return;
        }
        
        std::cout << "Total Books: " << books.size() << std::endl << std::endl;
        
        for (const auto& book : books) {
            book.display();
            std::cout << std::endl;
        }
    }

    // Find books by various criteria
    void findBook() {
        if (books.empty()) {
            std::cout << "No books in the library." << std::endl;
            return;
        }

        int choice;
        std::cout << "\n===== Find Book =====\n";
        std::cout << "Search by:\n";
        std::cout << "1. Title\n";
        std::cout << "2. Author\n";
        std::cout << "3. Category\n";
        std::cout << "Enter your choice (1-3): ";
        std::cin >> choice;
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::string searchTerm;
        std::string searchType;
        
        switch (choice) {
            case 1:
                searchType = "title";
                break;
            case 2:
                searchType = "author";
                break;
            case 3:
                searchType = "category";
                break;
            default:
                std::cout << "Invalid choice. Returning to main menu." << std::endl;
                return;
        }
        
        std::cout << "Enter " << searchType << " to search for: ";
        std::getline(std::cin, searchTerm);
        
        // Convert search term to lowercase for case-insensitive comparison
        std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), 
                       [](unsigned char c){ return std::tolower(c); });
        
        bool found = false;
        std::cout << "\n===== Search Results =====\n";
        
        for (const auto& book : books) {
            std::string bookValue;
            
            if (choice == 1) {
                bookValue = book.getTitle();
            } else if (choice == 2) {
                bookValue = book.getAuthor();
            } else {
                bookValue = book.getCategory();
            }
            
            // Convert book value to lowercase for comparison
            std::string bookValueLower = bookValue;
            std::transform(bookValueLower.begin(), bookValueLower.end(), bookValueLower.begin(), 
                           [](unsigned char c){ return std::tolower(c); });
            
            if (bookValueLower.find(searchTerm) != std::string::npos) {
                book.display();
                std::cout << std::endl;
                found = true;
            }
        }
        
        if (!found) {
            std::cout << "No books found matching '" << searchTerm << "' in " << searchType << "." << std::endl;
        }
    }

    // Update book information
    void updateBook() {
        int id;
        std::cout << "\n===== Update Book =====\n";
        std::cout << "Enter Book ID to update: ";
        std::cin >> id;
        
        Book* book = findBookById(id);
        if (!book) {
            std::cout << "Book with ID " << id << " not found." << std::endl;
            return;
        }
        
        book->display();
        
        int choice;
        std::cout << "\nWhat would you like to update?\n";
        std::cout << "1. Title\n";
        std::cout << "2. Author\n";
        std::cout << "3. Category\n";
        std::cout << "4. Availability\n";
        std::cout << "5. Cancel\n";
        std::cout << "Enter your choice (1-5): ";
        std::cin >> choice;
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::string newValue;
        bool updated = false;
        
        switch (choice) {
            case 1:
                std::cout << "Enter new title: ";
                std::getline(std::cin, newValue);
                book->setTitle(newValue);
                updated = true;
                break;
            case 2:
                std::cout << "Enter new author: ";
                std::getline(std::cin, newValue);
                book->setAuthor(newValue);
                updated = true;
                break;
            case 3:
                std::cout << "Enter new category: ";
                std::getline(std::cin, newValue);
                book->setCategory(newValue);
                updated = true;
                break;
            case 4:
                book->setAvailability(!book->getAvailability());
                std::cout << "Book status changed to: " 
                          << (book->getAvailability() ? "Available" : "Checked Out") << std::endl;
                updated = true;
                break;
            case 5:
                std::cout << "Update canceled." << std::endl;
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
                break;
        }
        
        if (updated && saveToFile()) {
            std::cout << "Book updated successfully." << std::endl;
            book->display();
        } else if (updated) {
            std::cout << "Error saving updates to file." << std::endl;
        }
    }

    // Remove a book from the library
    void removeBook() {
        int id;
        std::cout << "\n===== Remove Book =====\n";
        std::cout << "Enter Book ID to remove: ";
        std::cin >> id;
        
        Book* book = findBookById(id);
        if (!book) {
            std::cout << "Book with ID " << id << " not found." << std::endl;
            return;
        }
        
        std::cout << "The following book will be removed:\n";
        book->display();
        
        char confirm;
        std::cout << "Are you sure you want to remove this book? (y/n): ";
        std::cin >> confirm;
        
        if (tolower(confirm) == 'y') {
            auto it = std::remove_if(books.begin(), books.end(), 
                                     [id](const Book& b) { return b.getId() == id; });
            
            if (it != books.end()) {
                books.erase(it, books.end());
                
                if (saveToFile()) {
                    std::cout << "Book removed successfully." << std::endl;
                } else {
                    std::cout << "Error saving changes to file." << std::endl;
                }
            }
        } else {
            std::cout << "Removal canceled." << std::endl;
        }
    }

    // Display menu and handle user interaction
    void run() {
        int choice;
        bool running = true;
        
        while (running) {
            std::cout << "\n";
            std::cout << "╔═══════════════════════════════════════╗\n";
            std::cout << "║      LIBRARY MANAGEMENT SYSTEM        ║\n";
            std::cout << "╠═══════════════════════════════════════╣\n";
            std::cout << "║  1. Add New Book                      ║\n";
            std::cout << "║  2. Get Book Details                  ║\n";
            std::cout << "║  3. Print All Books                   ║\n";
            std::cout << "║  4. Find Book                         ║\n";
            std::cout << "║  5. Update Book                       ║\n";
            std::cout << "║  6. Remove Book                       ║\n";
            std::cout << "║  7. Exit                              ║\n";
            std::cout << "╚═══════════════════════════════════════╝\n";
            std::cout << "Enter your choice (1-7): ";
            
            // Handle invalid input
            if (!(std::cin >> choice)) {
                std::cin.clear(); // Clear error flags
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                std::cout << "Invalid input. Please enter a number." << std::endl;
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
                    std::cout << "Thank you for using the Library Management System. Goodbye!" << std::endl;
                    running = false;
                    break;
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
                    break;
            }
        }
    }
};

int main() {
    // Create and run the library management system
    LibraryManagementSystem lms;
    lms.run();
    
    return 0;
}
