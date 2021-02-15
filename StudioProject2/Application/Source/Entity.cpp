#include "Entity.h"

Entity::Entity()
{
	position = Vector3(0, 0, 0);
	scale = 1;
	angle = 0;
}

Entity::Entity(Vector3 position, float scale, float angle)
{
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

bool Entity::circlecollider(Vector3 camera)
{
	return true;
}
