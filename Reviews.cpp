#include "Reviews.h"

// Global variables
int reviewIdCounter = 10000;

// Good and bad reviews heaps
Reviews** goodReviewsHeap = nullptr;
int goodReviewsCount = 0;
int goodReviewsCapacity = 0;

Reviews** badReviewsHeap = nullptr;
int badReviewsCount = 0;
int badReviewsCapacity = 0;

// Constructor implementations
Reviews::Reviews(int id, int rating, std::string description, Restuarant* restuarant, Customer* reviewCustomer)
    : reviewId(id), rating(rating), description(description), reviewRestuarant(restuarant), reviewDish(nullptr), reviewCustomer(reviewCustomer) {}

Reviews::Reviews(int id, int rating, std::string description, Dish* dish, Customer* reviewCustomer)
    : reviewId(id), rating(rating), description(description), reviewRestuarant(nullptr), reviewDish(dish), reviewCustomer(reviewCustomer) {}

// Add review to the heap
void addReview(Restuarant* restuarant, int rating, const std::string& description, Customer* reviewCustomer) {
    Reviews* newReview = new Reviews(reviewIdCounter++, rating, description, restuarant, reviewCustomer);

    if (rating >= 3) { // Good review, here I am considering a review of 3 as good since it isnt explicitly told what is good review and what is bad
        if (goodReviewsCount == goodReviewsCapacity) {
            expandHeap(&goodReviewsHeap, goodReviewsCapacity);
        }
        goodReviewsHeap[goodReviewsCount++] = newReview;
        heapifyUp(goodReviewsHeap, goodReviewsCount);
    }
    else if (rating <= 2) { // Bad review
        if (badReviewsCount == badReviewsCapacity) {
            expandHeap(&badReviewsHeap, badReviewsCapacity);
        }
        badReviewsHeap[badReviewsCount++] = newReview;
        heapifyUp(badReviewsHeap, badReviewsCount);
    }

    std::cout << "Review added successfully!" << std::endl;
}

void addReview(Dish* dish, int rating, const std::string& description, Customer* reviewCustomer) {
    Reviews* newReview = new Reviews(reviewIdCounter++, rating, description, dish, reviewCustomer);

    if (rating >= 4) { // Good review
        if (goodReviewsCount == goodReviewsCapacity) {
            expandHeap(&goodReviewsHeap, goodReviewsCapacity);
        }
        goodReviewsHeap[goodReviewsCount++] = newReview;
        heapifyUp(goodReviewsHeap, goodReviewsCount);
    }
    else if (rating <= 2) { // Bad review
        if (badReviewsCount == badReviewsCapacity) {
            expandHeap(&badReviewsHeap, badReviewsCapacity);
        }
        badReviewsHeap[badReviewsCount++] = newReview;
        heapifyUp(badReviewsHeap, badReviewsCount);
    }

    std::cout << "Review added successfully!" << std::endl;
}

// Expand heap capacity
void expandHeap(Reviews*** heap, int& capacity) {
    int newCapacity = (capacity == 0) ? 1 : capacity * 2;
    Reviews** newHeap = new Reviews * [newCapacity];

    for (int i = 0; i < capacity; ++i) {
        newHeap[i] = (*heap)[i];
    }

    delete[] * heap;
    *heap = newHeap;
    capacity = newCapacity;
}

// Heapify-up operation
void heapifyUp(Reviews** heap, int count) {
    int index = count - 1;
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[parent]->rating >= heap[index]->rating) {
            break;
        }
        std::swap(heap[parent], heap[index]);
        index = parent;
    }
}

// Heapify-down operation
void heapifyDown(Reviews** heap, int count, int index) {
    while (index < count) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < count && heap[left]->rating > heap[largest]->rating) {
            largest = left;
        }
        if (right < count && heap[right]->rating > heap[largest]->rating) {
            largest = right;
        }
        if (largest == index) {
            break;
        }
        std::swap(heap[largest], heap[index]);
        index = largest;
    }
}

// View top 5 reviews
void viewTopReviews(bool good) {
    Reviews** heap = good ? goodReviewsHeap : badReviewsHeap;
    int count = good ? goodReviewsCount : badReviewsCount;

    if (count == 0) {
        std::cout << (good ? "No good reviews available!" : "No bad reviews available!") << std::endl;
        return;
    }

    std::cout << "Top 5 " << (good ? "Good" : "Bad") << " Reviews:" << std::endl;

    for (int i = 0; i < 5 && i < count; ++i) {
        Reviews* review = heap[i];
        std::cout << "Review ID: " << review->reviewId
            << ", Rating: " << review->rating
            << ", Description: " << review->description << std::endl;
    }
}
