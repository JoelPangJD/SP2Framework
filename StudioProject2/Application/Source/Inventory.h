#pragma once
#include "Entity.h"
#include "InteractableObject.h"
#include <vector>

class Inventory
{
public:
	Inventory();
	~Inventory();
	int position(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	int get_key();
	int get_clue();
	void additem(InteractableObject item);
	void update_inventory();


private:
	int x;
	int y;
	int key;
	int clue1;
	std::vector<InteractableObject> storage;
};
