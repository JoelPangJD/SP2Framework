#include "Entity.h"

Entity::Entity()
{
	position = Vector3(0, 0, 0);
	scale = 1;
	angle = 0;
	radius = 0;
	type = "";
}

Entity::~Entity()
{
}

Vector3 Entity::getposition()
{
	return position;
}

void Entity::setposition(Vector3 position)
{
	this->position = position;
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

float Entity::getradius()
{
	return radius;
}

void Entity::setradius(float radius)
{
	this->radius = radius;
}

float Entity::getheight()
{
	return height;
}

void Entity::setheight(float height)
{
	this->height = height;
}

float Entity::getxwidth()
{
	return xwidth;
}

void Entity::setxwidth(float xwidth)
{
	this->xwidth = xwidth;
}

float Entity::getzwidth()
{
	return zwidth;
}

void Entity::setzwidth(float zwidth)
{
	this->zwidth = zwidth;
}

string Entity::gettype()
{
	return type;
}

string Entity::getname()
{
	return name;
}

void Entity::settype(string type)
{
	this->type = type;
}

void Entity::setname(string name)
{
	this->name = name;
}

void Entity::moveentity(int direction, float speed, double dt)
{
	speed = speed * dt;
	if (direction == 1) //foward //World bounds are 100 to -100
	{
		position.x += sin(angle * (3.141592 / 180)) * speed;
		position.z += cos(angle * (3.141592 / 180)) * speed;
	}
	else if (direction == 2) //backwards
	{
		position.x -= sin(angle * (3.141592 / 180)) * speed;
		position.z -= cos(angle * (3.141592 / 180)) * speed;
	}
}

bool Entity::spherecollider(Vector3 position)
{
	//distance between two points ( ( (x1-x2)^2 + (z1-z2)^2 )*0.5 )^2 + (y1-y2)^2 )^0.5) 
	if(pow(pow(pow(pow((this->position.x - position.x), 2.f) + pow((this->position.z - position.z), 2.f), 0.5f),2.f) + pow((this-> position.y - position.y), 2.f),0.5f) < radius)
		return true;
	return false;
}

bool Entity::circlecollider(Vector3 position)
{
	if (pow(pow((this->position.x - position.x), 2.f) + pow((this->position.z - position.z), 2.f), 0.5f) < radius)
		return true;
	return false;
}

bool Entity::boxcollider(Vector3 camera)
{	//checks if entity is within the camera's Vector3 passed in
	if (camera.x < position.x + xwidth * 0.5 && camera.x > position.x - xwidth * 0.5
		&& camera.y < position.y + height * 0.5 && camera.y > position.y - height * 0.5
		&& camera.z < position.z + zwidth * 0.5 && camera.z > position.z - zwidth * 0.5)
		return true;
	return false;
}

