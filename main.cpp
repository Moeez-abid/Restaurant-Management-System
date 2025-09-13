#include "Restuarant.h"
#include "Employee.h"
#include "Customer.h"
#include "Orders.h"
#include "Promotions.h"
#include "Reviews.h"
#include "sstream"
#include "fstream"
#include "iomanip"

Orders* processedOrders = NULL;

//Employee* getEmployee(std::string email) {
//    for (int i = 0; i < 100; i++)
//    {
//        for (int j = 0; j < 10; j++)
//        {
//            if (email == employeeHashmap[i][j]->email)
//            {
//                return employeeHashmap[i][j];
//            }
//        }
//    }
//    return NULL;
//}

// HELPER STRUCTURE TO VERIFY LOCATIONS ENTERED BY USER
struct locationNode {
    std::string location;
    locationNode* next;

    locationNode() :location("") { next = NULL; }
    locationNode(std::string location) : location(location) { next = NULL; }
};

locationNode* locationsHead = NULL;
void addLocation(std::string location) {
    if (!locationsHead) {
        locationsHead = new locationNode(location);
        return;
    }

    locationNode* temp = locationsHead;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    locationNode* newNode = new locationNode(location);
    temp->next = newNode;
}
// helper function to verify whether user has entered the correct location
bool verifyLocation(std::string location) {
    if (!locationsHead) return false;

    locationNode* temp = locationsHead;
    while (temp->next != NULL) {
        if (temp->location == location) return true;
        temp = temp->next;
    }
    return false;
}

struct Edge {
    std::string destination;
    int weight;
};

struct AdjacencyList {
    std::string vertex;
    Edge* edges = nullptr;  // Initialize to nullptr
    int edgeCount = 0;      // Initialize edge count to 0
    int capacity = 0;       // Initialize capacity to 0
};

int graphSize = 0;         // Number of vertices in the graph
int graphCapacity = 2;     // Initial capacity of the adjacency list
AdjacencyList* graph = new AdjacencyList[graphCapacity];

// Function to add an edge to the adjacency list
void addEdge(AdjacencyList*& list, int& listSize, int& listCapacity,
    const std::string& vertex, const std::string& destination, int weight) {
    // Find the vertex in the adjacency list
    int index = -1;
    for (int i = 0; i < listSize; ++i) {
        if (list[i].vertex == vertex) {
            index = i;
            break;
        }
    }

    // If vertex is not found, add it
    if (index == -1) {
        if (listSize == listCapacity) {
            // Resize the adjacency list
            listCapacity *= 2;
            AdjacencyList* newList = new AdjacencyList[listCapacity];
            for (int i = 0; i < listSize; ++i) {
                newList[i] = list[i];
            }
            delete[] list;
            list = newList;
        }

        index = listSize++;
        list[index].vertex = vertex;
        list[index].edges = new Edge[2]; // Initial capacity for edges
        list[index].edgeCount = 0;
        list[index].capacity = 2;
    }

    // Add the edge to the vertex's edge list
    if (list[index].edgeCount == list[index].capacity) {
        // Resize the edges array
        list[index].capacity *= 2;
        Edge* newEdges = new Edge[list[index].capacity];
        for (int i = 0; i < list[index].edgeCount; ++i) {
            newEdges[i] = list[index].edges[i];
        }
        delete[] list[index].edges;
        list[index].edges = newEdges;
    }

    list[index].edges[list[index].edgeCount++] = { destination, weight };
}

// Function to read the CSV and populate the adjacency list
void readFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return;
    }

    std::string line;
    // Skip the first header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string location1, location2, weightStr;

        // extracting the items from the graph by parsing them using the ',' delimeter
        std::getline(ss, location1, ',');
        std::getline(ss, location2, ',');
        std::getline(ss, weightStr, ',');

        int weight = std::stoi(weightStr);

        if (!verifyLocation(location1)) addLocation(location1); // adding new location to the linked list if it doesnt already exist
        addEdge(graph, graphSize, graphCapacity, location1, location2, weight);
        addEdge(graph, graphSize, graphCapacity, location2, location1, weight); // beacuse graph is undirected, we will make the direction both ways
    }

    file.close();
}

// Printing all locations in Islamabad
void printLocations() {
    if (!locationsHead) {
        std::cout << YELLOW << "No locations to display." << RESET << std::endl;
        return;
    }

    std::cout << YELLOW << "List of Locations in Islamabad" << RESET << std::endl;
    std::cout << std::string(30, '-') << std::endl;

    locationNode* temp = locationsHead;
    while (temp != NULL) {
        std::cout << YELLOW << "Location: " << std::left << std::setw(20) << temp->location << RESET << std::endl;
        temp = temp->next;
    }
}


// deallocate memory
void freeMemory() {
    for (int i = 0; i < graphSize; ++i) {
        delete[] graph[i].edges;
    }
    delete[] graph;
}


