#pragma once

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

	void update_inventory();


private:
	int x;
	int y;
	int key;
	int clue1;
};
