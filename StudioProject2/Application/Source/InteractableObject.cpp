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

	std::string line;
	fstream file("Itemdescriptions.txt");
	while (getline(file, line))
	{
		int instance = line.find('|');
		if (this->gettype() == line.substr(0, instance)) //If found item description based on item type in the text file
		{
			int instance2 = line.find('|', instance + 1);
			lookat = line.substr(instance + 1, instance2 - 1 - instance);
			cout << lookat << endl;
		}
	}
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

void InteractableObject::interact()
{
	if (Application::IsKeyPressed('F'))//Look at (read the description from text file based on object type)
	{
		cout << lookat << endl;
	}
	else if (Application::IsKeyPressed('G')) //Pick up
	{

	}
	else if (Application::IsKeyPressed('R')) //use
	{

	}
	else if (Application::IsKeyPressed('T'))//Talk to 
	{

	}
}

