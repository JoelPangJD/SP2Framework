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
	return (*currentitem);
}

int Inventory::getcurrentindex()
{
	if (!storage.empty())
	{
		return (currentitem - storage.begin());
	}
	return NULL;
}
void Inventory::navigateinventory(int direction)
{
	if (!storage.empty())
	{
		if (direction == 1)
		{
			if (currentitem == storage.begin()) //Wrap around if above first item
				currentitem = storage.end();
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

void Inventory::removeitem(string type)
{
	for (std::vector<InteractableObject*>::iterator it = storage.begin(); it != storage.end(); it++)
	{
		if ((*it)->gettype() == type)
		{
			storage.erase(storage.begin() + distance(storage.begin(), it));
			break;
		}
	}
	currentitem = storage.begin();
}

void Inventory::removeitem(InteractableObject* item)
{
	for (std::vector<InteractableObject*>::iterator it = storage.begin(); it != storage.end(); it++)
	{
		if ((*it) == item)
		{
			storage.erase(storage.begin() + distance(storage.begin(), it));
			break;
		}
	}
	currentitem = storage.begin();
}

bool Inventory::checkinventory(string type)
{
	for (auto it = storage.begin(); it != storage.end(); it++)
	{
		if ((*it)->gettype() == type)
			return true;			//if found, return true and break
	}
	return false;
}

vector<InteractableObject*> Inventory::getstorage()
{
	return storage;
}


