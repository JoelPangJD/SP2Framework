#ifndef CAMERA3_H
#define CAMERA3_H

#include "Vector3.h"

class Camera3
{
public:
	float phi, theta;
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Camera3();
	~Camera3();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Reset();
	void Update(double dt);
	void Update(Vector3 target, double dt);
};

#endif