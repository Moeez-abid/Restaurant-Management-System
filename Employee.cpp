#include "Employee.h"
#include "Restuarant.h"
#include <cctype> 

int employeeIdCounter = 10000;
Employee* employeeHashmap[HASHMAP_SIZE][MAX_COLLISIONS] = { nullptr };

int getHashKey(int id) {
    return id % HASHMAP_SIZE;
}

Employee::Employee(int id, std::string name, std::string email, std::string password, Restuarant* empRst)
    : employeeId(id), name(name), email(email), password(password), employeeRst(empRst) {
}

bool Employee::isEmployed() {
    return employeeRst;
}

void Employee::displayEmployee() {
    std::cout << YELLOW << "Employee ID: " << RESET<< employeeId << std::endl;
    std::cout << YELLOW << "Name: " << RESET<< name << std::endl;
    std::cout << YELLOW << "Email: " << RESET<< email << std::endl;
    if (isEmployed()) std::cout << "Currently works at: " << employeeRst->name << std::endl;
    else std::cout << "Currently Not Employed" << std::endl;
}

bool verifyPassword(std::string& password) {
    if (password.size() < 8 || password.size() > 12) {
        return false; // Ensure password length is between 8-12 characters
    }

    bool hasCapital = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (char ch : password) {
        if (std::isupper(ch)) hasCapital = true;
        else if (std::islower(ch)) hasLower = true;
        else if (std::isdigit(ch)) hasDigit = true;
        else if (std::ispunct(ch)) hasSpecial = true; // Check for punctuation as special characters
    }

    return hasCapital && hasLower && hasDigit && hasSpecial;
}


void addEmployee(Restuarant*& restuarant) {
    std::string name;
    std::cout << "Enter employee name: ";
    std::cin.ignore();
    std::getline(std::cin, name);

    std::string email;
    std::cout << "Enter email: ";
    std::cin >> email;

    std::string password;
    std::cout << "Enter password: ";
    std::cin >> password;

    while (!verifyPassword(password)) {
        std::cout << RED << "Invalid password! It must contain at least 1 capital letter, 1 lowercase letter, 1 digit, and 1 special character, and be between 8-12 characters long." << RESET<< std::endl;
        std::cout << "Enter password: ";
        std::cin >> password;
    }

    Employee* newEmployee = new Employee(employeeIdCounter, name, email, password, restuarant);
    int hashKey = getHashKey(employeeIdCounter);

    for (int i = 0; i < MAX_COLLISIONS; i++) {
        if (!employeeHashmap[hashKey][i]) {
            employeeHashmap[hashKey][i] = newEmployee;
            std::cout << "Employee signup successful!" << std::endl;
            newEmployee->displayEmployee();
            employeeIdCounter++;
            return;
        }
    }

    std::cout << RED << "Failed to add employee. Collision limit exceeded for bucket " << hashKey << "." << RESET<< std::endl;
    employeeIdCounter++;
}

Employee* findEmployee(int id) {
    int hashKey = getHashKey(id);

    for (int i = 0; i < MAX_COLLISIONS; i++) {
        if (employeeHashmap[hashKey][i] != nullptr && employeeHashmap[hashKey][i]->employeeId == id)
            return employeeHashmap[hashKey][i];
    }
    return nullptr;
}

Employee* loginEmployee() {
    int id;
    std::cout << "Enter employee ID: ";
    std::cin >> id;

    Employee* thisEmployee = findEmployee(id);
    if (!thisEmployee) {
        std::cout << RED << "Invalid ID!"  << RESET << std::endl;
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

    if (thisEmployee->password == password) {
        std::cout << GREEN << "Login successful! Welcome, " << thisEmployee->name << RESET << std::endl;
        return thisEmployee;
    }
    else {
        std::cout << RED << "Invalid password!" << RESET<< std::endl;
        return nullptr;
    }
}

void Employee::editEmployee() {
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
            std::cout << GREEN << "Name updated successfully!"  << RESET << std::endl;
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
                std::cout << GREEN << "Password updated successfully!" << RESET << std::endl;
                return;
            }
            else {
                std::cout << RED << "Incorrect current password. No changes made." << RESET << std::endl;
                return;
            }

        }
        else if (choice == 3) {
            std::cout << "Going back..." << std::endl;
            break;

        }
        else {
            std::cout << RED << "Invalid choice! Please enter 1, 2, or 3." << RESET << std::endl;
        }
    }
}

