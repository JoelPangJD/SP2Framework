#include "Inventory.h"
#include "SceneMain.h"
#include "GL\glew.h"
#include "Application.h"
#include "LoadTGA.h"
#include "Utility.h"
#include "shader.hpp"
#include <Mtx44.h>
#include"MeshBuilder.h"
#include "Mesh.h"
#include "SceneMuseum.h"


Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::additem(InteractableObject* item)
{
	storage.push_back(item);
}

void Inventory::RemoveItem(string Item)
{
	for (std::vector<InteractableObject*>::iterator it = storage.begin(); it != storage.end(); it++)
	{
		if ((*it)->gettype() == "fish")
		{
			storage.erase(storage.begin());
		}
	}

}


