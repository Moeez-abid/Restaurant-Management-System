#include "Restuarant.h"
#include <iostream>

int restuarantIdCounter = 10000;

Restuarant::Restuarant(int id, std::string name, std::string type, Employee* manager, std::string menuName, std::string restaurantLocation)
    : restuarantId(id), name(name), menu(menuIdCounter++, menuName), type(type), manager(manager), restaurantLocation(), next(NULL) {}

Restuarant* addRestuarant(Restuarant* root, Restuarant* restuarant) {
    if (root == NULL) {                                                     // checking if the root is null
        std::cout << GREEN << "Restuarant added successfully!" << RESET << std::endl;         // adding restaurant at the root
        return restuarant;
    }

    Restuarant* temp = root;
    while (temp->next != NULL) {
        temp = temp->next;                                                  // traversing to the last node of linked list
    }

    temp->next = restuarant;                                                // adding new restaurant
    std::cout << GREEN << "Restuarant added successfully!" << RESET << std::endl;

    return root;
}

void removeOrderFromQueue(Restuarant* toRemove) {
    Orders* current = orderQueueHead;
    Orders* prev = NULL;
    bool deleted = true;
    while (deleted)
    {                                                   // function to remove all orders of the deleted restaurant
        deleted = false;
        current = orderQueueHead;
        prev = NULL;
        while (current)
        {
            if (current->rest == toRemove)
            {
                if (prev)
                {
                    prev->next = current->next;
                    deleted = true;
                    break;
                }
                else
                {
                    orderQueueHead = orderQueueHead->next;
                    deleted = true;
                    break;
                }
            }
            prev = current;
            current = current->next;
        }
    }
}

Restuarant* removeRestuarant(Restuarant* root, int id) {
    if (root == NULL) {                                             // checking if the root is null, then no restaurant is there to remove
        std::cout << RED << "No restuarant to remove!" << RESET << std::endl;
        return NULL;
    }

    if (root->restuarantId == id) {          // if restaurant to remove is the root, then deleting the root, and returning the new root
        std::string name = root->name;
        Restuarant* temp = root->next;
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if (employeeHashmap[i][j])          // storing null in the restaurants pointers in employee
                {
                    if (employeeHashmap[i][j]->employeeRst == root)
                    {
                        employeeHashmap[i][j]->employeeRst = NULL;
                    }
                }
            }
        }
        removeOrderFromQueue(root);
        std::cout << GREEN << "Restuarant (ID: " << id << ") " << name << " successfully deleted!" << RESET << std::endl;
        return temp;
    }

    Restuarant* prev = root;
    Restuarant* temp = root->next;

    while (temp != NULL) {
        if (temp->restuarantId == id) {             // if id is found
            std::string name = temp->name;          // storing the name in the temporary string
            prev->next = temp->next;                // pointing the previous pointer's next to current's next, skipping current
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    if (employeeHashmap[i][j])          // storing null in the restaurants pointers in employee
                    {
                        if (employeeHashmap[i][j]->employeeRst == temp)
                        {
                            employeeHashmap[i][j]->employeeRst = NULL;
                        }
                    }
                }
            }
            removeOrderFromQueue(temp);                            
            std::cout << GREEN << "Restuarant (ID: " << id << ") " << name << " successfully deleted!" << RESET<< std::endl;
            return root;                      
        }
        prev = temp;
        temp = temp->next;
    }
    std::cout  << RED<< "Restuarant (ID: " << id << ") not found" << RESET << std::endl;
    return root;                                    // returning the unchanged root
}

Restuarant* getRestuarant(Restuarant* root, int id) {
    if (root == NULL) return root;

    Restuarant* temp = root;

    while (temp != NULL) {
        if (temp->restuarantId == id) break;                    // if id is found, then break
        temp = temp->next;
    }
    return temp;
}

Restuarant* editRestuarant(Restuarant* root) {
    if (root == NULL) {
        std::cout << "No restaurants available to edit." << std::endl;
        return root;
    }

    int id;
    std::cout << "Enter the ID of the restaurant you want to edit: ";       // id input from user
    std::cin >> id;

    Restuarant* restuarantToEdit = getRestuarant(root, id);
    if (restuarantToEdit == NULL) {
        std::cout << "Restaurant with ID " << id << " does not exist." << std::endl;
        return root;                                        // if restaurant is not found
    }

    std::cout << "Editing Restaurant: " << restuarantToEdit->name << std::endl;
    std::cout << "What would you like to edit?" << std::endl;
    std::cout << "1. Name" << std::endl;
    std::cout << "2. Type" << std::endl;    
    std::cout << "3. Manager (Employee ID)" << std::endl;
    std::cout << "Enter your choice: ";
    int choice;     
    std::cin >> choice;

    switch (choice) {
    case 1: {
        std::cout << "Enter new name: ";
        std::cin.ignore();
        std::string newName;                            // editing the name
        std::getline(std::cin, newName);
        restuarantToEdit->name = newName;
        std::cout << GREEN<< "Restaurant name updated successfully!" << RESET<< std::endl;
        break;
    }
    case 2: {
        std::cout << "Enter new type: ";
        std::cin.ignore();
        std::string newType;
        std::getline(std::cin, newType);                // editing the type
        restuarantToEdit->type = newType;
        std::cout << GREEN << "Restaurant type updated successfully!" << RESET<< std::endl;
        break;
    }
    case 3: {
        std::cout << "Enter new manager's Employee ID: ";
        int managerId;
        std::cin >> managerId;                          // input manager id 

        Employee* newManager = findEmployee(managerId);
        if (newManager == NULL) {                       // if manager with this id is not found
            std::cout << RED << "Employee with ID " << managerId << " does not exist." << RESET<< std::endl;
        }
        else if (newManager->employeeRst != restuarantToEdit)
        {                                               // if employee does not belong to this restaurant
            std::cout << RED << "Employee does not belong to this restaurant" << RESET << std::endl;
        }
        else {
            restuarantToEdit->manager = newManager;     // updating restaurant manager
            std::cout << GREEN << "Restuarant manager updated successfully!" << RESET << std::endl;
        }
        break;
    }
    default:
        std::cout << GREEN<< "Invalid choice. No changes made." << RESET<< std::endl;
        break;
    }
    return root;
}

void Restuarant::displayRestuarant() {                  // display function
    std::cout << YELLOW << "Restuarant ID: " << RESET << restuarantId << std::endl;
    std::cout << YELLOW << "Name: " << RESET << name << std::endl;
    std::cout << YELLOW << "Type: " << RESET << type << std::endl;

    std::cout << YELLOW << "\nManager Details:\n" << RESET;
    if (manager) {
        manager->displayEmployee(); 
    }
    else {
        std::cout << "No manager assigned." << std::endl;
    }

    std::cout << YELLOW << "\nMenu Details:\n" << RESET;
    menu.displayMenu(menu.root); 
}
void Restuarant::displayTable() {                       // display table of restaurants
    std::cout << YELLOW << "| " << RESET << std::left << std::setw(6) << restuarantId << YELLOW << "| " << RESET << std::left << std::setw(25) << name << YELLOW << "| " << RESET << std::left << std::setw(25) << type << YELLOW << "| " << RESET << std::left << std::setw(25) << manager->name << YELLOW << "| " << RESET << std::endl;
    std::cout << YELLOW << "------------------------------------------------------------------------------------------" << RESET << std::endl;

}
