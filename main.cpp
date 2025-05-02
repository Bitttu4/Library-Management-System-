// Include necessary header files for input/output operations
#include <iostream>  // For console input/output operations (cin, cout)
#include <fstream>   // For file input/output operations (ifstream, ofstream)
#include <cstring>   // For string manipulation functions like strcpy, strcmp
#include <cctype>    // For character handling functions like tolower

// Define constants for string length limits
const int MAX_STRING_LENGTH = 100;  // Maximum length for strings (title, author, category)
const int MAX_BOOKS = 10000;        // Maximum number of books the library can hold

// Book structure to represent individual book records with essential attributes
struct Book {
    int id;                           // Unique identifier for each book
    char title[MAX_STRING_LENGTH];    // Title of the book
    char author[MAX_STRING_LENGTH];   // Author name
    char category[MAX_STRING_LENGTH]; // Category/genre of the book
    bool available;                   // Availability status (true = available, false = checked out)
    
    // Method to display all information about a book in a formatted way
    void display() {
        std::cout << "-------------------------\n";    // Separator line for readability
        std::cout << "Book ID: " << id << "\n";        // Display book ID
        std::cout << "Title: " << title << "\n";       // Display book title
        std::cout << "Author: " << author << "\n";     // Display book author
        std::cout << "Category: " << category << "\n"; // Display book category
        // Display availability status as text (Available or Checked Out)
        std::cout << "Status: " << (available ? "Available" : "Checked Out") << "\n";
        std::cout << "-------------------------\n";    // Bottom separator line
    }
};

// Library class to manage the collection of books and provide operations
class Library {
private:
    // Private member variables
    Book books[MAX_BOOKS];      // Array to store all book records
    int bookCount;              // Current number of books in the array
    char filename[MAX_STRING_LENGTH]; // Name of the file where books data is stored
    int nextId;                 // Next available ID for a new book
    
    // Helper function to convert a string to lowercase for case-insensitive comparison
    void toLowerCase(char* str) {
        for (int i = 0; str[i]; i++) {
            str[i] = tolower(str[i]);
        }
    }
    
    // Helper function to check if a substring exists in a string (case-insensitive)
    bool containsSubstring(const char* str, const char* substr) {
        // Create temporary copies for case conversion
        char tempStr[MAX_STRING_LENGTH];
        char tempSubstr[MAX_STRING_LENGTH];
        
        // Copy strings to temporary buffers
        strncpy(tempStr, str, MAX_STRING_LENGTH - 1);
        tempStr[MAX_STRING_LENGTH - 1] = '\0';  // Ensure null termination
        
        strncpy(tempSubstr, substr, MAX_STRING_LENGTH - 1);
        tempSubstr[MAX_STRING_LENGTH - 1] = '\0';  // Ensure null termination
        
        // Convert both to lowercase
        toLowerCase(tempStr);
        toLowerCase(tempSubstr);
        
        // Check if substring exists in string
        return (strstr(tempStr, tempSubstr) != NULL);
    }
    
    // Private method to save all books to a file
    void saveToFile() {
        // Open the file for writing
        std::ofstream file(filename);
        
        // Check if file opened successfully
        if (!file) {
            std::cout << "Error: Could not open file for writing.\n";
            return;  // Exit if file cannot be opened
        }
        
        // Write each book record to the file in CSV format
        for (int i = 0; i < bookCount; i++) {
            // Format: ID,Title,Author,Category,Availability(1/0)
            file << books[i].id << "," << books[i].title << "," << books[i].author << ",";
            file << books[i].category << "," << (books[i].available ? "1" : "0") << "\n";
        }
        
        // Close the file after writing
        file.close();
    }
    
    // Function to split a CSV line into fields
    void parseCsvLine(const char* line, char fields[][MAX_STRING_LENGTH], int& fieldCount) {
        fieldCount = 0;
        int linePos = 0;
        int fieldPos = 0;
        
        // Process each character in the line
        while (line[linePos] != '\0' && fieldCount < 5) {
            if (line[linePos] == ',') {
                // End of field - terminate the current field and move to next
                fields[fieldCount][fieldPos] = '\0';
                fieldCount++;
                fieldPos = 0;
            } else {
                // Add character to current field
                fields[fieldCount][fieldPos] = line[linePos];
                fieldPos++;
                
                // Ensure we don't overflow the field buffer
                if (fieldPos >= MAX_STRING_LENGTH - 1) {
                    fieldPos = MAX_STRING_LENGTH - 1;
                    fields[fieldCount][fieldPos] = '\0';
                }
            }
            linePos++;
        }
        
        // Add the last field if there's any content
        if (fieldPos > 0 || line[linePos-1] == ',') {
            fields[fieldCount][fieldPos] = '\0';
            fieldCount++;
        }
    }
    
