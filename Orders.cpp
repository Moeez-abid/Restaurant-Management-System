#include "Orders.h"

int orderIdCounter = 10000;


DishLinkedListNode::DishLinkedListNode() {
    dish = NULL;
    next = NULL;
}

Orders* orderQueueHead = nullptr;

Orders::Orders(int id, DishLinkedListNode* head, double& total, int type, Customer* customer,Restuarant * rest, Employee* waiter)
    : orderId(id), head(head), total(total), type(type), waiter(waiter), customer(customer), deliveryDriver(NULL), next(nullptr), rest(rest),
        restaurantLocation(""), destinationLocation("") {}

void addOrder(DishLinkedListNode* dishHead, int dishCount, int type, Customer* customer, Promotion*& head, Restuarant * rest, std::string location) {
    double totalCost = 0.0;

    // Calculate the total cost of the dishes
    DishLinkedListNode* temp = dishHead;
    while (temp)
    {
        totalCost += temp->dish->price;
        temp = temp->next;
    }

    // Create a new order
    Orders* newOrder = new Orders(orderIdCounter++, dishHead, totalCost, type, customer, rest, NULL);
    newOrder->destinationLocation = location;

    // Insert the new order into the priority queue
    if (!orderQueueHead || newOrder->type < orderQueueHead->type) {
        newOrder->next = orderQueueHead;
        orderQueueHead = newOrder;
    }
    else {
        Orders* current = orderQueueHead;
        while (current->next && current->next->type <= newOrder->type) {
            current = current->next;
        }
        newOrder->next = current->next;
        current->next = newOrder;
    }

    // Apply promotions, if available
    while (head && head->limit == head->numPromotionsApplied) {
        // Pop promotion if its limit is reached
        head = checkLimit(head);
    }

    if (head) {
        // Apply the current promotion
        double discountAmount = (head->discount / 100.0) * totalCost;
        totalCost -= discountAmount;

        std::cout << "Promotion applied: " << head->name
            << ", Discount: " << head->discount << "%, Amount Saved: Rs. " << discountAmount << std::endl;
    }

    std::cout << "Applying delivery charges (if any)..." << std::endl;
    // Update the order's cost
    newOrder->total = totalCost;

    // Apply additional charges based on order type
    if (type == 1) { // Premium
        newOrder->total += 500;
    }
    else if (type == 2) { // Express
        newOrder->total += 200; //delivery charges
        newOrder->total *= 1.10;
    }
    else if (type == 3) { // normal
        newOrder->total += 100;  // delivery charges
    }

    // Display order confirmation
    std::cout << "Order added successfully! Order ID: " << newOrder->orderId
        << ", Total Cost: Rs. " << newOrder->total << std::endl;
}


void processOrder(Employee* employee, Orders*& processedOrders) {
    if (!orderQueueHead) {
        std::cout << "No orders to process!" << std::endl;
        return;
    }

    Employee* tempDelDrv = NULL;
    int delDrvId;
    std::cout << "Enter the Id of the delivery Driver who will deliver the order: ";
    std::cin >> delDrvId;
    tempDelDrv = findEmployee(delDrvId);

    while (!tempDelDrv) {
        std::cout << RED << "Enter a valid Delivery Driver Id: " << RESET;
        std::cin >> delDrvId;
        tempDelDrv = findEmployee(delDrvId);
    }

    Orders* highestPriorityOrder = orderQueueHead;
    orderQueueHead = orderQueueHead->next;

    highestPriorityOrder->waiter = employee;
    highestPriorityOrder->deliveryDriver = tempDelDrv;

    if (!processedOrders) {
        processedOrders = highestPriorityOrder; 
        highestPriorityOrder->next = nullptr;
    }
    else {
        Orders* current = processedOrders;
        while (current->next) {
            current = current->next; 
        }
        current->next = highestPriorityOrder;
        highestPriorityOrder->next = nullptr;
    }

    std::cout << "Processed Order ID: " << highestPriorityOrder->orderId << std::endl;
    std::cout << "Total: Rs. " << highestPriorityOrder->total << std::endl;
    std::cout << "Type: " << (highestPriorityOrder->type == 1 ? "Premium"
        : (highestPriorityOrder->type == 2 ? "Express" : "Normal"))
        << std::endl;
    std::cout << "Processed by: " << employee->name << std::endl;
}


void displayOrders() {
    if (!orderQueueHead) {
        std::cout << "No orders in the queue!" << std::endl;
        return;
    }

    Orders* current = orderQueueHead;
    while (current) {
        std::cout << YELLOW << "Order ID: " << RESET << current->orderId
            << YELLOW << ", Total: Rs. " << RESET << current->total
            << YELLOW << ", Type: "  << RESET << (current->type == 1 ? "Premium" : (current->type == 2 ? "Express" : "Normal")) << std::endl;
        DishLinkedListNode* tempDish = current->head;
        while (tempDish)
        {
            std::cout << YELLOW << "Name : " << RESET  <<  tempDish->dish->name<< std::endl;
            std::cout << YELLOW << "Price : " << RESET << tempDish->dish->price << std::endl;
            std::cout << std::endl;
            tempDish = tempDish->next;
        }
        std::cout << std::endl << std::endl;
        current = current->next;
    }
}

