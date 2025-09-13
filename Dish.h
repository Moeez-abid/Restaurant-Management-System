#pragma once
#include <iostream>
#include <string>

const std::string BLUE = "\033[38;5;39m";
const std::string YELLOW = "\033[38;5;226m";
const std::string RED = "\033[38;5;196m";
const std::string GREEN = "\033[38;5;46m";
const std::string RESET = "\033[0m";

extern int dishIdCounter;

struct Dish {
    int dishId;
    std::string name;
    std::string type;
    float price;
    Dish* left;
    Dish* right;

    // Constructor declaration
    Dish();
    Dish(int id, std::string name, std::string type, float price);

    // Function declaration
    void display() const;
    void displayTable();
};
