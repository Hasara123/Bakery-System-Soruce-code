#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <Windows.h>


using namespace std;



// ANSI escape codes for text color
const string RESET_COLOR = "\033[0m";
const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";
const string YELLOW_COLOR = "\033[33m";
const string BLUE_COLOR = "\033[34m";
const string PURPLE_COLOR ="\x1B[35m";


struct User {
    string username;
    string password;
    string role;
};

struct BakeryItem {
    int itemId;
    string name;
    double price;
    string description;
};

struct OrderItem {
    int itemId;
    int quantity;
};

struct Order {
    vector<OrderItem> items;
    double totalBill;
};


// Array to store user data
User users[100];

// Function to load user data from a file into the array
void loadUsersFromFile(User users[], int& numUsers) {
    ifstream inFile("users.txt");
    numUsers = 0; // Initialize the number of users to 0

    while (inFile >> users[numUsers].username >> users[numUsers].password >> users[numUsers].role) {
        numUsers++; // Increment the number of users read from the file
    }

    inFile.close();
}

// Function to save user data from the array to a file
void saveUsersToFile(const User users[], int numUsers) {
    ofstream outFile("users.txt");

    for (int i = 0; i < numUsers; i++) {
        outFile << users[i].username << " " << users[i].password << " " << users[i].role << endl;
    }

    outFile.close();
}

bool loginUser(User users[], int numUsers, User& loggedInUser) {
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    for (int i = 0; i < numUsers; i++) {
        if (users[i].username == username && users[i].password == password) {
            loggedInUser = users[i]; // Set the logged-in user
            cout << GREEN_COLOR << "Login Successful! Welcome to the " << loggedInUser.username << " Menu" << RESET_COLOR << endl;
            return true;
        }
    }

    cout << RED_COLOR << "Login failed. Invalid credentials." << RESET_COLOR << endl;
    return false;
}

bool signupUser(User users[], int& numUsers) {
    string username, password;
    cout << "Enter Username (admin/cashier): ";
    cin >> username;

    // Check if the entered username is valid
    if (username != "admin" && username != "cashier") {
        cout << RED_COLOR << "Invalid username. Please enter 'admin' or 'cashier'." << RESET_COLOR << endl;
        return false;
    }

    cout << "Enter Password: ";
    cin >> password;

    // Check if a user with the same username and password already exists
    for (int i = 0; i < numUsers; i++) {
        if (users[i].username == username && users[i].password == password) {
            cout << RED_COLOR << "User with the same username and password already exists." << RESET_COLOR << endl;
            return false;
        }
    }

    // Add the new user
    users[numUsers] = { username, password, (username == "admin") ? "admin" : "cashier" };
    numUsers++;
    saveUsersToFile(users, numUsers);

    cout << GREEN_COLOR << "Signup successful! You can now login." << RESET_COLOR << endl;
    return true;
}
void loadBakeryItemFromFile(vector<BakeryItem>& items) {
    ifstream inFile("bakeryItems.txt");
    items.clear();
    BakeryItem item;
    while (inFile >> item.itemId >> item.name >> item.price) {
        items.push_back(item);
    }
    inFile.close();
}


void saveBakeryItemsToFile(const vector<BakeryItem>& items) {
    ofstream outFile("bakeryItems.txt");
    for (const BakeryItem& item : items) {
        outFile << item.itemId << " " << item.name << " " << item.price << endl;
    }
    outFile.close();
}


