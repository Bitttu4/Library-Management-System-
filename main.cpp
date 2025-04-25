// Include necessary header files for input/output operations
#include <iostream>  // For console input/output operations (cin, cout)
#include <fstream>   // For file input/output operations (ifstream, ofstream)
#include <string>    // For string data type and operations
#include <vector>    // For dynamic array (vector) container

// Use the standard namespace to avoid prefixing std:: before every standard library element
using namespace std;

// Book structure to represent individual book records with essential attributes
struct Book {
    int id;          // Unique identifier for each book
    string title;    // Title of the book
    string author;   // Author name
    string category; // Category/genre of the book
    bool available;  // Availability status (true = available, false = checked out)
    
    // Method to display all information about a book in a formatted way
    void display() {
        cout << "-------------------------\n";  // Separator line for readability
        cout << "Book ID: " << id << "\n";      // Display book ID
        cout << "Title: " << title << "\n";     // Display book title
        cout << "Author: " << author << "\n";   // Display book author
        cout << "Category: " << category << "\n"; // Display book category
        // Display availability status as text (Available or Checked Out)
        cout << "Status: " << (available ? "Available" : "Checked Out") << "\n";
        cout << "-------------------------\n";  // Bottom separator line
    }
};

// Library class to manage the collection of books and provide operations
class Library {
private:
    // Private member variables
    vector<Book> books;  // Vector to store all book records
    string filename;     // Name of the file where books data is stored
    int nextId;          // Next available ID for a new book
    
    // Private method to save all books to a file
    void saveToFile() {
        // Open the file for writing
        ofstream file(filename);
        
        // Check if file opened successfully
        if (!file) {
            cout << "Error: Could not open file for writing.\n";
            return;  // Exit if file cannot be opened
        }
        
        // Write each book record to the file in CSV format
        for (Book book : books) {
            // Format: ID,Title,Author,Category,Availability(1/0)
            file << book.id << "," << book.title << "," << book.author << ",";
            file << book.category << "," << (book.available ? "1" : "0") << "\n";
        }
        
        // Close the file after writing
        file.close();
    }
    
    // Private method to load books from a file
    void loadFromFile() {
        // Open the file for reading
        ifstream file(filename);
        
        // Handle case when file doesn't exist
        if (!file) {
            cout << "No existing book file found. Starting with empty library.\n";
            nextId = 1;  // Start with ID 1 for the first book
            return;
        }
        
        // Clear existing books and prepare variables for parsing
        books.clear();
        string line, part;
        vector<string> parts;
        
        // Read file line by line
        while (getline(file, line)) {
            parts.clear();  // Clear parts for each new line
            
            // Split the line by commas to extract individual fields
            size_t pos = 0;
            while ((pos = line.find(",")) != string::npos) {
                part = line.substr(0, pos);  // Extract part before comma
                parts.push_back(part);       // Add to parts vector
                line.erase(0, pos + 1);      // Remove extracted part and comma
            }
            parts.push_back(line);  // Add the last part (after last comma)
            
            // Check if we have all 5 expected fields (ID, Title, Author, Category, Availability)
            if (parts.size() == 5) {
                Book book;  // Create a new book object
                
                // Parse and assign values to book fields
                book.id = stoi(parts[0]);          // Convert ID from string to integer
                book.title = parts[1];             // Assign title
                book.author = parts[2];            // Assign author
                book.category = parts[3];          // Assign category
                book.available = (parts[4] == "1"); // Convert "1" to true, anything else to false
                
                // Add book to the collection
                books.push_back(book);
                
                // Update nextId to be higher than any existing ID
                if (book.id >= nextId) {
                    nextId = book.id + 1;
                }
            }
        }
        
        // Close the file after reading
        file.close();
    }
    
public:
    // Constructor - initialize the library
    Library(string file = "books.dat") {
        filename = file;  // Set the filename for book data
        nextId = 1;       // Initialize nextId to 1
        loadFromFile();   // Load books from file if available
    }
    
