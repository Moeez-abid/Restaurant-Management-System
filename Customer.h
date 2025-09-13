#pragma once
#include <iostream>
#include "Employee.h"
#include "Dish.h"

extern int customerIdCounter;

const int HASHMAP_SIZE_CUSTOMER = 100;
const int MAX_COLLISIONS_CUSTOMER = 10;

struct Customer {
	int customerId;
	std::string name;
	std::string email;
	std::string password;

	Customer(int id, std::string name, std::string email, std::string password);
	void displayCustomer();
	void editCustomer();
};

extern Customer* customerHashmap[HASHMAP_SIZE_CUSTOMER][MAX_COLLISIONS_CUSTOMER];

void addCustomer();
Customer* findCustomer(int id);
Customer* loginCustomer();
int getHashKeyCustomer(int id);