void addBakeryItem(vector<BakeryItem>& items) {
    BakeryItem newItem;

    cout << "      *===============================================================*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                 ******** WELCOME ********                   |*  " << endl;
    cout << "      *|                             TO                              |*  " << endl;
    cout << "      *|                        Amber Bakers                         |*  " << endl;
    cout << "      *|                ********         ********                    |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                       --Add Items--                         |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *===============================================================*  " << endl;
    cout << endl << endl;    
    cout << "Enter item id: ";
    cin >> newItem.itemId;
    cout << endl;

    // Check for duplicate IDs
    bool idExists = any_of(items.begin(), items.end(), [&newItem](const BakeryItem& item) {
        return item.itemId == newItem.itemId;
    });

    if (idExists) {
        cout << RED_COLOR<<"An item with the same ID already exists. Please choose a different ID.\n"<<RESET_COLOR;
        return;
    }

    cout << "Enter item name: ";
    cin.ignore();
    getline(cin, newItem.name);
    cout << "Enter item price: ";
    cin >> newItem.price;
    items.push_back(newItem);
   saveBakeryItemsToFile(items); // Save the updated list of items to the file
    cout <<GREEN_COLOR<< "Item added successfully.\n"<<RESET_COLOR;
}


// Function to delete a bakery item
void deleteBakeryItem(vector<BakeryItem>& items, const int& itemId) {

    cout << "      *===============================================================*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                 ******** WELCOME ********                   |*  " << endl;
    cout << "      *|                             TO                              |*  " << endl;
    cout << "      *|                        Amber Bakers                         |*  " << endl;
    cout << "      *|                ********         ********                    |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                      --Delete Items--                       |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *===============================================================*  " << endl;
    cout << endl << endl;

    auto itemToRemove = remove_if(items.begin(), items.end(), [&itemId](const BakeryItem& item) {
        return item.itemId == itemId;
    });

    if (itemToRemove != items.end()) {
        items.erase(itemToRemove, items.end());
        cout << "Item with ID " << itemId << GREEN_COLOR<<" deleted successfully.\n"<<RESET_COLOR;
        saveBakeryItemsToFile(items); // Save the updated list of items to the file
    } else {
        cout <<RED_COLOR<< "Item not found.\n"<<RESET_COLOR;
    }
}

// Function to view a bakery item
void viewBakeryItems(const vector<BakeryItem>& items) {

    cout << "      *===============================================================*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                 ******** WELCOME ********                   |*  " << endl;
    cout << "      *|                             TO                              |*  " << endl;
    cout << "      *|                        Amber Bakers                         |*  " << endl;
    cout << "      *|                ********         ********                    |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                      --View Items--                         |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *===============================================================*  " << endl;
    cout << endl << endl;

    if (items.empty()) {
        cout << GREEN_COLOR<<"No bakery items available.\n"<<RESET_COLOR;
    } else {
        cout << "Bakery Items:\n";
        for (const BakeryItem& item : items) {
            cout << "Item ID: " << item.itemId << "\tName: " << item.name << "\tPrice: " << item.price << endl;
        }
    }
}

