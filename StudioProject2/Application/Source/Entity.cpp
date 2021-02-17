#include "Entity.h"

Entity::Entity()
{
	position = Vector3(0, 0, 0);
	scale = 1;
	angle = 0;
	radius = 0;
	type = "";
}

Entity::Entity(Vector3 position, float scale, float angle, float radius, string type)
{
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->radius = radius;
	this->type = type;
}

Entity::Entity(Vector3 position, float scale, float angle, float height, float width, string type)
{
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->radius = radius;
	this->height = height;
	xwidth = width;
	zwidth = width;
	this->type = type;
}
Entity::Entity(Vector3 position, float scale, float angle, float height, float xwidth, float zwidth, string type)
{
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->height = height;
	this->xwidth = xwidth;
	this->zwidth = zwidth;
	this->type = type;
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

string Entity::gettype()
{
	return type;
}

void Entity::settype(string type)
{
	this->type = type;
}

void Entity::moveentity(int direction, float speed, double dt)
{
	speed = speed * dt;
	if (direction == 1) //foward //World bounds are 100 to -100
	{
		float testx = position.x + sin(angle * (3.141592 / 180)) * speed;
		float testz = position.z + sin(angle * (3.141592 / 180)) * speed;
		if(testx < 100 && testx > -100)
			position.x += sin(angle * (3.141592 / 180)) * speed;
		if (testz < 100 && testz > -100)
			position.z += cos(angle * (3.141592 / 180)) * speed;
	}
	else if (direction == 2) //backwards
	{
		float testx = position.x - sin(angle * (3.141592 / 180)) * speed;
		float testz = position.z - sin(angle * (3.141592 / 180)) * speed;
		if (testx < 100 && testx > -100)
			position.x -= sin(angle * (3.141592 / 180)) * speed;
		if (testz < 100 && testz > -100)
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

