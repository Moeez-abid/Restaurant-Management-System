#pragma once
#include <iostream>
#include "Dish.h"
#include "Employee.h"
#include "Customer.h"
#include "Promotions.h"

extern int orderIdCounter;

struct DishLinkedListNode {
	Dish* dish;
	DishLinkedListNode* next;

	DishLinkedListNode();
};


struct Orders {
	int orderId;
	DishLinkedListNode* head;
	double total;
	int type; // 1 for premium, 2 for express, 3 for normal
	Restuarant* rest;
	Customer* customer;
	Employee* waiter;
	Employee* deliveryDriver;
	std::string destinationLocation;
	std::string restaurantLocation;
	Orders* next;

	Orders(int id, DishLinkedListNode* dishes, double& total, int type, Customer* customer = NULL,Restuarant * rest = NULL, Employee* waiter = NULL);
};

extern Orders* orderQueueHead;

void addOrder(DishLinkedListNode* dishes, int dishCount, int type, Customer* customer, Promotion*& head, Restuarant * rest, std::string location);
void processOrder(Employee* employee, Orders*& processedOrders);
void displayOrders();
