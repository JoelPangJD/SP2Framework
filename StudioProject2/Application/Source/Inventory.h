#pragma once
#include "InteractableObject.h"
#include <vector>

class Inventory
{
private:
	std::vector<InteractableObject*> storage;
public:
	Inventory();
	~Inventory();
	void additem(InteractableObject* item);
	void RemoveItem(string item);
	std::vector<InteractableObject*> getstorage();
};
