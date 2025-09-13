#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include <unordered_map>
#include <climits>

struct Restuarant; 

extern int employeeIdCounter;

const int HASHMAP_SIZE = 100;
const int MAX_COLLISIONS = 10;

struct Employee {
    int employeeId;
    std::string name;
    std::string email;
    std::string password;
    Restuarant* employeeRst;

    Employee(int id, std::string name, std::string email, std::string password, Restuarant* empRst = nullptr);
    bool isEmployed();
    void displayEmployee();
    void editEmployee();
};

extern Employee* employeeHashmap[HASHMAP_SIZE][MAX_COLLISIONS];

// Function declarations
int getHashKey(int id);
bool verifyPassword(std::string& password);
void addEmployee(Restuarant*& restuarant);
Employee* findEmployee(int id);
Employee* loginEmployee();