// Function to search a bakery item
void searchBakeryItem(const vector<BakeryItem>& items, const int& itemId) {

    cout << "      *===============================================================*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                 ******** WELCOME ********                   |*  " << endl;
    cout << "      *|                             TO                              |*  " << endl;
    cout << "      *|                        Amber Bakers                         |*  " << endl;
    cout << "      *|                ********         ********                    |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                      --Search Items--                       |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *===============================================================*  " << endl;
    cout << endl << endl;
    bool found = false;
    for (const BakeryItem& item : items) {
        if (item.itemId == itemId) {
            cout << "Item found:\n";
            cout << "Item ID: " << item.itemId << "\tName: " << item.name << "\tPrice: " << item.price << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout <<RED_COLOR<< "Item not found.\n"<<RESET_COLOR;
    }
}

// Function to update a bakery item
void updateBakeryItem(vector<BakeryItem>& items, const int& itemId) {

    cout << "      *===============================================================*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                 ******** WELCOME ********                   |*  " << endl;
    cout << "      *|                             TO                              |*  " << endl;
    cout << "      *|                        Amber Bakers                         |*  " << endl;
    cout << "      *|                ********         ********                    |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                     --Updates Items--                       |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *|                                                             |*  " << endl;
    cout << "      *===============================================================*  " << endl;


    bool found = false;
    for (BakeryItem& item : items) {
        if (item.itemId == itemId) {
            cout << "Updating item:\n";
            cout << "Item ID: " << item.itemId << "\tName: " << item.name << "\tPrice: " << item.price << endl;

            cout << "Enter updated item price: ";
            cin >> item.price;
            saveBakeryItemsToFile(items); // Save the updated list of items to the file
            cout << GREEN_COLOR<<"Item updated successfully.\n"<<RESET_COLOR;
            found = true;
            break;
        }
    }
    if (!found) {
        cout <<RED_COLOR<< "Item not found.\n"<<RESET_COLOR;
    }
}
// Function to handle the administrator menu
void administratorMenu(vector<BakeryItem>& items) {
            cout << R"(


 __       __          __                                                  __                                    
/  |  _  /  |        /  |                                                /  |                                   
$$ | / \ $$ | ______ $$ | _______  ______  _____  ____   ______         _$$ |_    ______                        
$$ |/$  \$$ |/      \$$ |/       |/      \/     \/    \ /      \       / $$   |  /      \                       
$$ /$$$  $$ /$$$$$$  $$ /$$$$$$$//$$$$$$  $$$$$$ $$$$  /$$$$$$  |      $$$$$$/  /$$$$$$  |                      
$$ $$/$$ $$ $$    $$ $$ $$ |     $$ |  $$ $$ | $$ | $$ $$    $$ |        $$ | __$$ |  $$ |                      
$$$$/  $$$$ $$$$$$$$/$$ $$ \_____$$ \__$$ $$ | $$ | $$ $$$$$$$$/         $$ |/  $$ \__$$ |                      
$$$/    $$$ $$       $$ $$       $$    $$/$$ | $$ | $$ $$       |        $$  $$/$$    $$/                       
$$______ $$/ $$$__$$/$$/ $$$$$$$__$$$$$$/ $$__ $$/  $$/ $$__$$$/          $$$$/  $$$__$/   __                   
 /      \      /  |            /  |        /  |          /  |                      /  |   /  |                  
/$$$$$$  | ____$$ |_____  ____ $$/ _______ $$/  _______ _$$ |_    ______  ______  _$$ |_  $$/  ______  _______  
$$ |__$$ |/    $$ /     \/    \/  /       \/  |/       / $$   |  /      \/      \/ $$   | /  |/      \/       \ 
$$    $$ /$$$$$$$ $$$$$$ $$$$  $$ $$$$$$$  $$ /$$$$$$$/$$$$$$/  /$$$$$$  $$$$$$  $$$$$$/  $$ /$$$$$$  $$$$$$$  |
$$$$$$$$ $$ |  $$ $$ | $$ | $$ $$ $$ |  $$ $$ $$      \  $$ | __$$ |  $$//    $$ | $$ | __$$ $$ |  $$ $$ |  $$ |
$$ |  $$ $$ \__$$ $$ | $$ | $$ $$ $$ |  $$ $$ |$$$$$$  | $$ |/  $$ |    /$$$$$$$ | $$ |/  $$ $$ \__$$ $$ |  $$ |
$$ |  $$ $$    $$ $$ | $$ | $$ $$ $$ |  $$ $$ /     $$/  $$  $$/$$ |    $$    $$ | $$  $$/$$ $$    $$/$$ |  $$ |
$$/   $$/ $$$$$$$/$$/  $$/  $$/$$/$$/   $$/$$/$$$$$$$/    $$$$/ $$/      $$$$$$$/   $$$$/ $$/ $$$$$$/ $$/   $$/ 
                                                                                                    
   )" << endl;

    while (true) {
        cout << "Administrator Menu:"<<endl<<endl;
        cout << "1. View Bakery Items\n";
        cout << "2. Add Bakery Item\n";
        cout << "3. Search Bakery Item\n";
        cout << "4. Update Bakery Item\n";
        cout << "5. Delete Bakery Item\n";
        cout << "6. Exit"<<endl<<endl;
        cout << "Enter your choice: ";
       

        int adminChoice;
        cin >> adminChoice;
        cout <<endl;
        
        switch (adminChoice) {
            case 1: viewBakeryItems(items); break;
            case 2: addBakeryItem(items); break;
            case 3: {
                cout << "Enter item id to search: ";
                int itemId;
                cin >> itemId;
                searchBakeryItem(items, itemId);
                break;
            }
            case 4: {
                cout << "Enter item id to update: ";
                int itemId;
                cin >> itemId;
                updateBakeryItem(items, itemId);
                break;
            }

              case 5: {
                cout << "Enter item id to delete: ";
                int itemId;
                cin >> itemId;
                deleteBakeryItem(items, itemId);
                break;
            }
            case 6: return; // Exit Administrator menu
            default:
                cout << RED_COLOR<<"Invalid choice.\n"<<RESET_COLOR;
                break;
        }
    }
}

