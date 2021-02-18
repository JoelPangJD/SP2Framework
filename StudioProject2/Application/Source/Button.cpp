#include "Button.h"
#include "Application.h"

Button::Button(float positionX, float positionY, float width, float height)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->width = width;
	this->height = height;
	clickedOn = false;
	text = "";
}

Button::~Button()
{
}

bool Button::isClickedOn()
{
	return clickedOn;
}

void Button::renderButton()
{
	
}

void Button::updateButton()
{

	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;

		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / 10;
		float posY = 60 - y / 10;
		if (posX > positionX && posX < (positionX + width) && posY > positionY && posY < (positionY + height)) {
			clickedOn = true;
		}
		else {
			clickedOn = false;
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}
}
