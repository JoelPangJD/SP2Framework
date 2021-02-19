#pragma once
#include <string>


class Button
{
private:
	float positionX, positionY, width, height;
	bool clickedOn; 
public:
	Button(float positionX, float positionY, float width, float height);//positionx and position y is bottom left
	~Button();
	bool isClickedOn();//returns value of whether its clicked on
	void updateButton();//checks whether its clicked on; meant to be put in update function

};

