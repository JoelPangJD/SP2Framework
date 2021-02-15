#pragma once
#include<iostream>
#include<string>
#include "Vector3.h"

class Entity
{
private:
	Vector3 position;
	float scale;
	float angle;
	float radius; //radius of the circle collider
	float height, xwidth, zwidth;	//scaling of hitbox
public:
	Entity();
	~Entity();
	Entity(Vector3 position, float scale, float angle, float radius);
	Entity(Vector3 position, float scale, float angle, float height, float width);
	Entity(Vector3 position, float scale, float angle, float height, float xwidth, float zwidth);
	Vector3 getposition();
	void setposition(Vector3 position);
	float getscale();
	void setscale(float x);
	float getangle();
	void setangle(float x);

	void moveentity(int direction, float speed, double dt);

	bool spherecollider(Vector3 position);
	bool boxcollider(Vector3 camera);
};

