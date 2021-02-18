#include "Terrain.h"

Terrain::Terrain()
{
	setposition(Vector3(0, 0, 0));
	setangle(0);
	setscale(1);
	settype("");
}

Terrain::Terrain(Vector3 pos, float angle, float scale, float radius, string type)
{
	setposition(pos);
	setangle(angle);
	setscale(scale);
	setradius(radius);
	settype(type);
}

Terrain::Terrain(Vector3 pos, float angle, float scale, float xwidth, float zwidth, string type)
{
	setposition(pos);
	setangle(angle);
	setscale(scale);
	setxwidth(xwidth);
	setzwidth(zwidth);
	settype(type);
}

Terrain::Terrain(Vector3 pos, float angle, float scale, float height, float xwidth, float zwidth, string type)
{
	setposition(pos);
	setangle(angle);
	setscale(scale);
	setheight(height);
	setxwidth(xwidth);
	setzwidth(zwidth);
	settype(type);
}

void Terrain::solidCollisionBox(Vector3 &position)
{
	if (position.x < this->getposition().x + (this->getxwidth() * 0.5) //Check for a box collision along the xz plane
		&& position.x > this->getposition().x - (this->getxwidth() * 0.5)
		&&
		position.z < this->getposition().z + (this->getzwidth() * 0.5)
		&& position.z > this->getposition().z - (this->getzwidth() * 0.5))
	{//if a collision is found, push the position out of the hit box
		float xdist;
		float zdist;
		if (position.x > this->getposition().x) //if x is to the right of the
			xdist = this->getposition().x + (this->getxwidth() * 0.5) - position.x;
		else
			xdist = this->getposition().x - (this->getxwidth() * 0.5) - position.x;
		if (position.z > this->getposition().z) //if x is to the front of the
			zdist = this->getposition().z + (this->getzwidth() * 0.5) - position.z;
		else
			zdist = this->getposition().z - (this->getzwidth() * 0.5) - position.z;

		if (abs(xdist) < abs(zdist))
		{
			if (position.x > this->getposition().x)
				position.x += xdist;
			else
				position.x += xdist;
		}
		else
		{
			if (position.z > this->getposition().z)
				position.z += zdist;
			else
				position.z += zdist;
		}
	}
}

Terrain::~Terrain()
{
}
