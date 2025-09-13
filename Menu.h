#pragma once
#include "Dish.h"
#include "Orders.h"
#include <queue>
#include <string>
#include<iomanip>

extern int menuIdCounter;

struct Menu {
    int menuId;
    std::string name;
    Dish* root;
    int totalDishes;

    Menu(int id, std::string name);
    void addDish(int id, std::string name, std::string type, float price);
    void removeDish(int id);
    void displayMenu(Dish* current) const;
    void displayMenuTable(Dish * current);
    Dish* findDish(int id);
    void editDish(int ID);
};

// Helper function declarations
bool isDishInMenu(const Menu& menu, int id);