    // Method to add a new book to the library
    void addBook() {
        Book newBook;  // Create a new Book object
        
        // Display section header
        cout << "\n--- Add New Book ---\n";
        
        // Assign the next available ID to the new book
        newBook.id = nextId++;
        
        // Get book details from user
        cout << "Enter Title: ";
        cin.ignore();  // Clear input buffer to prevent skipping input
        getline(cin, newBook.title);  // Read full line for title
        
        cout << "Enter Author: ";
        getline(cin, newBook.author);  // Read full line for author
        
        cout << "Enter Category: ";
        getline(cin, newBook.category);  // Read full line for category
        
        // New books are initially available
        newBook.available = true;
        
        // Add the new book to the collection
        books.push_back(newBook);
        
        // Save the updated collection to file
        saveToFile();
        
        // Confirm successful addition
        cout << "Book added successfully with ID: " << newBook.id << "\n";
    }
    
    // Method to get and display a book's details by ID
    void getBook() {
        int id;       // Variable to store the ID entered by the user
        bool found = false;  // Flag to track if the book was found
        
        // Display section header and prompt for ID
        cout << "\n--- Get Book Details ---\n";
        cout << "Enter Book ID: ";
        cin >> id;
        
        // Loop through all books to find the one with matching ID
        for (Book book : books) {
            if (book.id == id) {
                book.display();  // Display the book's details
                found = true;    // Set flag to indicate book was found
                break;           // Exit loop after finding the book
            }
        }
        
        // If book wasn't found, inform the user
        if (!found) {
            cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Method to print all books in the library
    void printBooks() {
        // Display section header
        cout << "\n--- All Library Books ---\n";
        
        // Check if library is empty
        if (books.empty()) {
            cout << "No books in the library.\n";
            return;  // Exit the function if no books
        }
        
        // Display total number of books
        cout << "Total Books: " << books.size() << "\n\n";
        
        // Loop through and display each book
        for (Book book : books) {
            book.display();  // Display book details
            cout << "\n";    // Add extra newline for spacing
        }
    }
    
    // Method to find books by title, author, or category
    void findBook() {
        // Check if library is empty
        if (books.empty()) {
            cout << "No books in the library.\n";
            return;  // Exit if no books
        }
        
        int choice;         // Variable for search type choice
        string searchTerm;  // Variable for search term
        bool found = false; // Flag to track if any books match
        
        // Display search options menu
        cout << "\n--- Find Book ---\n";
        cout << "Search by:\n";
        cout << "1. Title\n";
        cout << "2. Author\n";
        cout << "3. Category\n";
        cout << "Enter your choice (1-3): ";
        cin >> choice;
        
        // Clear input buffer for getline
        cin.ignore();
        
        // Validate user choice
        if (choice < 1 || choice > 3) {
            cout << "Invalid choice. Returning to main menu.\n";
            return;  // Exit if invalid choice
        }
        
        // Get search term from user
        cout << "Enter search term: ";
        getline(cin, searchTerm);
        
        // Convert search term to lowercase for case-insensitive search
        for (char &c : searchTerm) {
            c = tolower(c);  // Convert each character to lowercase
        }
        
        // Display results header
        cout << "\n--- Search Results ---\n";
        
        // Loop through all books to find matches
        for (Book book : books) {
            string value;  // Variable to store the field to search
            
            // Get the appropriate field based on user's choice
            if (choice == 1) {
                value = book.title;  // Search by title
            } else if (choice == 2) {
                value = book.author;  // Search by author
            } else {
                value = book.category;  // Search by category
            }
            
            // Convert field value to lowercase for case-insensitive comparison
            string valueLower = value;
            for (char &c : valueLower) {
                c = tolower(c);  // Convert each character to lowercase
            }
            
            // Check if the search term appears in the selected field
            if (valueLower.find(searchTerm) != string::npos) {
                book.display();  // Display matching book
                cout << "\n";    // Add newline for spacing
                found = true;    // Set flag that a match was found
            }
        }
        
        // If no matching books were found, inform user
        if (!found) {
            cout << "No matching books found.\n";
        }
    }
    
    // Method to update an existing book's information
    void updateBook() {
        int id, field;     // Variables for book ID and field to update
        bool found = false; // Flag to track if book was found
        
        // Display section header and prompt for ID
        cout << "\n--- Update Book ---\n";
        cout << "Enter Book ID to update: ";
        cin >> id;
        
        // Loop through books to find the one with matching ID
        for (int i = 0; i < books.size(); i++) {
            if (books[i].id == id) {
                found = true;  // Set flag that book was found
                
                // Display current book details
                books[i].display();
                
                // Display update options menu
                cout << "\nWhat would you like to update?\n";
                cout << "1. Title\n";
                cout << "2. Author\n";
                cout << "3. Category\n";
                cout << "4. Availability\n";
                cout << "5. Cancel\n";
                cout << "Enter your choice (1-5): ";
                cin >> field;
                
                // Clear input buffer for potential getline calls
                cin.ignore();
                
                // Track if any update was made
                bool updated = false;
                
                // Process update based on user's choice
                switch (field) {
                    case 1:  // Update title
                        cout << "Enter new title: ";
                        getline(cin, books[i].title);
                        updated = true;
                        break;
                    case 2:  // Update author
                        cout << "Enter new author: ";
                        getline(cin, books[i].author);
                        updated = true;
                        break;
                    case 3:  // Update category
                        cout << "Enter new category: ";
                        getline(cin, books[i].category);
                        updated = true;
                        break;
                    case 4:  // Toggle availability status
                        books[i].available = !books[i].available;
                        cout << "Book status changed to: " 
                                  << (books[i].available ? "Available" : "Checked Out") << "\n";
                        updated = true;
                        break;
                    case 5:  // Cancel update
                        cout << "Update canceled.\n";
                        break;
                    default:  // Invalid choice
                        cout << "Invalid choice.\n";
                        break;
                }
                
                // If an update was made, save changes and confirm
                if (updated) {
                    saveToFile();  // Save changes to file
                    cout << "Book updated successfully.\n";
                    books[i].display();  // Display updated book details
                }
                
                break;  // Exit loop after processing the book
            }
        }
        
        // If book wasn't found, inform user
        if (!found) {
            cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Method to remove a book from the library
    void removeBook() {
        int id;           // Variable for book ID
        bool found = false; // Flag to track if book was found
        
        // Display section header and prompt for ID
        cout << "\n--- Remove Book ---\n";
        cout << "Enter Book ID to remove: ";
        cin >> id;
        
        // Loop through books to find the one with matching ID
        for (int i = 0; i < books.size(); i++) {
            if (books[i].id == id) {
                found = true;  // Set flag that book was found
                
                // Display book details before removal
                books[i].display();
                
                // Confirm with user before removing
                char confirm;
                cout << "Are you sure you want to remove this book? (y/n): ";
                cin >> confirm;
                
                // Process based on user confirmation
                if (confirm == 'y' || confirm == 'Y') {
                    books.erase(books.begin() + i);  // Remove book from vector
                    saveToFile();  // Save changes to file
                    cout << "Book removed successfully.\n";
                } else {
                    cout << "Removal canceled.\n";
                }
                
                break;  // Exit loop after processing the book
            }
        }
        
        // If book wasn't found, inform user
        if (!found) {
            cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Method to display the main menu and handle user interaction
    void showMenu() {
        int choice;        // Variable for user's menu choice
        bool running = true; // Flag to control menu loop
        
        // Loop until user chooses to exit
        while (running) {
            // Display main menu
            cout << "\n==== LIBRARY MANAGEMENT SYSTEM ====\n";
            cout << "1. Add New Book\n";
            cout << "2. Get Book Details\n";
            cout << "3. Print All Books\n";
            cout << "4. Find Book\n";
            cout << "5. Update Book\n";
            cout << "6. Remove Book\n";
            cout << "7. Exit\n";
            cout << "Enter your choice (1-7): ";
            
            // Handle invalid input (non-numeric)
            if (!(cin >> choice)) {
                cin.clear();  // Clear error flags
                cin.ignore(100, '\n');  // Discard invalid input (up to 100 chars)
                cout << "Invalid input. Please enter a number.\n";
                continue;  // Go back to the start of the loop
            }
            
            // Process user's choice
            switch (choice) {
                case 1:  // Add new book
                    addBook();
                    break;
                case 2:  // Get book details
                    getBook();
                    break;
                case 3:  // Print all books
                    printBooks();
                    break;
                case 4:  // Find book
                    findBook();
                    break;
                case 5:  // Update book
                    updateBook();
                    break;
                case 6:  // Remove book
                    removeBook();
                    break;
                case 7:  // Exit program
                    cout << "Thank you for using the Library Management System. Goodbye!\n";
                    running = false;  // Set flag to exit the loop
                    break;
                default:  // Invalid choice (number out of range)
                    cout << "Invalid choice. Please try again.\n";
                    break;
            }
        }
    }
};

// Main function - program entry point
int main() {
    // Create a Library object with default filename
    Library lms;
    
    // Start the library management system by displaying the menu
    lms.showMenu();
    
    // Return 0 to indicate successful program execution
    return 0;
}
