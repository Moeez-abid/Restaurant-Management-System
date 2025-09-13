#include "Dish.h"
#include <iostream>
#include<iomanip>

int dishIdCounter = 10000;

// default constructor
Dish::Dish() {
    dishId = dishIdCounter++;
    name = "\0";
    type = 3;
    price = 0;
    left = NULL;
    right = NULL;
}

// Constructor definition
Dish::Dish(int id, std::string name, std::string type, float price)
    : dishId(id), name(name), type(type), price(price), left(nullptr), right(nullptr) {}

// Display function definition
void Dish::display() const {
    std::cout << "Dish ID: " << dishId << std::endl;
    std::cout << "Dish Name: " << name << std::endl;
    std::cout << "Dish Type: " << type << std::endl;
    std::cout << "Dish Price: " << price << std::endl;
}

// display table of dishes
void Dish::displayTable() {
    std::cout << YELLOW << "| " << RESET << std::left << std::setw(6) << dishId << YELLOW << "| " << RESET << std::left << std::setw(25) << name << YELLOW << "| " << RESET << std::left << std::setw(25) << type << YELLOW << "| " << RESET << std::left << std::setw(10) << price << YELLOW << "| " << RESET << std::endl;
    std::cout << YELLOW << "---------------------------------------------------------------------------" << RESET << std::endl;
}