void displayMenu1() {
    std::cout << "\n===== Welcome to the Restaurant Management System =====" << std::endl;
    std::cout << "1. Customer Signup" << std::endl;
    std::cout << "2. Customer Login" << std::endl;
    std::cout << "3. Employee Login" << std::endl;
    std::cout << "4. Admin Login" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

//helper function to take input and add promotion
void addPromotionInput(Promotion*& head) {
    std::string name;
    double discount;
    int limit;

    std::cin.ignore(); // Clear input buffer
    std::cout << "Enter promotion name: ";
    std::getline(std::cin, name);

    std::cout << "Enter discount percentage: ";
    std::cin >> discount;

    while (discount <= 0 || discount > 100) {
        std::cout << "Invalid discount! Enter a value between 0 and 100: ";
        std::cin >> discount;
    }

    std::cout << "Enter usage limit for this promotion: ";
    std::cin >> limit;

    while (limit <= 0) {
        std::cout << "Invalid limit! Enter a positive value: ";
        std::cin >> limit;
    }

    // Add promotion to the stack
    head = addPromotion(head, promotionIdCounter++, name, discount, limit);
}

void customerLoggedIn(Customer* loggedInCustomer, Restuarant *& root, Promotion* &promotionsHead) {
    int choice = -1;
    while (choice != 8) {
        std::cout << "\n===== Customer Menu =====" << std::endl;
        std::cout << "1. Edit Profile" << std::endl;
        std::cout << "2. Place Order" << std::endl;
        std::cout << "3. Leave Review" << std::endl;
        std::cout << "4. View Order History" << std::endl;
        std::cout << "5. View Current Orders and Status" << std::endl;
        std::cout << "6. View All Reviews You Have Left" << std::endl;
        std::cout << "7. View Profile" << std::endl;
        std::cout << "8. Return to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            loggedInCustomer->editCustomer();
            break;
        }
        case 2: {
            // show all restuarants to the customer
            // he will pick one, then show all the dishes in that restuarant
            // allow him to select one or go back
            // if he selects, then add that dish to the dishes linked list, which will be passed to the addOrder function
            // give him choice to order or let him add more dishes to the order
            // at the end give him choice to choose between the three types of order
            // then call addOrder function
            Restuarant* tempRes = root;
            std::cout << YELLOW << "------------------------------------------------------------------------------------------" << RESET << std::endl;
            std::cout << YELLOW << "| " << RESET << BLUE << std::left << std::setw(6) << "ID" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Name" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Type" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Manager" << YELLOW << "| " << RESET << std::endl;
            std::cout << YELLOW << "------------------------------------------------------------------------------------------" << RESET << std::endl;
            while (tempRes)
            {
                tempRes->displayTable();
                tempRes = tempRes->next;
            }
            int tempID = 0;
            std::cout << "Enter the restaurant ID: " << std::endl;
            std::cin >> tempID;
            while (tempID <= 9999 || tempID > 99999)                                    // ID must be of five digits
            {
                std::cout << RED << "ID must be of five digits" << RESET << std::endl;
                std::cin >> tempID;
            }
            tempRes = getRestuarant(root, tempID);
            std::cout << YELLOW << "---------------------------------------------------------------------------" << RESET << std::endl;
            std::cout << YELLOW << "| " << RESET << BLUE << std::left << std::setw(6) << "ID" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Name" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Type" << YELLOW << "| " << BLUE << std::left << std::setw(10) << "Price" << YELLOW << "| " << RESET << std::endl;
            std::cout << YELLOW << "---------------------------------------------------------------------------" << RESET << std::endl;
            tempRes->menu.displayMenuTable(tempRes->menu.root);
            DishLinkedListNode* head = NULL;
            int dishCount = 0;
            int type = 0;
            int choice2 = 0;
            std::cout << "Enter the type of order: (1 for premium, 2 for express & 3 for normal)" << std::endl;
            std::cin >> type;
            while (type < 1 || type > 3)
            {
                std::cout << RED << "Invalid Choice, Enter Again: " << RESET << std::endl;
                std::cin >> type;
            }
            std::cout << std::endl;
            std::string location;
            std::cout << "Enter delivery location in the format [I-11]: ";
            printLocations();
            std::cout << "Enter location: ";
            std::cin >> location;
            while (!verifyLocation(location)) {
                std::cout << RED << "Invalid location. Enter again: " << RESET << std::endl;
                std::cin >> location;
            }
            while (choice2 != 2)
            {
                std::cout << "1. Add Dish in order" << std::endl;
                std::cout << "2. Confirm Order" << std::endl;
                std::cin >> choice2;
                if (choice2 == 1)
                {
                    std::cout << "Enter the ID of the dish you want to add: " << std::endl;
                    std::cin >> tempID;
                    if (isDishInMenu(tempRes->menu, tempID))
                    {   
                        DishLinkedListNode* toAdd = new DishLinkedListNode;
                        toAdd->dish = tempRes->menu.findDish(tempID);
                        dishCount++;
                        
                        toAdd->next = head;
                        head = toAdd;
                        
                        std::cout << GREEN << "Dish Added in Order" << RESET << std::endl;
                    }
                    else
                    {
                        std::cout << RED << "Dish Not Found" << RESET << std::endl;
                    }
                }
                else if (choice2 == 2)
                {
                    addOrder(head, dishCount, type, loggedInCustomer, promotionsHead, tempRes, location);
                }
            }
            break;
        }
        case 3: {
            int reviewType = 0;
            std::cout << "1. Review a Restaurant" << std::endl;
            std::cout << "2. Review a Dish" << std::endl;
            std::cout << "Enter your choice: ";
            std::cin >> reviewType;

            // Validate review type input
            while (reviewType != 1 && reviewType != 2) {
                std::cout << RED << "Invalid input! Please enter 1 or 2: " << RESET;
                std::cin >> reviewType;
            }

            if (reviewType == 1) {
                // Review a restaurant
                Restuarant* tempRes = root;

                // Display all restaurants
                std::cout << YELLOW << "------------------------------------------------------------------------------------------" << RESET << std::endl;
                std::cout << YELLOW << "| " << RESET << BLUE << std::left << std::setw(6) << "ID" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Name" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Type" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Manager" << YELLOW << "| " << RESET << std::endl;
                std::cout << YELLOW << "------------------------------------------------------------------------------------------" << RESET << std::endl;

                while (tempRes) {
                    tempRes->displayTable();
                    tempRes = tempRes->next;
                }

                // Prompt for restaurant ID and validate
                int tempID = 0;
                std::cout << "Enter the restaurant ID you want to review: ";
                std::cin >> tempID;

                Restuarant* selectedRes = getRestuarant(root, tempID);
                while (!selectedRes) {
                    std::cout << RED << "Invalid restaurant ID! Please try again: " << RESET;
                    std::cin >> tempID;
                    selectedRes = getRestuarant(root, tempID);
                }

                // Collect and validate rating
                int rating = 0;
                std::cout << "Enter your rating (1-5): ";
                std::cin >> rating;
                while (rating < 1 || rating > 5) {
                    std::cout << RED << "Rating must be between 1 and 5. Please try again: " << RESET;
                    std::cin >> rating;
                }

                // Collect review description
                std::cin.ignore(); // Clear input buffer before getline
                std::string description;
                std::cout << "Enter your review description: ";
                std::getline(std::cin, description);

                // Add the review
                addReview(selectedRes, rating, description, loggedInCustomer);
                std::cout << GREEN << "Review added successfully!" << RESET << std::endl;

            }
            else if (reviewType == 2) {
                // Review a dish
                Restuarant* tempRes = root;

                // Prompt for restaurant ID to find the dish
                int tempID = 0;
                std::cout << "Enter the restaurant ID where the dish is located: ";
                std::cin >> tempID;

                Restuarant* selectedRes = getRestuarant(root, tempID);
                while (!selectedRes) {
                    std::cout << RED << "Invalid restaurant ID! Please try again: " << RESET;
                    std::cin >> tempID;
                    selectedRes = getRestuarant(root, tempID);
                }

                // Display the menu of the selected restaurant
                std::cout << YELLOW << "---------------------------------------------------------------------------" << RESET << std::endl;
                std::cout << YELLOW << "| " << RESET << BLUE << std::left << std::setw(6) << "ID" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Name" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Type" << YELLOW << "| " << BLUE << std::left << std::setw(10) << "Price" << YELLOW << "| " << RESET << std::endl;
                std::cout << YELLOW << "---------------------------------------------------------------------------" << RESET << std::endl;
                selectedRes->menu.displayMenuTable(selectedRes->menu.root);

                // Prompt for dish ID and validate
                std::cout << "Enter the dish ID you want to review: ";
                std::cin >> tempID;

                Dish* selectedDish = selectedRes->menu.findDish(tempID);
                while (!selectedDish) {
                    std::cout << RED << "Dish not found! Please try again: " << RESET;
                    std::cin >> tempID;
                    selectedDish = selectedRes->menu.findDish(tempID);
                }

                // Collect and validate rating
                int rating = 0;
                std::cout << "Enter your rating (1-5): ";
                std::cin >> rating;
                while (rating < 1 || rating > 5) {
                    std::cout << RED << "Rating must be between 1 and 5. Please try again: " << RESET;
                    std::cin >> rating;
                }

                // Collect review description
                std::cin.ignore(); // Clear input buffer before getline
                std::string description;
                std::cout << "Enter your review description: ";
                std::getline(std::cin, description);

                // Add the review
                addReview(selectedDish, rating, description, loggedInCustomer);
                std::cout << GREEN << "Review added successfully!" << RESET << std::endl;
            }

            break;
        }
        case 4: {
            std::cout << "\n===== Order History =====" << std::endl;

            Orders* currentOrder = processedOrders; // Global pointer to the head of processed orders
            bool hasOrders = false;

            // Traverse processed orders and match with the logged-in customer
            while (currentOrder) {
                if (currentOrder->customer == loggedInCustomer) {
                    hasOrders = true;
                    std::cout << "Order ID: " << currentOrder->orderId
                        << ", Total: Rs. " << currentOrder->total
                        << ", Type: " << (currentOrder->type == 1 ? "Premium"
                            : currentOrder->type == 2 ? "Express"
                            : "Normal")
                        << std::endl;
                }
                currentOrder = currentOrder->next;
            }

            if (!hasOrders) {
                std::cout << "You have not placed any orders yet." << std::endl;
            }

            break;
        }
        case 5: {
            std::cout << "\n===== Current Orders and Their Status =====" << std::endl;

            bool hasOrders = false;
            // Display processed orders
            std::cout << "\nProcessed Orders:" << std::endl;
            Orders* current = processedOrders; // Global pointer to the head of processed orders
            while (current) {
                if (current->customer == loggedInCustomer) {
                    hasOrders = true;
                    std::cout << YELLOW << "Order ID: " << RESET << current->orderId
                        << YELLOW << ", Total: Rs. " << RESET << current->total
                        << YELLOW << ", Type: " << RESET << (current->type == 1 ? "Premium" : (current->type == 2 ? "Express" : "Normal")) << std::endl;
                    DishLinkedListNode* tempDish = current->head;
                    while (tempDish)
                    {
                        std::cout << YELLOW << "Name : " << RESET << tempDish->dish->name << std::endl;
                        std::cout << YELLOW << "Price : " << RESET << tempDish->dish->price << std::endl;
                        std::cout << std::endl;
                        tempDish = tempDish->next;
                    }
                    std::cout << "=================================================" << std::endl;
                }
                current = current->next;
            }
            // Display unprocessed orders
            std::cout << "\nUnprocessed Orders:" << std::endl;
            current = orderQueueHead; // Global pointer to the head of unprocessed orders
            while (current) {
                DishLinkedListNode* tempDish = current->head;
                current->total = 0;
                while (tempDish)
                {
                    current->total += tempDish->dish->price;
                    tempDish = tempDish->next;
                }

                while (promotionsHead && promotionsHead->limit == promotionsHead->numPromotionsApplied) {
                    // Pop promotion if its limit is reached
                    promotionsHead = checkLimit(promotionsHead);
                }

                if (promotionsHead) {
                    // Apply the current promotion
                    double discountAmount = (promotionsHead->discount / 100.0) * current->total;
                    current->total -= discountAmount;

                    // Update the promotion usage count
                    promotionsHead->numPromotionsApplied++;

                    std::cout << "Promotion applied: " << promotionsHead->name
                        << ", Discount: " << promotionsHead->discount << "%, Amount Saved: Rs. " << discountAmount << std::endl;
                }

                if (current->type == 1)                     // premium
                {
                    current->total += 500; //premium order fee
                }
                else if (current->type == 2) { // express
                    current->total += 200;   // delivery charges
                    current->total *= 1.10;  // express order fee
                }
                else if (current->type == 3) {  // normal
                    current->total += 100; // delivery charges
                }

                if (current->customer == loggedInCustomer) {
                    hasOrders = true;
                    std::cout << YELLOW << "Order ID: " << RESET << current->orderId
                        << YELLOW << ", Total: Rs. " << RESET << current->total
                        << YELLOW << ", Type: " << RESET << (current->type == 1 ? "Premium" : (current->type == 2 ? "Express" : "Normal")) << std::endl;
                    DishLinkedListNode* tempDish = current->head;
                    while (tempDish)
                    {
                        std::cout << YELLOW << "Name : " << RESET << tempDish->dish->name << std::endl;
                        std::cout << YELLOW << "Price : " << RESET << tempDish->dish->price << std::endl;
                        std::cout << std::endl;
                        tempDish = tempDish->next;
                    }
                    std::cout << "=================================================" <<  std::endl;
                }
                current = current->next;
            }

            if (!hasOrders) {
                std::cout << "You currently have no orders." << std::endl;
            }

            break;
        }
        case 6: {
            std::cout << "\n===== All Reviews Left by You =====" << std::endl;

            bool hasReviews = false;

            // Check good reviews heap
            std::cout << "\nGood Reviews:" << std::endl;
            for (int i = 0; i < goodReviewsCount; ++i) {
                if (goodReviewsHeap[i]->reviewRestuarant && goodReviewsHeap[i]->reviewCustomer == loggedInCustomer) {
                    hasReviews = true;
                    std::cout << "Review ID: " << goodReviewsHeap[i]->reviewId
                        << ", Rating: " << goodReviewsHeap[i]->rating << " stars"
                        << ", Description: " << goodReviewsHeap[i]->description
                        << ", Restaurant: " << goodReviewsHeap[i]->reviewRestuarant->name
                        << std::endl;
                }
                else if (goodReviewsHeap[i]->reviewDish && goodReviewsHeap[i]->reviewCustomer == loggedInCustomer) {
                    hasReviews = true;
                    std::cout << "Review ID: " << goodReviewsHeap[i]->reviewId
                        << ", Rating: " << goodReviewsHeap[i]->rating << " stars"
                        << ", Description: " << goodReviewsHeap[i]->description
                        << ", Dish: " << goodReviewsHeap[i]->reviewDish->name
                        << std::endl;
                }
            }

            // Check bad reviews heap
            std::cout << "\nBad Reviews:" << std::endl;
            for (int i = 0; i < badReviewsCount; ++i) {
                if (badReviewsHeap[i]->reviewRestuarant && badReviewsHeap[i]->reviewCustomer == loggedInCustomer) {
                    hasReviews = true;
                    std::cout << "Review ID: " << badReviewsHeap[i]->reviewId
                        << ", Rating: " << badReviewsHeap[i]->rating << " stars"
                        << ", Description: " << badReviewsHeap[i]->description
                        << ", Restaurant: " << badReviewsHeap[i]->reviewRestuarant->name
                        << std::endl;
                }
                else if (badReviewsHeap[i]->reviewDish && badReviewsHeap[i]->reviewCustomer == loggedInCustomer) {
                    hasReviews = true;
                    std::cout << "Review ID: " << badReviewsHeap[i]->reviewId
                        << ", Rating: " << badReviewsHeap[i]->rating << " stars"
                        << ", Description: " << badReviewsHeap[i]->description
                        << ", Dish: " << badReviewsHeap[i]->reviewDish->name
                        << std::endl;
                }
            }

            if (!hasReviews) {
                std::cout << "You have not left any reviews yet." << std::endl;
            }

            break;
        }
        case 7: {
            std::cout << std::endl;
            loggedInCustomer->displayCustomer();
            std::cout << std::endl;
            break;
        }
        case 8:{
            std::cout << "Returning to main menu..." << std::endl;
            break;
        }
        default: {
            std::cout << "Invalid choice! Please enter a valid option (1-7)." << std::endl;
            break;
        }
        }
    }
}