// Function to calculate the total bill based on order items and bakery items
double calculateTotalBill(const vector<OrderItem>& orderItems, const vector<BakeryItem>& items) {
    double totalBill = 0.0;
    for (const OrderItem& orderItem : orderItems) {
        for (const BakeryItem& bakeryItem : items) {
            if (orderItem.itemId == bakeryItem.itemId) {
                totalBill += (bakeryItem.price * orderItem.quantity);
                break;
            }
        }
    }
    return totalBill;
}

// Function to save orders to a file
void saveOrdersToFile(const vector<Order>& orders) {
    ofstream outFile("orders.txt");
    for (const Order& order : orders) {
        outFile << "Total Bill: " << order.totalBill << endl;
        for (const OrderItem& item : order.items) {
            outFile << "Item ID: " << item.itemId << "\tQuantity: " << item.quantity << endl;
        }
    }
    outFile.close();
}

// Function to load orders from a file
void loadOrdersFromFile(vector<Order>& orders) {
    ifstream inFile("orders.txt");
    //orders.clear();
    Order order;
    while (inFile >> order.totalBill) {
        OrderItem item;
        while (inFile >> item.itemId >> item.quantity) {
            order.items.push_back(item);
        }
        orders.push_back(order);
      //  order.items.clear(); // Clear items for the next order
    }
    inFile.close();
}

