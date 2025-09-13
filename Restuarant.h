#pragma once
#include <string>
#include "Menu.h"
#include "Employee.h"
#include "Orders.h"
#include<iomanip>


struct Employee;

extern int restuarantIdCounter;

struct Restuarant {
    int restuarantId;
    std::string name;
    std::string type;
    Menu menu;
    Employee* manager;
    std::string restaurantLocation;
    Restuarant* next;

    Restuarant(int id, std::string name, std::string type, Employee* manager, std::string menuName, std::string restaurantLocation);
    void displayRestuarant();
    void displayTable();
};

// Function declarations
Restuarant* addRestuarant(Restuarant* root, Restuarant* restuarant);
Restuarant* removeRestuarant(Restuarant* root, int id);
Restuarant* getRestuarant(Restuarant* root, int id);
Restuarant* editRestuarant(Restuarant* root);
void removeOrderFromQueue(Restuarant* toRemove);
