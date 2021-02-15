#include "Entity.h"

Entity::Entity()
{
	x = 0;
	y = 0;
	z = 0;
	scale = 0;
	angle = 0;
}

Entity::Entity(float x, float y, float z, float scale, float angle, std::string type)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->scale = scale;
	this->angle = angle;
	this->type = type;
}

Entity::~Entity()
{
}

float Entity::getx()
{
	return x;
}

void Entity::setx(float x)
{
	this->x = x;
}

float Entity::gety()
{
	return y;
}

void Entity::sety(float y)
{
	this->y = y;
}

float Entity::getz()
{
	return z;
}

void Entity::setz(float z)
{
	this->z = z;
}

float Entity::getscale()
{
	return scale;
}

void Entity::setscale(float scale)
{
	this->scale = scale;
}

float Entity::getangle()
{
	return angle;
}

void Entity::setangle(float angle)
{
	this->angle = angle;
	if (angle >= 360)
		angle -= 360;
	else if (angle <= -360)
		angle += 360;
}

std::string Entity::gettype()
{
	return type;
}

void Entity::settype(std::string type)
{
	this->type = type;
}

void Entity::moveentity(int direction, float speed, double dt)
{
	speed = speed * dt;
	if (direction == 1) //foward //World bounds are 100 to -100
	{
		float testx = x + sin(angle * (3.141592 / 180)) * speed;
		float testz = z + sin(angle * (3.141592 / 180)) * speed;
		if(testx < 100 && testx > -100)
			x += sin(angle * (3.141592 / 180)) * speed;
		if (testz < 100 && testz > -100)
			z += cos(angle * (3.141592 / 180)) * speed;
	}
	else if (direction == 2) //backwards
	{
		float testx = x - sin(angle * (3.141592 / 180)) * speed;
		float testz = z - sin(angle * (3.141592 / 180)) * speed;
		if (testx < 100 && testx > -100)
			x -= sin(angle * (3.141592 / 180)) * speed;
		if (testz < 100 && testz > -100)
			z -= cos(angle * (3.141592 / 180)) * speed;
	}
}
