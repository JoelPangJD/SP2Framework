#pragma once
#include "Entity.h"
class Terrain : public Entity
{
public:
	Terrain();
	Terrain(Vector3 pos, float angle, float scale, float xwidth, float zwidth, string type);
	Terrain(Vector3 pos, float angle, float scale, float height, float xwidth, float zwidth, string type);
	void solidCollisionBox(Vector3 &position); //Adds solid collisions to the position passed in so that they cannot overlap with terrains
	~Terrain();
};

