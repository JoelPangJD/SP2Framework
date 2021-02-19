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

void InteractableObject::interact()
{
	if (Application::IsMousePressed('F'))//Look at (read the description from text file based on object type)
	{
		std::string line;
		fstream file("Itemdescriptions.txt");
		while (getline(file, line));
		{
			size_t instance = line.find('|');
			if (this->gettype() == line.substr(0, instance)) //If found item description based on item type in the text file
			{
				size_t instance2 = line.find('|',instance);
				size_t instance3 = line.find('|', instance2);
				size_t instance4 = line.find('|', instance3);
				cout << line.substr(instance2,(instance3-1)) << endl;
			}
		}

	}
	else if (Application::IsMousePressed('G')) //Pick up
	{

	}
	else if (Application::IsMousePressed('R')) //use
	{

	}
	else if (Application::IsMousePressed('T'))//Talk to 
	{

	}
}