std::string* dijkstra(AdjacencyList* graph, int graphSize, const std::string& start, const std::string& destination, int& pathLength) {
    // Distance map to keep track of shortest distance to each vertex
    std::unordered_map<std::string, int> distances;
    // Predecessor map to reconstruct the shortest path
    std::unordered_map<std::string, std::string> predecessors;
    // Visited map to track whether a vertex has been processed
    std::unordered_map<std::string, bool> visited;

    // Initialize distances to infinity and visited to false
    for (int i = 0; i < graphSize; ++i) {
        distances[graph[i].vertex] = INT_MAX;
        visited[graph[i].vertex] = false;
    }
    distances[start] = 0; // Distance to the start vertex is 0

    // Dijkstra's algorithm without a priority queue
    for (int count = 0; count < graphSize; ++count) {
        // Find the unvisited vertex with the smallest distance
        std::string currentVertex;
        int smallestDistance = INT_MAX;
        for (int i = 0; i < graphSize; ++i) {
            if (!visited[graph[i].vertex] && distances[graph[i].vertex] < smallestDistance) {
                smallestDistance = distances[graph[i].vertex];
                currentVertex = graph[i].vertex;
            }
        }

        // If no valid vertex is found, break (graph is disconnected)
        if (smallestDistance == INT_MAX) break;

        // Mark the current vertex as visited
        visited[currentVertex] = true;

        // Update distances for neighbors of the current vertex
        for (int i = 0; i < graphSize; ++i) {
            if (graph[i].vertex == currentVertex) {
                for (int j = 0; j < graph[i].edgeCount; ++j) {
                    Edge& edge = graph[i].edges[j];
                    int newDistance = distances[currentVertex] + edge.weight;

                    // Update the distance if a shorter path is found
                    if (newDistance < distances[edge.destination]) {
                        distances[edge.destination] = newDistance;
                        predecessors[edge.destination] = currentVertex;
                    }
                }
                break;
            }
        }
    }

    // Reconstruct the shortest path using the predecessor map
    pathLength = 0;
    std::string at = destination;

    // Dynamically allocate an array for the path
    std::string* path = new std::string[graphSize];

    while (!at.empty() && distances[at] != INT_MAX) {
        path[pathLength++] = at;
        at = predecessors[at];
    }

    // Reverse the array manually
    for (int i = 0; i < pathLength / 2; ++i) {
        std::swap(path[i], path[pathLength - i - 1]);
    }

    // If the path does not start with the source, no valid path exists
    if (pathLength == 0 || path[0] != start) {
        delete[] path;
        std::cout << "No path exists between " << start << " and " << destination << "." << std::endl;
        return nullptr;
    }

    return path;
}

