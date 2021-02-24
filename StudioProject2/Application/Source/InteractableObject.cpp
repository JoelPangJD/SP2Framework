#include "InteractableObject.h"
InteractableObject::InteractableObject()
{
	this->setposition(Vector3(0, 0, 0));
	this->setangle(0);
	this->setscale(1);
}

bool InteractableObject::getpickupable()
{
	return pickupable;
}

void InteractableObject::readfromfile(string filename, vector<string>& type)
{
	std::string line;
	fstream file(filename);
	while (getline(file, line))
	{
		int instance = line.find('|');
		int instance2;
		if (this->gettype() == line.substr(0, instance))
		{
			for (int i = std::count(line.begin(), line.end(), '|'); i > 0; i--) //Reads data from dialogue file and splits the data into a string vector
			{
				instance2 = line.find('|', instance + 1);
				type.push_back(line.substr(instance + 1, instance2 - 1 - instance)); //Separate lines based on | positions
				instance = instance2;
			}
		}
	}
}

InteractableObject::InteractableObject(Vector3 pos, float angle, float scale, float radius, string type, string name, bool pickupable)
{
	setposition(pos);
	setangle(angle);
	setscale(scale);
	setradius(radius);
	settype(type);
	setname(name);
	this->pickupable = pickupable;
	readfromfile("Itemdescriptions.txt", lookat);
	readfromfile("Itemdialogue.txt", dialogue);
}

InteractableObject::~InteractableObject()
{

}

void InteractableObject::setpickupable(bool pickupable)
{
	this->pickupable = pickupable;
}

void InteractableObject::updatedialogue(string type)
{
	settype(type);
	dialogue.clear();
	lookat.clear();
	readfromfile("Itemdescriptions.txt", lookat);
	readfromfile("Itemdialogue.txt", dialogue);
}

int InteractableObject::interact()
{
	return 0;
}

