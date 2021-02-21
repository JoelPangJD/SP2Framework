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
	fstream descriptionfile("Itemdescriptions.txt");
	while (getline(descriptionfile, line))
	{
		int instance = line.find('|');
		if (this->gettype() == line.substr(0, instance)) //If found item description based on item type in the text file
		{
			int instance2 = line.find('|', instance + 1);
			lookat.push_back(line.substr(instance + 1, instance2 - 1 - instance));
		}
	}
	fstream dialoguefile("Itemdialogue.txt");
	while (getline(dialoguefile, line))
	{
		int instance = line.find('|');
		int instance2;
		if (this->gettype() == line.substr(0, instance))
		{
			for (int i = std::count(line.begin(), line.end(), '|'); i > 0; i--) //Reads dialogue from dialogue file and splits the dialogue into a string vector
			{
				instance2 = line.find('|', instance + 1);
				dialogue.push_back(line.substr(instance + 1, instance2 - 1 - instance));
				instance = instance2;
			}
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

int InteractableObject::interact()
{
	if (Application::IsKeyPressed('F'))//Look at (read the description from text file based on object type)
	{
		return 1;
	}
	else if (Application::IsKeyPressed('G')) //Pick up
	{
		return 2;
	}
	else if (Application::IsKeyPressed('R')) //Use
	{
		return 3;
	}
	else if (Application::IsKeyPressed('T'))//Talk to 
	{
		return 4;
	}
	return 0;
}