void employeeLoggedIn(Employee* loggedInEmployee, Restuarant*& root) {
    int choice = -1;

    while (choice != 8) {
        std::cout << "\n===== Employee Menu =====" << std::endl;
        std::cout << "1. Edit Profile" << std::endl;
        std::cout << "2. View Orders and Reviews for Your Restaurant" << std::endl;
        std::cout << "3. Process Orders" << std::endl;
        std::cout << "4. View Processed Order History" << std::endl;
        std::cout << "5. View Current Orders and Status" << std::endl;
        std::cout << "6. View Profile" << std::endl;
        std::cout << "7. Deliver Orders" << std::endl;
        std::cout << "8. Return to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            // Edit Profile
            int subChoice;
            std::cout << "\nEdit Profile:\n1. Edit Name\n2. Reset Password\nEnter your choice: ";
            std::cin >> subChoice;

            if (subChoice == 1) {
                // Edit Name
                std::cout << "Enter new name: ";
                std::cin.ignore(); 
                std::getline(std::cin, loggedInEmployee->name);
                std::cout << "Name updated successfully!" << std::endl;
            }
            else if (subChoice == 2) {
                // Reset Password
                std::string currentPassword, newPassword;
                std::cout << "Enter your current password: ";
                char ch;
                while ((ch = _getch()) != '\r') { 
                    if (ch == '\b') { 
                        if (!currentPassword.empty()) {
                            currentPassword.pop_back();
                            std::cout << "\b \b";
                        }
                    }
                    else {
                        currentPassword += ch;
                        std::cout << '*';
                    }
                }
                std::cout << std::endl;

                if (currentPassword == loggedInEmployee->password) {
                    std::cout << "Enter new password: ";
                    std::cin >> newPassword;
                    while (!verifyPassword(newPassword)) {
                        std::cout << "Invalid password! It must contain at least 1 capital letter, "
                            "1 lowercase letter, 1 digit, and 1 special character, and be between 8-12 characters long."
                            << std::endl;
                        std::cout << "Enter new password: ";
                        std::cin >> newPassword;
                    }
                    loggedInEmployee->password = newPassword;
                    std::cout << "Password updated successfully!" << std::endl;
                }
                else {
                    std::cout << "Incorrect current password. Password reset failed." << std::endl;
                }
            }
            else {
                std::cout << "Invalid choice for Edit Profile." << std::endl;
            }
            break;
        }
        case 2: {
            // Ensure the employee is assigned to a restaurant
            if (!loggedInEmployee->employeeRst) {
                std::cout << "You are not assigned to any restaurant. Please contact the admin." << std::endl;
                break;
            }

            // Display orders processed by this employee
            std::cout << "\n===== Orders Processed by You =====" << std::endl;
            Orders* currentOrder = processedOrders; 
            bool hasOrders = false;

            while (currentOrder) {
                if (currentOrder->waiter == loggedInEmployee) {
                    hasOrders = true;
                    std::cout << "Order ID: " << currentOrder->orderId
                        << ", Total: Rs. " << currentOrder->total
                        << ", Type: " << (currentOrder->type == 1 ? "Premium"
                            : currentOrder->type == 2 ? "Express"
                            : "Normal")
                        << std::endl;
                }
                currentOrder = currentOrder->next;
            }

            if (!hasOrders) {
                std::cout << "You have not processed any orders yet." << std::endl;
            }

            // Display reviews for the employee's restaurant
            std::cout << "\n===== Reviews for Your Restaurant =====" << std::endl;

            // Check good reviews heap
            bool hasReviews = false;
            for (int i = 0; i < goodReviewsCount; ++i) {
                if (goodReviewsHeap[i]->reviewRestuarant == loggedInEmployee->employeeRst) {
                    hasReviews = true;
                    std::cout << "Review ID: " << goodReviewsHeap[i]->reviewId
                        << ", Rating: " << goodReviewsHeap[i]->rating << " stars"
                        << ", Description: " << goodReviewsHeap[i]->description
                        << std::endl;
                }
            }

            // Check bad reviews heap
            for (int i = 0; i < badReviewsCount; ++i) {
                if (badReviewsHeap[i]->reviewRestuarant == loggedInEmployee->employeeRst) {
                    hasReviews = true;
                    std::cout << "Review ID: " << badReviewsHeap[i]->reviewId
                        << ", Rating: " << badReviewsHeap[i]->rating << " stars"
                        << ", Description: " << badReviewsHeap[i]->description
                        << std::endl;
                }
            }

            if (!hasReviews) {
                std::cout << "No reviews have been left for your restaurant yet." << std::endl;
            }

            break;
        }
        case 3: {
            processOrder(loggedInEmployee, processedOrders);
            break;
        }
        case 4: {
            // Ensure the employee is assigned to a restaurant
            if (!loggedInEmployee->employeeRst) {
                std::cout << "You are not assigned to any restaurant. Please contact the admin." << std::endl;
                break;
            }

            std::cout << "\n===== Processed Order History =====" << std::endl;
            Orders* currentOrder = processedOrders; 
            bool hasProcessedOrders = false;

            while (currentOrder) {
                // Check if this order was processed by the logged-in employee
                if (currentOrder->waiter == loggedInEmployee) {
                    hasProcessedOrders = true;
                    std::cout << "Order ID: " << currentOrder->orderId
                        << ", Total: Rs. " << currentOrder->total
                        << ", Type: " << (currentOrder->type == 1 ? "Premium"
                            : currentOrder->type == 2 ? "Express"
                            : "Normal")
                        << std::endl;
                }
                currentOrder = currentOrder->next;
            }

            if (!hasProcessedOrders) {
                std::cout << "You have not processed any orders yet." << std::endl;
            }
            break;
        }
        case 5: {
            // Ensure the employee is assigned to a restaurant
            if (!loggedInEmployee->employeeRst) {
                std::cout << "You are not assigned to any restaurant. Please contact the admin." << std::endl;
                break;
            }

            std::cout << "\n===== Current Unprocessed Orders =====" << std::endl;

            bool hasOrders = false;

            // Display unprocessed orders from the orderQueueHead
            Orders* currentOrder = orderQueueHead; // Global pointer to the head of unprocessed orders
            while (currentOrder) {
                // Match the order's customer restaurant with the employee's assigned restaurant
                if (loggedInEmployee->employeeRst)
                {
                    if (currentOrder->rest == loggedInEmployee->employeeRst) {
                        hasOrders = true;
                        std::cout << "Order ID: " << currentOrder->orderId
                            << ", Total: Rs. " << currentOrder->total
                            << ", Type: " << (currentOrder->type == 1 ? "Premium"
                                : currentOrder->type == 2 ? "Express"
                                : "Normal")
                            << ", Status: Pending" << std::endl;
                    }
                }
                else
                {
                    std::cout << RED << "You are not employed at any restuarant currently" << RESET << std::endl;
                }
                currentOrder = currentOrder->next;
            }
            if (!hasOrders) {
                std::cout << "There are no current unprocessed orders for your restaurant." << std::endl;
            }
            break;
        }
        case 6: {
            std::cout << std::endl;
            loggedInEmployee->displayEmployee();
            std::cout << std::endl;
            break;
        }
        case 7: {
            if (!loggedInEmployee->employeeRst) {
                std::cout << RED << "You are not assigned to any restaurant. Please contact the admin." << RESET << std::endl;
                break;
            }

            if (!processedOrders) {
                std::cout << RED << "There are no processed orders to deliver." << RESET << std::endl;
                break;
            }

            std::cout << "\n===== Delivering Orders =====" << std::endl;

            Orders* ordersToDeliver = nullptr;
            Orders* lastInList = nullptr;

            Orders* currentOrder = processedOrders;
            Orders* prevOrder = nullptr;

            while (currentOrder) {
                if (currentOrder->waiter == loggedInEmployee && currentOrder->deliveryDriver == loggedInEmployee) {
                    // Remove order from processedOrders and move to ordersToDeliver
                    Orders* toDeliver = currentOrder;

                    if (prevOrder) {
                        prevOrder->next = currentOrder->next;
                    }
                    else {
                        processedOrders = currentOrder->next; // Update head if it's the first order
                    }

                    currentOrder = currentOrder->next; // Move forward in the processedOrders list

                    // Insert into ordersToDeliver based on total (delivery priority by weight)
                    toDeliver->next = nullptr;
                    if (!ordersToDeliver || toDeliver->total < ordersToDeliver->total) {
                        toDeliver->next = ordersToDeliver;
                        ordersToDeliver = toDeliver;
                    }
                    else {
                        Orders* temp = ordersToDeliver;
                        while (temp->next && temp->next->total <= toDeliver->total) {
                            temp = temp->next;
                        }
                        toDeliver->next = temp->next;
                        temp->next = toDeliver;
                    }
                }
                else {
                    prevOrder = currentOrder;
                    currentOrder = currentOrder->next;
                }
            }

            if (!ordersToDeliver) {
                std::cout << RED << "You have no processed orders to deliver." << RESET << std::endl;
                break;
            }

            // Deliver orders based on shortest path
            Orders* tempOrder = ordersToDeliver;
            std::string startLocation = loggedInEmployee->employeeRst->restaurantLocation;
            std::string deliveryPath = startLocation;

            while (tempOrder) {
                int pathLength = 0;

                // Find the shortest path using Dijkstra's algorithm
                std::string* path = dijkstra(graph, graphSize, startLocation, tempOrder->destinationLocation, pathLength);

                if (!path) {
                    std::cout << RED << "Could not find a path to deliver Order ID: " << tempOrder->orderId << RESET << std::endl;
                }
                else {
                    // Update the delivery path
                    for (int i = 1; i < pathLength; ++i) {
                        deliveryPath += " -> " + path[i];
                    }

                    // Update order status
                    std::cout << GREEN << "Delivered Order ID: " << tempOrder->orderId
                        << ", Path: " << deliveryPath << RESET << std::endl;

                    // Free path memory
                    delete[] path;
                }

                startLocation = tempOrder->destinationLocation;
                Orders* deliveredOrder = tempOrder;
                tempOrder = tempOrder->next;
                delete deliveredOrder; // Remove from delivery list after delivery
            }

            std::cout << GREEN << "All orders delivered successfully!" << RESET << std::endl;
            break;
        }
        case 8: {
            std::cout << "Returning to main menu..." << std::endl;
            break;
        }
        default: {
            std::cout << "Invalid choice! Please enter a valid option (1-6)." << std::endl;
            break;
        }
        }
    }
}

