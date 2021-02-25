#pragma once
#include "InteractableObject.h"
#include <vector>

class Inventory
{
private:
	std::vector<InteractableObject*> storage;
	std::vector<InteractableObject*>::iterator currentitem;
public:
	Inventory();
	~Inventory();
	InteractableObject* getcurrentitem();
	int getcurrentindex(); //get the position of the current item
	void navigateinventory(int direction); //1 for up, 2 for down
	void additem(InteractableObject* item);
	void removeitem(InteractableObject* item);
	std::vector<InteractableObject*> getstorage();
};
