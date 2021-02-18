#pragma once
#include <string>
#include "Entity.h"
using namespace std;

class InteractableObject : public Entity
{
public:
	InteractableObject();
	InteractableObject(Vector3 pos, float angle, float scale, float radius, string type);
	InteractableObject(Vector3 pos, float angle, float scale, float height, float xwidth, float zwidth, string type);
	~InteractableObject();
};