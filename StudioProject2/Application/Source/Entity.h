#pragma once
#include<iostream>
#include<string>
#include "Vector3.h"
using namespace std;
class Entity
{
protected:
	int x;
	int y;
	Vector3 ItemPosition;

private:
	string type;
	//Vector3 position;
	float scale;
	float angle;
	float radius; //radius of the circle collider
	float height, xwidth, zwidth;	//scaling of hitbox
public:
	Vector3 position;
	Entity();
	~Entity();
	Entity(Vector3 position, float scale, float angle, float radius, string type);
	Entity(Vector3 position, float scale, float angle, float height, float width, string type);
	Entity(Vector3 position, float scale, float angle, float height, float xwidth, float zwidth, string type);

	//Getters
	Vector3 getposition();
	float getscale();
	float getangle();
	float getradius();
	float getheight();
	float getxwidth();
	float getzwidth();
	string gettype();

	//Setters 
	void setposition(Vector3 position);
	void setscale(float x);
	void setangle(float x);
	void setradius(float radius);
	void setheight(float height);
	void setxwidth(float xwidth);
	void setzwidth(float zwidth);
	void settype(string type);

	void moveentity(int direction, float speed, double dt);
	bool spherecollider(Vector3 position);
	bool circlecollider(Vector3 position);
	bool boxcollider(Vector3 camera);
};