void cashierMenu(const vector<BakeryItem>& items, vector<Order>& orders) {
    Order order; // Initialize an order
     
         cout << R"(



$$\      $$\         $$\                                                 $$\              
$$ | $\  $$ |        $$ |                                                $$ |             
$$ |$$$\ $$ |$$$$$$\ $$ |$$$$$$$\ $$$$$$\ $$$$$$\$$$$\  $$$$$$\        $$$$$$\   $$$$$$\  
$$ $$ $$\$$ $$  __$$\$$ $$  _____$$  __$$\$$  _$$  _$$\$$  __$$\       \_$$  _| $$  __$$\ 
$$$$  _$$$$ $$$$$$$$ $$ $$ /     $$ /  $$ $$ / $$ / $$ $$$$$$$$ |        $$ |   $$ /  $$ |
$$$  / \$$$ $$   ____$$ $$ |     $$ |  $$ $$ | $$ | $$ $$   ____|        $$ |$$\$$ |  $$ |
$$  /   \$$ \$$$$$$$\$$ \$$$$$$$\\$$$$$$  $$ | $$ | $$ \$$$$$$$\         \$$$$  \$$$$$$  |
\$$$$$$\ \__|\_______\__|\_$$\___|\_$$\__/\__| \__| \__|\_______|         \____/ \______/ 
$$  __$$\                  $$ |     \__|                                                  
$$ /  \__|$$$$$$\  $$$$$$$\$$$$$$$\ $$\ $$$$$$\  $$$$$$\                                  
$$ |      \____$$\$$  _____$$  __$$\$$ $$  __$$\$$  __$$\                                 
$$ |      $$$$$$$ \$$$$$$\ $$ |  $$ $$ $$$$$$$$ $$ |  \__|                                
$$ |  $$\$$  __$$ |\____$$\$$ |  $$ $$ $$   ____$$ |                                      
\$$$$$$  \$$$$$$$ $$$$$$$  $$ |  $$ $$ \$$$$$$$\$$ |                                      
 \______/ \_______\_______/\__|  \__\__|\_______\__|                                      
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
   )" << endl;


    while (true) {
        cout << "\nCashier Menu:" << endl;
        cout << "1. Add Item to Order\n";
        cout << "2. Print the Bill\n";
        cout << "3. Clear Orders List\n";
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";

        int cashierChoice;
        cin >> cashierChoice;
        cout << endl ;

        switch (cashierChoice) {
            case 1: {
                cout << "Available Items:\n";
                cout << "\n\t\t";
                cout << "______________________________________________ ";
                cout << "\n\t\t";
                cout << "|  Item ID.  |   Item Name   |  Price  ";
                cout << "\n\t\t";
                cout << "---------------------------------------------"<< endl<<endl;
                for (const BakeryItem& item : items) {
                    cout << "\t\t\t" << item.itemId << "\t" << item.name << "\t\t" << item.price << endl;
                }

                while (true) {
                    int itemId, qty;
                    cout << endl<<endl;
                    cout << "Enter item ID to add to the order (-1 to finish adding): ";
                    cin >> itemId;
                    

                    if (itemId == -1) {
                        if (order.items.empty()) {
                            cout  <<RED_COLOR<< "No items in order. Cannot finish order.\n" << RESET_COLOR;
                        } else {
                            cout <<GREEN_COLOR<< "Item(s) added to the order."<<RESET_COLOR<<endl;
                        }
                        break; // Exit the inner loop after finishing adding items
                    } else {
                        // Check if the entered item ID is valid
                        bool itemFound = false;
                        for (const BakeryItem& item : items) {
                            if (item.itemId == itemId) {
                                cout << "Enter quantity for item ID " << itemId << ": ";
                                cin >> qty;
                                order.items.push_back({ itemId, qty });
                                itemFound = true;
                                break;
                            }
                        }
                        if (!itemFound) {
                            cout << RED_COLOR << "Invalid item ID. Please enter a valid ID.\n" << RESET_COLOR;
                        }
                    }
                }
                break;
            }
            case 2: {
                if (order.items.empty()) {
                    cout << RED_COLOR << "No items in the order.\n" << RESET_COLOR;
                } else {
                    // Ask if the customer is a loyalty customer
                    char isLoyaltyCustomer;
                    cout << "Are you a loyalty customer? (y/n): ";
                    cin >> isLoyaltyCustomer;

                    // Calculate the total bill
                    double totalBill = calculateTotalBill(order.items, items);

                    // Apply a 2% discount if the customer is a loyalty customer
                    if (isLoyaltyCustomer == 'y' || isLoyaltyCustomer == 'Y') {
                        double discountAmount = 0.02 * totalBill;
                        totalBill -= discountAmount;
                        cout << "Loyalty customer discount (2%): $" << discountAmount << endl;
                    }

                    // Display the official receipt
                    cout << "\nAmber Bakery Receipt\n";
                    cout << "--------------------------------\n";

                    // Display each item in the order with quantity and price
                    for (const OrderItem& orderItem : order.items) {
                        for (const BakeryItem& bakeryItem : items) {
                            if (orderItem.itemId == bakeryItem.itemId) {
                                cout << "Item: " << bakeryItem.name << "\n";
                                cout << "Quantity: " << orderItem.quantity << "\n";
                                cout << "Price per item: $" << bakeryItem.price << "\n";
                                cout << "Subtotal: $" << bakeryItem.price * orderItem.quantity << "\n";
                                cout << "--------------------------------\n";
                                break;
                            }
                        }
                    }

                    // Display the total bill
                    cout << "Total Bill: $" << totalBill << "\n";
                    cout << "--------------------------------\n";

                    cout << "Thank you for using the cashier system.\n";

                    // Save the order here
                    orders.push_back(order);
                    saveOrdersToFile(orders); // Save the updated list of orders to the file
                }
                break;
            }
            case 3: {
                // Clear the current order
                order = Order();
                cout << "Order cleared.\n";
                break;
            }
            case 4: {
                return; // Exit the function
            }
            default: {
                cout << RED_COLOR << "Invalid choice. Please enter a valid option.\n" << RESET_COLOR;
                break;
            }
        }
    }
}


