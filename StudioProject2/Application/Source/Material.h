#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vertex.h";

struct Component
{
	float r, g, b;
	Component(float r = 0.1f, float g = 0.1f, float b = 0.1f)
	{
		Set(r, g, b);
	}
	void Set(float r, float g, float b)
	{
		this->r = r; this->g = g; this->b = b;
	}
	Component& operator=(const Component& rhs)
	{
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
		return *this;
	}
};
struct Material
{
	unsigned size;
	Component kAmbient;
	Component kDiffuse;
	Component kSpecular;
	float kShininess;
	Material()
	{
		kAmbient = Component(0.5f, 0.5f, 0.5f);
		kDiffuse = Component(0.5f, 0.5f, 0.5f);
		kSpecular = Component(0.5f, 0.5f, 0.5f);
		kShininess = 1.f;
		size = 0;
	}
	Material& operator=(const Material& rhs)
	{
		kAmbient = rhs.kAmbient;
		kDiffuse = rhs.kDiffuse;
		kSpecular = rhs.kSpecular;
		kShininess = rhs.kShininess;
		size = rhs.size;
		return *this;
	}
};

#endif