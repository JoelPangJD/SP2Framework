#pragma once
#include<iostream>
#include<string>
#include "Vector3.h"
using namespace std;
class Entity
{
private:
	string type;
	string name;
	//Vector3 position;
	float scale;
	float angle;
	float radius; //radius of the circle collider
	float height, xwidth, zwidth;	//scaling of hitbox
public:
	Entity();
	~Entity();

	Vector3 position;
	//Getters
	Vector3 getposition();
	float getscale();
	float getangle();
	float getradius();
	float getheight();
	float getxwidth();
	float getzwidth();
	string gettype();
	string getname();

	//Setters 
	void setposition(Vector3 position);
	void setscale(float x);
	void setangle(float x);
	void setradius(float radius);
	void setheight(float height);
	void setxwidth(float xwidth);
	void setzwidth(float zwidth);
	void settype(string type);
	void setname(string name);

	void moveentity(int direction, float speed, double dt);
	bool spherecollider(Vector3 position); //Checks for collisions is a sphere around an entity
	bool circlecollider(Vector3 position); //Checks for collisions in a circle around an entity
	bool boxcollider(Vector3 camera); //Check for collisions in a box around an entity
};

