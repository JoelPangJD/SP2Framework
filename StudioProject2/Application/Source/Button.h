#pragma once
#include <string>


class Button
{
public:
	float positionX, positionY, width, height;
	bool clickedOn; 
	bool bLButtonState;
	bool active;
	bool hold;

	Button();
	Button(float positionX, float positionY, float width, float height, bool isActive = false);//positionx and position y is bottom left
	~Button();
	bool isClickedOn();//returns value of whether its clicked on
	void updateButton();//checks whether its clicked on; meant to be put in update function
};

