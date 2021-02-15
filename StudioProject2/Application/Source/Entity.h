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

public:
	Entity();
	Entity(Vector3 position, float scale, float angle);
	~Entity();
	Vector3 getposition();
	void setposition(Vector3 position);
	float getscale();
	void setscale(float x);
	float getangle();
	void setangle(float x);

	void moveentity(int direction, float speed, double dt);

	bool circlecollider(Vector3 camera);

};

