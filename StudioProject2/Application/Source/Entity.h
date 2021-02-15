#pragma once
#include<iostream>
#include<string>

class Entity
{
private:
	float x, y, z;
	float scale;
	float angle;
	std::string type;

public:
	Entity();
	Entity(float x, float y, float z, float scale, float angle, std::string type);
	~Entity();
	float getx();
	void setx(float x);
	float gety();
	void sety(float x);
	float getz();
	void setz(float x);
	float getscale();
	void setscale(float x);
	float getangle();
	void setangle(float x);
	std::string gettype();
	void settype(std::string type);

	void moveentity(int direction, float speed, double dt);

};

