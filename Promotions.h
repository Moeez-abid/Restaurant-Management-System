#pragma once
#include <iostream>

extern int promotionIdCounter;

struct Promotion {
	int promotionId;
	std::string name;
	double discount;
	int limit;
	int numPromotionsApplied;
	Promotion* next;

	Promotion(int id, std::string name, double discount, int limit);
};

Promotion* addPromotion(Promotion* head, int id, std::string name, double discount, int limit);
Promotion* checkLimit(Promotion* head);