    // Private method to load books from a file
    void loadFromFile() {
        // Open the file for reading
        std::ifstream file(filename);
        
        // Handle case when file doesn't exist
        if (!file) {
            std::cout << "No existing book file found. Starting with empty library.\n";
            nextId = 1;     // Start with ID 1 for the first book
            bookCount = 0;  // Initialize book count to 0
            return;
        }
        
        // Reset book count
        bookCount = 0;
        
        // Variables for line parsing
        char line[MAX_STRING_LENGTH * 4];  // Buffer for reading a line (large enough for all fields)
        char fields[5][MAX_STRING_LENGTH]; // Array to hold parsed fields
        int fieldCount;
        
        // Read file line by line
        while (file.getline(line, sizeof(line)) && bookCount < MAX_BOOKS) {
            // Parse the CSV line
            parseCsvLine(line, fields, fieldCount);
            
            // Check if we have all 5 expected fields (ID, Title, Author, Category, Availability)
            if (fieldCount == 5) {
                // Parse and assign values to book fields
                books[bookCount].id = atoi(fields[0]);  // Convert ID from string to integer
                
                // Copy strings to book fields (ensuring they're null-terminated)
                strncpy(books[bookCount].title, fields[1], MAX_STRING_LENGTH - 1);
                books[bookCount].title[MAX_STRING_LENGTH - 1] = '\0';
                
                strncpy(books[bookCount].author, fields[2], MAX_STRING_LENGTH - 1);
                books[bookCount].author[MAX_STRING_LENGTH - 1] = '\0';
                
                strncpy(books[bookCount].category, fields[3], MAX_STRING_LENGTH - 1);
                books[bookCount].category[MAX_STRING_LENGTH - 1] = '\0';
                
                // Convert "1" to true, anything else to false
                books[bookCount].available = (strcmp(fields[4], "1") == 0);
                
                // Update nextId to be higher than any existing ID
                if (books[bookCount].id >= nextId) {
                    nextId = books[bookCount].id + 1;
                }
                
                // Increment book count
                bookCount++;
            }
        }
        
        // Close the file after reading
        file.close();
    }
    
public:
    // Constructor - initialize the library
    Library(const char* file = "books.dat") {
        // Set the filename for book data (ensure it's null-terminated)
        strncpy(filename, file, MAX_STRING_LENGTH - 1);
        filename[MAX_STRING_LENGTH - 1] = '\0';
        
        nextId = 1;       // Initialize nextId to 1
        bookCount = 0;    // Initialize book count to 0
        loadFromFile();   // Load books from file if available
    }
    
    // Method to add a new book to the library
    void addBook() {
        // Check if library is full
        if (bookCount >= MAX_BOOKS) {
            std::cout << "Error: Library is full. Cannot add more books.\n";
            return;
        }
        
        // Create a new Book object
        Book newBook;
        
        // Display section header
        std::cout << "\n--- Add New Book ---\n";
        
        // Assign the next available ID to the new book
        newBook.id = nextId++;
        
        // Temporary buffer for input
        char buffer[MAX_STRING_LENGTH];
        
        // Get book details from user
        std::cout << "Enter Title: ";
        std::cin.ignore();  // Clear input buffer to prevent skipping input
        std::cin.getline(buffer, MAX_STRING_LENGTH);
        strncpy(newBook.title, buffer, MAX_STRING_LENGTH - 1);
        newBook.title[MAX_STRING_LENGTH - 1] = '\0';  // Ensure null termination
        
        std::cout << "Enter Author: ";
        std::cin.getline(buffer, MAX_STRING_LENGTH);
        strncpy(newBook.author, buffer, MAX_STRING_LENGTH - 1);
        newBook.author[MAX_STRING_LENGTH - 1] = '\0';  // Ensure null termination
        
        std::cout << "Enter Category: ";
        std::cin.getline(buffer, MAX_STRING_LENGTH);
        strncpy(newBook.category, buffer, MAX_STRING_LENGTH - 1);
        newBook.category[MAX_STRING_LENGTH - 1] = '\0';  // Ensure null termination
        
        // New books are initially available
        newBook.available = true;
        
        // Add the new book to the array
        books[bookCount++] = newBook;
        
        // Save the updated collection to file
        saveToFile();
        
        // Confirm successful addition
        std::cout << "Book added successfully with ID: " << newBook.id << "\n";
    }
    
