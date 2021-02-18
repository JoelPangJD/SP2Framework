#pragma once
#include<iostream>
#include<string>
#include "Inventory.h"
#include "Vector3.h"
using namespace std;
class Entity
{
private:
	string type;
	Vector3 position;
	float scale;
	float angle;
	float radius; //radius of the circle collider
	float height, xwidth, zwidth;	//scaling of hitbox
public:

	Entity();
	~Entity();
	Entity(Vector3 position, float scale, float angle, float radius, string type);
	Entity(Vector3 position, float scale, float angle, float height, float width, string type);
	Entity(Vector3 position, float scale, float angle, float height, float xwidth, float zwidth, string type);
	Vector3 getposition();
	void setposition(Vector3 position);
	float getscale();
	void setscale(float x);
	float getangle();
	void setangle(float x);
	float getradius();
	void setradius(float radius);
	float getheight();
	void setheight(float height);
	float getxwidth();
	void setxwidth(float xwidth);
	float getzwidth();
	void setzwidth(float zwidth);
	string gettype();
	void settype(string type);

	void moveentity(int direction, float speed, double dt);
	bool spherecollider(Vector3 position);
	bool circlecollider(Vector3 position);
	bool boxcollider(Vector3 camera);
};

