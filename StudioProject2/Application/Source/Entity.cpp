#include "Entity.h"

Entity::Entity()
{
	position = Vector3(0, 0, 0);
	scale = 1;
	angle = 0;
	radius = 0;
}

Entity::Entity(Vector3 position, float scale, float angle, float radius)
{
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->radius = radius;
	this->height = height;
	xwidth = width;
	zwidth = width;
}

Entity::Entity(Vector3 position, float scale, float angle, float height, float width)
{
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->radius = radius;
	this->height = height;
	xwidth = width;
	zwidth = width;
}
Entity::Entity(Vector3 position, float scale, float angle, float height, float xwidth, float zwidth)
{
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->height = height;
	this->xwidth = xwidth;
	this->zwidth = zwidth;
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
}

bool Entity::boxcollider(Vector3 camera)
{
	if (camera.x < position.x + scale * 0.5 && camera.x > position.x - scale * 0.5
		&& camera.y < position.y + scale * 0.5 && camera.y > position.y - scale * 0.5)
	return false;
}

