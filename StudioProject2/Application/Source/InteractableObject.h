#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Entity.h"
#include "Application.h"
using namespace std;

class InteractableObject : public Entity
{
public:
	InteractableObject();
	InteractableObject(Vector3 pos, float angle, float scale, float radius, string type);
	InteractableObject(Vector3 pos, float angle, float scale, float height, float xwidth, float zwidth, string type);
	~InteractableObject();
	void interact();
};