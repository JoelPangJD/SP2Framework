#include "Inventory.h"


Inventory::Inventory()
{
}

Inventory::~Inventory()
{
	for (auto it = storage.begin(); it != storage.end(); it++)//Delete pointers in the storage vector
		delete (*it);
}

InteractableObject* Inventory::getcurrentitem()
{
	return *currentitem;
}

int Inventory::getcurrentindex()
{
	return (currentitem - storage.begin());
}
void Inventory::navigateinventory(int direction)
{
	if (!storage.empty())
	{
		if (direction == 1)
		{
			if (currentitem == storage.begin()) //Wrap around if above first item
				currentitem = storage.end() - 1;
			currentitem--;
		}
		else if (direction == 2)
		{
			currentitem++;
			if (currentitem == storage.end()) //Wrap around if last item
				currentitem = storage.begin();
		}
	}
}

void Inventory::additem(InteractableObject* item)
{
	storage.push_back(item);
	currentitem = storage.begin();
}

void Inventory::RemoveItem(string Item)
{
	for (std::vector<InteractableObject*>::iterator it = storage.begin(); it != storage.end(); it++)
	{
		if ((*it)->gettype() == Item)
		{
			storage.erase(storage.begin());
		}
	}
}

vector<InteractableObject*> Inventory::getstorage()
{
	return storage;
}


