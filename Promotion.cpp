#include "Promotions.h"

int promotionIdCounter = 10000;

Promotion::Promotion(int id, std::string name, double discount, int limit)
	: promotionId(id), name(name), discount(discount), limit(limit)
{
	numPromotionsApplied = 0;
	next = NULL;
}

Promotion* addPromotion(Promotion* head, int id, std::string name, double discount, int limit) {
	if (head == NULL) {
		std::cout << "Promotion added to stack successfully!" << std::endl;
		return new Promotion(id, name, discount, limit);
	}

	// adding at at head
	Promotion* temp = new Promotion(id, name, discount, limit);
	temp->next = head;
	std::cout << "Promotion added to stack successfully!" << std::endl;
	return temp;
}

Promotion* checkLimit(Promotion* head) {
	if (head == NULL) return NULL;

	// popping
	if (head->limit == head->numPromotionsApplied) {
		Promotion* temp = head->next;
		head->next = NULL;
		delete head;
		return temp;
	}
	else return head;
}

