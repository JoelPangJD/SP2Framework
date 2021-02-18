#include "Terrain.h"

Terrain::Terrain()
{
	this->setposition(Vector3(0, 0, 0));
	this->setangle(0);
	this->setscale(1);
}

Terrain::Terrain(Vector3 pos, float angle, float scale, float radius, string type)
{
	this->setposition(pos);
	this->setangle(angle);
	this->setscale(scale);
	this->setradius(radius);
}

Terrain::Terrain(Vector3 pos, float angle, float scale, float xwidth, float zwidth, string type)
{
	this->setposition(pos);
	this->setangle(angle);
	this->setscale(scale);
	this->setxwidth(xwidth);
	this->setzwidth(zwidth);
}

Terrain::Terrain(Vector3 pos, float angle, float scale, float height, float xwidth, float zwidth, string type)
{
	this->setposition(pos);
	this->setangle(angle);
	this->setscale(scale);
	this->setheight(height);
	this->setxwidth(xwidth);
	this->setzwidth(zwidth);
}

void Terrain::solidCollisionBox(Vector3 &position)
{
	//if (position.x < this->getposition().x + (this->getxwidth() * 0.5) //Check for a box collision along the xz plane
	//	&& position.x > this->getposition().x - (this->getxwidth() * 0.5)
	//	&&
	//	position.z < this->getposition().z + (this->getzwidth() * 0.5)
	//	&& position.z > this->getposition().z - (this->getzwidth() * 0.5))
	//{//if a collision is found, push the position out of the hit box
	//	float xdist;
	//	float zdist;
	//	if (position.x > this->getposition().x) //if x is to the right of the
	//	{
	//		xdist = this->getposition().x + (this->getxwidth() * 0.5) -;
	//	}
	//	else
	//	{

	//	}


	//}


}

Terrain::~Terrain()
{
}
