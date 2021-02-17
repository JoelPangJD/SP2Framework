#pragma once
#include <string>
#include "Entity.h"
#include "Inventory.h"
using namespace std;

class InteractableObject : public Entity
{
public:
	InteractableObject();
	InteractableObject(Vector3 pos, float angle, float scale, float radius, string type);
	InteractableObject(Vector3 pos, float angle, float scale, float height, float xwidth, float zwidth, string type);
	~InteractableObject();
	void pickup(Inventory inventory);
};