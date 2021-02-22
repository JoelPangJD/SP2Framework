#pragma once
#include "Entity.h"
#include "InteractableObject.h"
#include <vector>

class Inventory : public InteractableObject
{
public:
	Inventory();
	~Inventory();
	void additem(InteractableObject* item);
	void RemoveItem(string item);



private:
	std::vector<InteractableObject*> storage;
};
