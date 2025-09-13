#include "Customer.h"

int customerIdCounter = 10000;
Customer* customerHashmap[HASHMAP_SIZE_CUSTOMER][MAX_COLLISIONS_CUSTOMER] = { NULL };

int getHashKeyCustomer(int id) {
	return id % HASHMAP_SIZE_CUSTOMER;
}

Customer::Customer(int id, std::string name, std::string email, std::string password) 
	: customerId(id), name(name), email(email), password(password) {}


void Customer::displayCustomer() {
    std::cout << YELLOW << "Customer ID: " << RESET  << customerId << std::endl;
    std::cout << YELLOW << "Name: " << RESET << name << std::endl;
    std::cout << YELLOW << "Email: " << RESET << email << std::endl;
}

void addCustomer() {
    std::string name;
    std::cout << "Enter customer name: ";
    std::cin.ignore(); 
    std::getline(std::cin, name); 

    std::string email;
    std::cout << "Enter email: ";
    std::getline(std::cin, email); 

    std::string password = "\0";
    char ch;
    std::cout << "Enter password: ";
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else {
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;

    while (!verifyPassword(password)) {
        std::cout << "Invalid password! It must contain at least 1 capital letter, 1 lowercase letter, 1 digit, and 1 special character, and be between 8-12 characters long." << std::endl;
        std::cout << "Enter password: ";
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            }
            else {
                password += ch;
                std::cout << '*';
            }
        }
        std::cout << std::endl;
    }

    // Create a new customer
    Customer* newCustomer = new Customer(customerIdCounter, name, email, password);
    int hashKey = getHashKeyCustomer(customerIdCounter);

    for (int i = 0; i < MAX_COLLISIONS_CUSTOMER; i++) {
        if (!customerHashmap[hashKey][i]) {
            customerHashmap[hashKey][i] = newCustomer;
            customerIdCounter++;
            std::cout << "Customer signup successful!" << std::endl;
            newCustomer->displayCustomer();
            return;
        }
    }

    std::cout << "Failed to add customer. Collision limit exceeded for bucket " << hashKey << "." << std::endl;
    std::cout << "Try signing up again to get another auto_generated ID" << std::endl;
    customerIdCounter++;
}


Customer* findCustomer(int id) {
    int hashKey = getHashKeyCustomer(id);

    for (int i = 0; i < MAX_COLLISIONS_CUSTOMER; i++) {
        if (customerHashmap[hashKey][i] != nullptr && customerHashmap[hashKey][i]->customerId == id)
            return customerHashmap[hashKey][i];
    }
    return nullptr;
}

Customer* loginCustomer() {
    int id;
    std::cout << "Enter customer ID: ";
    std::cin >> id;

    Customer* thisCustomer = findCustomer(id);
    if (!thisCustomer) {
        std::cout << "Invalid ID!" << std::endl;
        return nullptr;
    }

    std::string password;
    char ch;
    std::cout << "Enter password: ";
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else {
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;

    if (thisCustomer->password == password) {
        std::cout << "Login successful! Welcome, " << thisCustomer->name << std::endl;
        return thisCustomer;
    }
    else {
        std::cout << "Invalid password!" << std::endl;
        return nullptr;
    }
}

void Customer::editCustomer() {
    while (true) {
        std::cout << "\nWhat would you like to edit?" << std::endl;
        std::cout << "1. Name" << std::endl;
        std::cout << "2. Reset Password" << std::endl;
        std::cout << "3. Go back" << std::endl;
        std::cout << "Enter your choice: ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "Enter new name: ";
            std::cin.ignore(); 
            std::getline(std::cin, name);
            std::cout << "Name updated successfully!" << std::endl;
            return;
        }
        else if (choice == 2) {
            std::string oldPassword;
            std::cout << "Enter your current password: ";
            char ch;
            while ((ch = _getch()) != '\r') { 
                if (ch == '\b') { 
                    if (!oldPassword.empty()) {
                        oldPassword.pop_back();
                        std::cout << "\b \b";
                    }
                }
                else {
                    oldPassword += ch;
                    std::cout << '*';
                }
            }
            std::cout << std::endl;

            if (oldPassword == password) {
                std::string newPassword;
                std::cout << "Enter new password: ";
                std::cin >> newPassword;

                while (!verifyPassword(newPassword)) {
                    std::cout << "Invalid password! Password must:\n"
                        << " - Be 8-12 characters long\n"
                        << " - Contain at least 1 uppercase letter\n"
                        << " - Contain at least 1 lowercase letter\n"
                        << " - Contain at least 1 digit\n"
                        << " - Contain at least 1 special character\n"
                        << "Enter new password: ";
                    std::cin >> newPassword;
                }

                password = newPassword;
                std::cout << "Password updated successfully!" << std::endl;
            }
            else {
                std::cout << "Incorrect current password. No changes made." << std::endl;
            }
            return;
        }
        else if (choice == 3) {
            std::cout << "Going back..." << std::endl;
            break;

        }
        else {
            std::cout << "Invalid choice! Please enter 1, 2, or 3." << std::endl;
        }
    }
}
