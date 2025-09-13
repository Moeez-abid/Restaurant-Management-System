#include "Menu.h"
#include <iostream>
#include <queue>

int menuIdCounter = 10000;

Menu::Menu(int id, std::string name) : menuId(id), name(name), root(nullptr), totalDishes(0) {}

void Menu::addDish(int id, std::string name, std::string type, float price) {
    if (root == nullptr) {                                      // if root is null
        root = new Dish(id, name, type, price);
        totalDishes++;
        std::cout << GREEN << "Dish " << name << " added successfully!" << RESET << std::endl;
        return;
    }

    std::queue<Dish*> q;
    q.push(root);                                           // pushing root in queue
                                                            // level order addition in tree
    while (!q.empty()) {
        Dish* temp = q.front();
        q.pop();

        if (temp->left != nullptr) {                        // if left child exists, then push
            q.push(temp->left);
        }
        else {
            temp->left = new Dish(id, name, type, price);       // if it does not exist then add new dish
            std::cout << GREEN << "Dish " << name << " added successfully!" << RESET << std::endl;
            return;
        }

        if (temp->right != nullptr) {                       // if right child exists, then push
            q.push(temp->right);
        }
        else {                                              // else add dish to right
            temp->right = new Dish(id, name, type, price);
            std::cout << GREEN << "Dish " << name << " added successfully!"  << RESET << std::endl;
            return;
        }
    }
}

void removeDishFromOrder(Dish * toRemove) {                 // function to remove dish from the unprocessed orders
    Orders* tempOrder = orderQueueHead;

    while (tempOrder)                                       // loop to iterate orders queue
    {
        DishLinkedListNode* tempDishNode = tempOrder->head;
        if (tempDishNode->dish == toRemove)                 // if first order has that dish in order
        {
            tempOrder->head = tempOrder->head->next;        

        }
        tempDishNode = tempOrder->head;                     
        DishLinkedListNode* prevDishNode = tempOrder->head;
        while (tempDishNode)                                // loop to iterate dishes linked list in current order
        {
            if (tempDishNode->dish == toRemove)
            {
                prevDishNode->next = tempDishNode->next;        // if dish is found, then remove it from linked list
            }
            prevDishNode = tempDishNode;
            tempDishNode = tempDishNode->next;
        }
        tempDishNode = tempOrder->head;
        tempOrder->total = 0;
        while (tempDishNode)
        {
            tempOrder->total += tempDishNode->dish->price;      // recalculating the total after removing dishes
            tempDishNode = tempDishNode->next;
        }
        if (tempOrder->type == 1) {                             // adding 500 for premium
            tempOrder->total += 500;
        }
        else if (tempOrder->type == 2) {                        // multiplying by 1.1 for express
            tempOrder->total *= 1.10;
        }
        tempOrder = tempOrder->next;
    }
}

void Menu::removeDish(int id) {
    if (!root) {
        std::cout << RED << "Menu is empty! No dish to remove."  << RESET<< std::endl;
        return;                                     // if menu is empty
    }

    if (root->dishId == id && !root->left && !root->right) {        // if there is only root and it is to be deleted
        removeDishFromOrder(root);
        root = nullptr;
        totalDishes--;
        std::cout << GREEN << "Dish removed successfully!"  << RESET << std::endl;
        return;
    }
    std::queue<Dish*> q;                            // queue to check last child of the tree and find target dish
    q.push(root);
    Dish* target = nullptr;
    Dish* last = nullptr;

    while (!q.empty()) {
        last = q.front();
        q.pop();

        if (last->dishId == id) {
            target = last;                          // if dish is found
        }

        if (last->left) q.push(last->left);
        if (last->right) q.push(last->right);
    }

    if (!target) {                                 // if dish is not found
        std::cout << RED << "Dish with ID " << id << " not found!" << RESET << std::endl;
        return;
    }

    while (!q.empty()) {
        Dish* current = q.front();
        q.pop();

        if (current->left) {                                    // swapping target with last (pointers)
            if (current->left == target) {
                current->left = last;
            }
            else if (current->left == last)
            {
                current->left = target;
            }
            q.push(current->left);
        }

        if (current->right) {
            if (current->right == target) {
                current->right = last;
            }
            else if (current->right == last)
            {
                current->right = target;
            }
            q.push(current->right);
        }
    }

    q.push(root);
    Dish* parent = nullptr;

    while (!q.empty()) {
        Dish* current = q.front();
        q.pop();

        if (current->left) {
            if (current->left == last) {
                parent = current;
                break;
            }
            q.push(current->left);
        }

        if (current->right) {
            if (current->right == last) {
                parent = current;
                break;
            }
            q.push(current->right);
        }
    }

    if (parent) {
        if (parent->left == last) {
            removeDishFromOrder(target);
            parent->left = nullptr;
        }
        else if (parent->right == last) {
            removeDishFromOrder(target);
            parent->right = nullptr;
        }
    }

    totalDishes--;
    std::cout << GREEN << "Dish removed successfully!" << RESET  << std::endl;
}

