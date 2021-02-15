#ifndef LIGHT_H
#define LIGHT_H

#include "Vertex.h";

struct Light
{
	Position position;
	Color color;
	float power;
	float kC, kL, kQ;
	Light()
	{
		type = Light::LIGHT_SPOT;
		position = (0, 20, 0);
		color = (1, 1, 1);
		power = 1;
		kC = 1.f;
		kL = 0.01f;
		kQ = 0.001f;
		cosCutoff = cos(Math::DegreeToRadian(45));
		cosInner = cos(Math::DegreeToRadian(30));
		exponent = 3.f;
		spotDirection.Set(0.5f, 1.f, 0.f);
	}
	enum LIGHT_TYPE
	{
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,
	};
	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;
};

#endif