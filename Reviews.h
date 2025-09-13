#pragma once
#include <iostream>
#include <string>
#include "Restuarant.h"
#include "Customer.h"
#include "Dish.h"

extern int reviewIdCounter;

struct Reviews {
    int reviewId;
    int rating; // 1-5
    std::string description;
    Restuarant* reviewRestuarant;
    Dish* reviewDish;
    Customer* reviewCustomer;

    Reviews(int id, int rating, std::string description, Restuarant* restuarant, Customer* reviewCustomer);
    Reviews(int id, int rating, std::string description, Dish* dish, Customer* reviewCustomer);
};

// Global heaps for reviews
extern Reviews** goodReviewsHeap;
extern int goodReviewsCount;
extern int goodReviewsCapacity;

extern Reviews** badReviewsHeap;
extern int badReviewsCount;
extern int badReviewsCapacity;

// Functions to manage reviews
void addReview(Restuarant* restuarant, int rating, const std::string& description, Customer* reviewCustomer);
void addReview(Dish* dish, int rating, const std::string& description, Customer* reviewCustomer);
void viewTopReviews(bool good);
void expandHeap(Reviews*** heap, int& capacity);
void heapifyUp(Reviews** heap, int count);
void heapifyDown(Reviews** heap, int count, int index);