bool loginAdmin(std::string Aemail, std::string Apassword) {
    std::string email;
    std::cout << "Enter email: ";
    std::cin >> email;

    std::string password;
    char ch;
    std::cout << "Enter password: ";
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else {
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    if (email != Aemail || password != Apassword)
    {
        std::cout << RED << "Incorrect Email or Password!" << RESET << std::endl;
        return false;
    }
    
    std::cout << GREEN << "You are now logged in as Admin" << RESET << std::endl << std::endl;
    return true;
}
void adminLoggedIn(Restuarant *&root, Promotion*& promotionsHead) {
    int choice = -1;
    while (choice != 10)
    {
        std::cout << "\n===== Admin Menu =====" << std::endl;
        std::cout << "1. Add Restaurant" << std::endl;
        std::cout << "2. Remove Restaurant" << std::endl;
        std::cout << "3. Edit Restaurant" << std::endl;
        std::cout << "4. View All Restaurants" << std::endl;
        std::cout << "5. Add employee in Restaurant" << std::endl;
        std::cout << "6. Add dish in menu of a restaurant" << std::endl;
        std::cout << "7. Edit dish in menu of a restaurant" << std::endl;
        std::cout << "8. Remove dish from menu of a restaurant" << std::endl;
        std::cout << "9. Add promotion" << std::endl;
        std::cout << "10. Return to the main menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string name = "\0", type = "\0", restaurantLocation = "\0";
            std::cout << "Enter the name of the restaurant: " << std::endl;             // input restaurant name
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Enter the type of the restaurant: " << std::endl;
            std::getline(std::cin, type); // input restaurant type
            std::cout << "Enter the location of the restaurant: " << std::endl;
            std::cin >> restaurantLocation;
            while (!verifyLocation(restaurantLocation)) {
                std::cout << RED << "Invalid Location. Enter a valid location: " << RESET;
                std::cin >> restaurantLocation;
            }
            
            Restuarant* newRestaurant = new Restuarant(restuarantIdCounter, name, type, NULL, name + " Menu", restaurantLocation);
            std::cout << "Enter the Manager Details: " << std::endl;
            addEmployee(newRestaurant);
            Employee* manager = findEmployee(employeeIdCounter - 1);
            newRestaurant->manager = manager;
            root = addRestuarant(root, newRestaurant);                                  // creating new restaurant variable
            restuarantIdCounter++;                                                      // incrementing restaurant id counter
            break;
        }
        case 2: {
            int tempID = 0;
            std::cout << "Enter the ID of the restaurant to remove: " << std::endl;     // input ID of restaurant to remove
            std::cin >> tempID;
            while (tempID <= 9999 || tempID > 99999)                                    // ID must be of five digits
            {
                std::cout << RED << "ID must be of five digits" << RESET << std::endl;
                std::cin >> tempID;
            }
            root = removeRestuarant(root, tempID);
            break;
        }
        case 3: {
            editRestuarant(root);
            break;
        }
        case 4: {
            Restuarant* temp = root;
            std::cout << YELLOW << "------------------------------------------------------------------------------------------" << RESET << std::endl;
            std::cout << YELLOW << "| " << RESET << BLUE << std::left << std::setw(6) << "ID" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Name" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Type" << YELLOW << "| " << BLUE << std::left << std::setw(25) << "Manager" << YELLOW << "| " << RESET << std::endl;
            std::cout << YELLOW << "------------------------------------------------------------------------------------------" << RESET << std::endl;
            while (temp)
            {
                temp->displayTable();
                temp = temp->next;
            }
            break;
        }
        case 5: {
            bool restaurantCheck = false;
            int tempID = 0;
            std::cout << "Enter the restaurant ID: " << std::endl;
            std::cin >> tempID;
            Restuarant* temp = root;
            while (temp != NULL)
            {
                if (temp->restuarantId == tempID)
                {
                    addEmployee(temp);
                    restaurantCheck = true;
                    break;
                }
                temp = temp->next;
            }
            if (!restaurantCheck)
            {
                std::cout << RED << "Restaurant with this ID was not found" << RESET << std::endl;
            }
            break;
        }
        case 6: {
            bool restaurantCheck = false;
            std::string dishName = "\0", dishType = "\0";
            float dishPrice = 0;
            int tempID = 0;
            std::cout << "Enter the restaurant ID: " << std::endl;
            std::cin >> tempID;
            Restuarant* temp = root;
            while (temp != NULL)
            {
                if (temp->restuarantId == tempID)
                {
                    std::cout << "Enter the name of the dish: " << std::endl;
                    std::cin.ignore();
                    std::getline(std::cin, dishName);
                    std::cout << "Enter the type of the dish: " << std::endl;
                    std::getline(std::cin, dishType);
                    std::cout << "Enter the price of the dish: " << std::endl;
                    std::cin >> dishPrice;
                    while (dishPrice < 0)
                    {
                        std::cout << RED << "Price cannot be negative. Enter Again: " << RESET << std::endl;
                        std::cin >> dishPrice;
                    }
                    temp->menu.addDish(dishIdCounter++, dishName, dishType, dishPrice);
                    restaurantCheck = true;
                    break;
                }
                temp = temp->next;
            }
            if (!restaurantCheck)
            {
                std::cout << RED << "Restaurant with this ID was not found" << RESET << std::endl;
            }
            break;
        }
        case 7: {
            bool restaurantCheck = false;
            int tempID = 0;
            int dishID = 0;
            std::cout << "Enter the restaurant ID: " << std::endl;
            std::cin >> tempID;
            Restuarant* temp = root;
            std::cout << "Enter the Dish ID to edit: " << std::endl;
            std::cin >> dishID;
            while (temp != NULL)
            {
                if (temp->restuarantId == tempID)
                {
                    temp->menu.editDish(dishID);
                    restaurantCheck = true;
                    break;
                }
                temp = temp->next;
            }
            if (!restaurantCheck)
            {
                std::cout << RED << "Restaurant with this ID was not found" << RESET << std::endl;
            }
            break;
        }
        case 8: {
            bool restaurantCheck = false;
            int tempID = 0;
            int dishID = 0;
            std::cout << "Enter the restaurant ID: " << std::endl;
            std::cin >> tempID;
            Restuarant* temp = root;
            std::cout << "Enter the Dish ID to delete: " << std::endl;
            std::cin >> dishID;
            while (temp != NULL)
            {
                if (temp->restuarantId == tempID)
                {
                    temp->menu.removeDish(dishID);
                    restaurantCheck = true;
                    break;
                }
                temp = temp->next;
            }
            if (!restaurantCheck)
            {
                std::cout << RED << "Restaurant with this ID was not found" << RESET << std::endl;
            }
            break;
        }
        case 9: {
            addPromotionInput(promotionsHead);
            break;
        }
        case 10: {
            std::cout << "Returning to main menu..." << std::endl;
            break;
        }
        default: {
            std::cout << "Invalid choice! Please enter a valid option (1-9)." << std::endl;
            break;
        }
        }
    }
}

