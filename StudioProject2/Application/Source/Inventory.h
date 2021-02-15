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



private:
	int x = 0;
	int y = 0;

};
