#include "Button.h"
#include "Application.h"
#include <iostream>

Button::Button()
{
	positionX = positionY = width = height = 0;
	clickedOn = active = false;
	bLButtonState = true;
	hold = false;
}

Button::Button(float positionX, float positionY, float width, float height, bool isActive)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->width = width;
	this->height = height;
	clickedOn = false;
	bLButtonState = true;
	active = isActive;
	hold = false;
}

Button::~Button()
{
}

void Button::setButton(float positionX, float positionY, float width, float height)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->width = width;
	this->height = height;
	clickedOn = false;
	bLButtonState = true;
	active = true;
	hold = false;
}

bool Button::isClickedOn()
{
	return clickedOn;
}

void Button::updateButton()
{
	if (active) {
		if (Application::IsMousePressed(0) && (bLButtonState))
		{
			double x, y;
			Application::GetCursorPos(&x, &y);
			unsigned w = Application::GetWindowWidth();
			unsigned h = Application::GetWindowHeight();
			float posX = x / 10;
			float posY = 60 - y / 10;
			//std::cout << "posX = " << posX << std::endl;
			//std::cout << "posY = " << posY << std::endl;
			if (posX > positionX && posX < (positionX + width) && posY > positionY && posY < (positionY + height)) 
			{
				//std::cout << "hit!"<< std::endl;
				clickedOn = true;
				hold = true;
			}
			bLButtonState = false;
		}
		else if (Application::IsMousePressed(0) && (!bLButtonState)) {
			clickedOn = false;
		}
		else if (!Application::IsMousePressed(0) && !bLButtonState)
		{
			clickedOn = false;
			bLButtonState = true;
			hold = false;
		}
	}
}
