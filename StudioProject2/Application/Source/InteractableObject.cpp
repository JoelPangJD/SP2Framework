#include "InteractableObject.h"

InteractableObject::InteractableObject()
{
	this->setposition(Vector3(0, 0, 0));
	this->setangle(0);
	this->setscale(1);
}

InteractableObject::InteractableObject(Vector3 pos, float angle, float scale, float radius, string type)
{
	this->setposition(pos);
	this->setangle(angle);
	this->setscale(scale);
	this->setradius(radius);
	this->settype(type);
}

InteractableObject::InteractableObject(Vector3 pos, float angle, float scale, float height, float xwidth, float zwidth, string type)
{
	this->setposition(pos);
	this->setangle(angle);
	this->setscale(scale);
	this->setheight(height);
	this->setxwidth(xwidth);
	this->setzwidth(zwidth);
}

InteractableObject::~InteractableObject()
{
}

void InteractableObject::touch(Vector3 position)
{
	
}