    // Method to get and display a book's details by ID
    void getBook() {
        int id;       // Variable to store the ID entered by the user
        bool found = false;  // Flag to track if the book was found
        
        // Display section header and prompt for ID
        std::cout << "\n--- Get Book Details ---\n";
        std::cout << "Enter Book ID: ";
        std::cin >> id;
        
        // Loop through all books to find the one with matching ID
        for (int i = 0; i < bookCount; i++) {
            if (books[i].id == id) {
                books[i].display();  // Display the book's details
                found = true;        // Set flag to indicate book was found
                break;               // Exit loop after finding the book
            }
        }
        
        // If book wasn't found, inform the user
        if (!found) {
            std::cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Method to print all books in the library
    void printBooks() {
        // Display section header
        std::cout << "\n--- All Library Books ---\n";
        
        // Check if library is empty
        if (bookCount == 0) {
            std::cout << "No books in the library.\n";
            return;  // Exit the function if no books
        }
        
        // Display total number of books
        std::cout << "Total Books: " << bookCount << "\n\n";
        
        // Loop through and display each book
        for (int i = 0; i < bookCount; i++) {
            books[i].display();  // Display book details
            std::cout << "\n";        // Add extra newline for spacing
        }
    }
    
    // Method to find books by title, author, or category
    void findBook() {
        // Check if library is empty
        if (bookCount == 0) {
            std::cout << "No books in the library.\n";
            return;  // Exit if no books
        }
        
        int choice;                      // Variable for search type choice
        char searchTerm[MAX_STRING_LENGTH]; // Variable for search term
        bool found = false;              // Flag to track if any books match
        
        // Display search options menu
        std::cout << "\n--- Find Book ---\n";
        std::cout << "Search by:\n";
        std::cout << "1. Title\n";
        std::cout << "2. Author\n";
        std::cout << "3. Category\n";
        std::cout << "Enter your choice (1-3): ";
        std::cin >> choice;
        
        // Clear input buffer for getline
        std::cin.ignore();
        
        // Validate user choice
        if (choice < 1 || choice > 3) {
            std::cout << "Invalid choice. Returning to main menu.\n";
            return;  // Exit if invalid choice
        }
        
        // Get search term from user
        std::cout << "Enter search term: ";
        std::cin.getline(searchTerm, MAX_STRING_LENGTH);
        
        // Display results header
        std::cout << "\n--- Search Results ---\n";
        
        // Loop through all books to find matches
        for (int i = 0; i < bookCount; i++) {
            const char* valueToSearch = NULL;  // Pointer to the field to search
            
            // Get the appropriate field based on user's choice
            if (choice == 1) {
                valueToSearch = books[i].title;  // Search by title
            } else if (choice == 2) {
                valueToSearch = books[i].author;  // Search by author
            } else {
                valueToSearch = books[i].category;  // Search by category
            }
            
            // Check if the search term appears in the selected field (case-insensitive)
            if (containsSubstring(valueToSearch, searchTerm)) {
                books[i].display();  // Display matching book
                std::cout << "\n";        // Add newline for spacing
                found = true;        // Set flag that a match was found
            }
        }
        
        // If no matching books were found, inform user
        if (!found) {
            std::cout << "No matching books found.\n";
        }
    }
    
    // Method to update an existing book's information
    void updateBook() {
        int id, field;     // Variables for book ID and field to update
        bool found = false; // Flag to track if book was found
        
        // Display section header and prompt for ID
        std::cout << "\n--- Update Book ---\n";
        std::cout << "Enter Book ID to update: ";
        std::cin >> id;
        
        // Loop through books to find the one with matching ID
        for (int i = 0; i < bookCount; i++) {
            if (books[i].id == id) {
                found = true;  // Set flag that book was found
                
                // Display current book details
                books[i].display();
                
                // Display update options menu
                std::cout << "\nWhat would you like to update?\n";
                std::cout << "1. Title\n";
                std::cout << "2. Author\n";
                std::cout << "3. Category\n";
                std::cout << "4. Availability\n";
                std::cout << "5. Cancel\n";
                std::cout << "Enter your choice (1-5): ";
                std::cin >> field;
                
                // Clear input buffer for potential getline calls
                std::cin.ignore();
                
                // Temporary buffer for input
                char buffer[MAX_STRING_LENGTH];
                
                // Track if any update was made
                bool updated = false;
                
                // Process update based on user's choice
                switch (field) {
                    case 1:  // Update title
                        std::cout << "Enter new title: ";
                        std::cin.getline(buffer, MAX_STRING_LENGTH);
                        strncpy(books[i].title, buffer, MAX_STRING_LENGTH - 1);
                        books[i].title[MAX_STRING_LENGTH - 1] = '\0';  // Ensure null termination
                        updated = true;
                        break;
                    case 2:  // Update author
                        std::cout << "Enter new author: ";
                        std::cin.getline(buffer, MAX_STRING_LENGTH);
                        strncpy(books[i].author, buffer, MAX_STRING_LENGTH - 1);
                        books[i].author[MAX_STRING_LENGTH - 1] = '\0';  // Ensure null termination
                        updated = true;
                        break;
                    case 3:  // Update category
                        std::cout << "Enter new category: ";
                        std::cin.getline(buffer, MAX_STRING_LENGTH);
                        strncpy(books[i].category, buffer, MAX_STRING_LENGTH - 1);
                        books[i].category[MAX_STRING_LENGTH - 1] = '\0';  // Ensure null termination
                        updated = true;
                        break;
                    case 4:  // Toggle availability status
                        books[i].available = !books[i].available;
                        std::cout << "Book status changed to: " 
                                  << (books[i].available ? "Available" : "Checked Out") << "\n";
                        updated = true;
                        break;
                    case 5:  // Cancel update
                        std::cout << "Update canceled.\n";
                        break;
                    default:  // Invalid choice
                        std::cout << "Invalid choice.\n";
                        break;
                }
                
                // If an update was made, save changes and confirm
                if (updated) {
                    saveToFile();  // Save changes to file
                    std::cout << "Book updated successfully.\n";
                    books[i].display();  // Display updated book details
                }
                
                break;  // Exit loop after processing the book
            }
        }
        
        // If book wasn't found, inform user
        if (!found) {
            std::cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Method to remove a book from the library
    void removeBook() {
        int id;           // Variable for book ID
        bool found = false; // Flag to track if book was found
        
        // Display section header and prompt for ID
        std::cout << "\n--- Remove Book ---\n";
        std::cout << "Enter Book ID to remove: ";
        std::cin >> id;
        
        // Loop through books to find the one with matching ID
        for (int i = 0; i < bookCount; i++) {
            if (books[i].id == id) {
                found = true;  // Set flag that book was found
                
                // Display book details before removal
                books[i].display();
                
                // Confirm with user before removing
                char confirm;
                std::cout << "Are you sure you want to remove this book? (y/n): ";
                std::cin >> confirm;
                
                // Process based on user confirmation
                if (confirm == 'y' || confirm == 'Y') {
                    // Remove book by shifting all subsequent books one position back
                    for (int j = i; j < bookCount - 1; j++) {
                        books[j] = books[j + 1];
                    }
                    bookCount--;  // Decrement the book count
                    
                    saveToFile();  // Save changes to file
                    std::cout << "Book removed successfully.\n";
                } else {
                    std::cout << "Removal canceled.\n";
                }
                
                break;  // Exit loop after processing the book
            }
        }
        
        // If book wasn't found, inform user
        if (!found) {
            std::cout << "Book with ID " << id << " not found.\n";
        }
    }
    
    // Method to display the main menu and handle user interaction
    void showMenu() {
        int choice;        // Variable for user's menu choice
        bool running = true; // Flag to control menu loop
        
        // Loop until user chooses to exit
        while (running) {
            // Display main menu
            std::cout << "\n==== LIBRARY MANAGEMENT SYSTEM ====\n";
            std::cout << "1. Add New Book\n";
            std::cout << "2. Get Book Details\n";
            std::cout << "3. Print All Books\n";
            std::cout << "4. Find Book\n";
            std::cout << "5. Update Book\n";
            std::cout << "6. Remove Book\n";
            std::cout << "7. Exit\n";
            std::cout << "Enter your choice (1-7): ";
            
            // Handle invalid input (non-numeric)
            if (!(std::cin >> choice)) {
                std::cin.clear();  // Clear error flags
                std::cin.ignore(100, '\n');  // Discard invalid input (up to 100 chars)
                std::cout << "Invalid input. Please enter a number.\n";
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
                    std::cout << "Thank you for using the Library Management System. Goodbye!\n";
                    running = false;  // Set flag to exit the loop
                    break;
                default:  // Invalid choice (number out of range)
                    std::cout << "Invalid choice. Please try again.\n";
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