void Menu::editDish(int ID) {
    if (!root) {
        std::cout << RED << "Menu is empty! No dish to edit." << RESET << std::endl;
        return;                                 // if menu is empty
    }
    std::queue<Dish*> q;
    q.push(root);

    while (!q.empty()) {
        Dish* temp = q.front();
        q.pop();

        if (temp->dishId == ID) {               // if dish is found
            int choice = 0;
            std::string toChange = "\0";
            float toChangePrice = 0;
            std::cout << "1. Change Name" << std::endl;
            std::cout << "2. Change type" << std::endl;
            std::cout << "3. Change Price" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> choice;             
            if (choice == 1)
            {
                std::cout << "Enter new Name: " << std::endl;           // editing name
                std::cin.ignore();
                std::getline(std::cin, toChange);
                temp->name = toChange;
            }
            else if (choice == 2)
            {
                std::cout << "Enter new Type: " << std::endl;
                std::cin.ignore();                                      // editing type of dish
                std::getline(std::cin, toChange);
                temp->type = toChange;
            }
            else if (choice == 3)
            {
                std::cout << "Enter new Price: " << std::endl;
                std::cin >> toChangePrice;                              // editing price of dish
                while (toChangePrice < 0)
                {
                    std::cout << RED << "Price Cannot be negative. Enter Again: " << RESET << std::endl;
                    std::cin >> toChangePrice;
                }
                temp->price = toChangePrice;
            }
            return;
        }

        if (temp->left) q.push(temp->left);
        if (temp->right) q.push(temp->right);
    }
    std::cout << RED << "Dish with this Id not found" << RESET << std::endl;


}

// Display dishes in-order
void Menu::displayMenu(Dish* current) const {
    if (!current) return;
    displayMenu(current->left);
    current->display();
    displayMenu(current->right);
}
void Menu::displayMenuTable(Dish* current) {
    if (!current) return;
    displayMenuTable(current->left);
    current->displayTable();
    displayMenuTable (current->right);
}

bool isDishInMenu(const Menu& menu, int id) {                       // to check if dish is in menu or not 
    if (!menu.root) return false;

    std::queue<Dish*> q;
    q.push(menu.root);

    while (!q.empty()) {
        Dish* temp = q.front();
        q.pop();

        if (temp->dishId == id) return true;                        // if dish is found then return true

        if (temp->left) q.push(temp->left);
        if (temp->right) q.push(temp->right);
    }
    return false;                                               // return false if dish is not found
}

Dish* Menu::findDish(int id) {                                 // function to find specific dish in menu tree by id
    if (!root) return NULL;

    std::queue<Dish*> q;
    q.push(root);

    while (!q.empty()) {
        Dish* temp = q.front();
        q.pop();

        if (temp->dishId == id) return temp;                 // if dish is found then return the dish

        if (temp->left) q.push(temp->left);
        if (temp->right) q.push(temp->right);
    }
    return NULL;                                            // return null if dish is not found
}