// Function to display the main menu and get user input
int displayMainMenu() {
    int choice;

   

    // Display text with the updated font size and style
    cout << BLUE_COLOR<<" Amber Bakery System\n"<<RESET_COLOR;
    cout<<BLUE_COLOR<<"----------------------------------------"<<RESET_COLOR;
    cout << "\nMain Menu:\n";
    cout << endl;
    cout << "1. Login\n";
    cout << "2. Sign Up\n";
    cout << "3. Logout\n";
    cout << endl;
    cout << "Enter your choice: ";
    cin >> choice;
     cout << endl ;   
    return choice;

}

/// Function to display text with animation
void displayTextWithAnimation(const string& text) {
    for (char c : text) {
        cout << c;
        Sleep(100); // Adjust the sleep duration for animation speed
    }
}

// Function to display the welcome message
void displayWelcomeMessage() {
    cout << "\n\n\n";

    
displayTextWithAnimation("WELCOME TO AMBER BAKERY!!!!\n\n");
    
}

// Function to handle the login/signup menu
void loginSignupMenu(User users[], int& numUsers, User& loggedInUser, vector<BakeryItem>& bakeryItems, vector<Order>& orders) {
     bool running = true; // Flag to control the loop
    while (running) {
        int choice = displayMainMenu();

        switch (choice) {
            case 1:
                if (loginUser(users, numUsers, loggedInUser)) {
                    if (loggedInUser.role == "cashier") {
                        cashierMenu(bakeryItems, orders);
                    } else if (loggedInUser.role == "admin") {
                        administratorMenu(bakeryItems);
                    }
                }
                break;

            case 2:
                signupUser(users, numUsers);
                break;

             case 3:
                if (!loggedInUser.username.empty()) {
                    // Clear the logged-in user's data to log them out
                    loggedInUser = {};
                    cout << GREEN_COLOR << "Logout successful.\n" << RESET_COLOR;
                } else {
                    cout << RED_COLOR << "You are not logged in. Please log in first.\n" << RESET_COLOR;
                }
                break;

            case 4:
                // Save user data, orders, and bakery items to files before exiting
                saveUsersToFile(users, numUsers);
                saveOrdersToFile(orders);
                saveBakeryItemsToFile(bakeryItems);
                return;

            default:
                cout << RED_COLOR << "Invalid choice. Please try again." << RESET_COLOR << endl;
                break;
        }
    }
}

void home(){
     int numUsers = 0; // Initialize the number of users to 0
    loadUsersFromFile(users, numUsers); // Load user data from the file into the array

    vector<BakeryItem> bakeryItems;
    loadBakeryItemFromFile(bakeryItems);

    vector<Order> orders; // Store orders

    User loggedInUser; // Store the logged-in user
   

    cout << R"(
                     _                             
     /\             | |                            
    /  \   _ __ ___ | |__   ___ _ __               
   / /\ \ | '_ ` _ \| '_ \ / _ | '__|              
  / ____ \| | | | | | |_) |  __| |                 
 /_/    \_|_| |_| |_|____/ \___|__                 
                    |  _ \      | |                
                    | |_) | __ _| | _____ _ __ ___ 
                    |  _ < / _` | |/ / _ | '__/ __|
                    | |_) | (_| |   |  __| |  \__ \
                    |____/ \__,_|_|\_\___|_|  |___/                                                                                                                                                                                                             
   )" << endl;

    // Display the welcome message
    displayWelcomeMessage();

    loginSignupMenu(users, numUsers, loggedInUser, bakeryItems, orders);


}

int main() {
    home();
   
    return 0;
}