int main() {
    std::string filename = "Graph_of_Islamabad.csv";
    readFile(filename);

    Restuarant* root = NULL;
    std::string adminEmail = "admin@gmail.com";
    std::string adminPassword = "Admin@12";
    // adding two customers
    Customer* cust1 = new Customer(customerIdCounter++, "Maaz Abid", "maaz@gmail.com", "Maaz@123");
    int key = getHashKeyCustomer(cust1->customerId);
    customerHashmap[key][0] = cust1;
    Customer* cust2 = new Customer(customerIdCounter++, "Muhammad Ahmed", "ahmed@gmail.com", "Ahmed@12");
    key = getHashKeyCustomer(cust2->customerId);
    customerHashmap[key][0] = cust2;

    // adding restaurant 1 
    Employee* man1 = new Employee(employeeIdCounter++, "Ahnaf Abdullah", "ahnaf@gmail.com", "Ahnaf@12", NULL);    // manager
    Restuarant * res1 = new Restuarant(restuarantIdCounter++, "Cheezious", "Fast Food", man1, "Cheezious Menu", "F-12");                  // restaurant
    man1->employeeRst = res1;                                                                                     // assigning the restaurant to the manager
    res1->menu.addDish(dishIdCounter++, "Zinger Burger", "Burger", 400);                                        // adding dishes in menu
    res1->menu.addDish(dishIdCounter++, "Crown Crust", "Pizza", 900);
    res1->menu.addDish(dishIdCounter++, "Chicken Tikka", "Pizza", 690);
    res1->menu.addDish(dishIdCounter++, "French Fries", "Snack", 200);
    Employee* emp1 = new Employee(employeeIdCounter++, "Moeez Abid", "moeez@gmail.com", "Moeez@12", res1);          // adding employees
    Employee* emp2 = new Employee(employeeIdCounter++, "Inam Ullah", "inam@gmail.com", "Inam@123", res1);
    Employee* emp3 = new Employee(employeeIdCounter++, "Abdul basit", "basit@gmail.com", "Basit@12", res1);
    key = getHashKey(emp1->employeeId);                             // adding employee to the hashmap table
    employeeHashmap[key][0] = emp1;
    key = getHashKey(man1->employeeId);
    employeeHashmap[key][0] = man1;
    key = getHashKey(emp2->employeeId);                             // adding employee to the hashmap table
    employeeHashmap[key][0] = emp2;
    key = getHashKey(emp3->employeeId);                             // adding employee to the hashmap table
    employeeHashmap[key][0] = emp3;
    root = addRestuarant(root, res1);                                   // adding restaurant 1 to the linked list                             
    // restaurant 2
    Employee* man2 = new Employee(employeeIdCounter++, "Noor Alam", "noor@gmail.com", "Noor@123", NULL);            // manager
    Restuarant* res2 = new Restuarant(restuarantIdCounter++, "Gloria Jean's Coffees", "Cafe", man2, "Gloria Jean's menu", "G-12");                // restaurant
    man2->employeeRst = res2;                                                                                       // assigning the restaurant to the manager
    res2->menu.addDish(dishIdCounter++, "Cappuccino", "Coffee", 750);                                               // adding dishes in menu
    res2->menu.addDish(dishIdCounter++, "Chai Tea Latte", "Tea", 850);
    res2->menu.addDish(dishIdCounter++, "Cocoa Loco", "Chillers", 950);
    Employee* emp4 = new Employee(employeeIdCounter++, "Zaki haider", "zaki@gmail.com", "Zaki@123", res2);
    Employee* emp5 = new Employee(employeeIdCounter++, "Hashir", "hashir@gmail.com", "Hashir@1", res2);
    Employee* emp6 = new Employee(employeeIdCounter++, "Usman Gohar", "usman@gmail.com", "Usman@12", res2);
    key = getHashKey(man2->employeeId);
    employeeHashmap[key][0] = man2;
    key = getHashKey(emp4->employeeId);                             // adding employee to the hashmap table
    employeeHashmap[key][0] = emp4;
    key = getHashKey(emp5->employeeId);                             // adding employee to the hashmap table
    employeeHashmap[key][0] = emp5;
    key = getHashKey(emp6->employeeId);                             // adding employee to the hashmap table
    employeeHashmap[key][0] = emp6;
    root = addRestuarant(root, res2);                                   // adding restaurant 2 to the linked list
    /// restaurant 3
    Employee* man3 = new Employee(employeeIdCounter++, "Ijtaba Rizvi", "ijtaba@gmail.com", "Ijtaba@1", NULL);            // manager
    Restuarant* res3 = new Restuarant(restuarantIdCounter++, "Hot N Spicy", "Desi & Fast Food", man3, "Hot N Spicy menu", "H-8");                // restaurant
    man3->employeeRst = res3;                                                                                       // assigning the restaurant to the manager
    res3->menu.addDish(dishIdCounter++, "Chicken Tikka piece", "BBQ", 200);                                            // adding dishes in menu
    res3->menu.addDish(dishIdCounter++, "Chicken Malai Boti", "BBQ", 250);
    res3->menu.addDish(dishIdCounter++, "Zinger Burger", "Burger", 280);
    res3->menu.addDish(dishIdCounter++, "Chicken Karahi", "Desi", 500);
    Employee* emp7 = new Employee(employeeIdCounter++, "Huzaifa", "huzaifa@gmail.com", "Huzaifa@1", root->next->next);
    Employee* emp8 = new Employee(employeeIdCounter++, "Zain", "zain@gmail.com", "Zain@123", root->next->next);
    Employee* emp9 = new Employee(employeeIdCounter++, "Qasim", "qasim@gmail.com", "Qasim@12", root->next->next);
    key = getHashKey(man3->employeeId);
    employeeHashmap[key][0] = man3;
    key = getHashKey(emp7->employeeId);                             // adding employee to the hashmap table
    employeeHashmap[key][0] = emp7; 
    key = getHashKey(emp8->employeeId);                             // adding employee to the hashmap table
    employeeHashmap[key][1] = emp8;
    key = getHashKey(emp9->employeeId);                             // adding employee to the hashmap table
    employeeHashmap[key][1] = emp9;
    root = addRestuarant(root, res3);                                   // adding restaurant 3 to the linked list

    // promotion head(stack implemented using linkedlist)    
    Promotion* promotionsHead = new Promotion(promotionIdCounter++,"FirstPromo", 20, 18);

    int choice1 = -1;

    while (choice1 != 0) {
        displayMenu1();
        std::cin >> choice1;

        switch (choice1) {
        case 1:
            addCustomer();
            break;

        case 2: {
            Customer* loggedInCustomer = loginCustomer();
            if (loggedInCustomer) {
                std::cout << "Welcome, " << loggedInCustomer->name << "!" << std::endl;
                customerLoggedIn(loggedInCustomer, root, promotionsHead);
            }
            else {
                std::cout << "Customer login failed!" << std::endl;
            }
            break;
        }
        case 3: {
            Employee* loggedInEmployee = loginEmployee();
            if (loggedInEmployee) {
                std::cout << "Welcome, " << loggedInEmployee->name << "!" << std::endl;
                employeeLoggedIn(loggedInEmployee, root);
            }
            else {
                std::cout << "Employee login failed!" << std::endl;
            }
            break;
        }

        case 4:
            if (loginAdmin(adminEmail, adminPassword))
            {
                std::cout << "Welcome, Admin!" << std::endl;
                adminLoggedIn(root, promotionsHead);
            }\

            break;

        case 0:
            std::cout << "Exiting the system. Goodbye!" << std::endl;
            break;

        default:
            std::cout << "Invalid choice! Please enter a valid option (0-4)." << std::endl;
            break;
        }
    }

    freeMemory();

    return 0;
}
