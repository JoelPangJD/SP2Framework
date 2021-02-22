#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"
#include <GLFW/glfw3.h>

#define PI 3.141592

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera3::Camera3()
{
	position = Vector3(0,0,0);
	target = Vector3(0, 0, 1);
	up = Vector3(0, 1, 0);
	phi = 0;
	theta = 0;
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera3::~Camera3()
{
}

/******************************************************************************/
/*!
\brief
Initialize Camera3

\param pos - position of Camera3
\param target - where the Camera3 is looking at
\param up - up vector of Camera3
*/
/******************************************************************************/
void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;

	phi = atan((target.y - pos.y)/ sqrt(pow((target.x - pos.x),2.0) + pow((target.z - pos.z),2.0))) * (180/ PI);
	theta = atan((target.z - pos.z)/(target.x - pos.x)) * (180 / PI);

	lastX = 400;
	lastY = 300;
	firstMouse = true;
}

/******************************************************************************/
/*!
\brief
Reset the Camera3 settings
*/
/******************************************************************************/
void Camera3::Reset()
{
}


/******************************************************************************/
/*!
\brief
To be called every frame. Camera3 will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera3::Update(double dt)
{
	static const float speed = 10.f;
	static const float sensitivity = 0.1f; 

	if (Application::enableMouse == false) {

		if (Application::IsKeyPressed('W')) {
			Vector3 view = (target - position).Normalized();
			view.y = 0;
			view = view.Normalized();
			Vector3 test = position + view * speed * dt;
			position += view * speed * dt;
		}
		if (Application::IsKeyPressed('A')) {
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up).Normalized();
			right.y = 0;
			right = right.Normalized();
			position -= right * speed * dt;
		}
		if (Application::IsKeyPressed('S')) {
			Vector3 view = (target - position).Normalized();
			view.y = 0;
			view = view.Normalized();
			position -= view * speed * dt;
		}
		if (Application::IsKeyPressed('D')) {
			Vector3 view = (target - position).Normalized();
			Vector3 right = view.Cross(up).Normalized();
			right.y = 0;
			right = right.Normalized();
			position += right * speed * dt;
		}
		if (Application::IsKeyPressed('R'))
		{
			Reset();
		}
		if (Application::IsKeyPressed('C'))
		{
			//set scenery view
		}
		if (Application::IsKeyPressed(VK_SPACE)) {
			position.y += speed * dt;
		}
		if (Application::IsKeyPressed(VK_CONTROL)) {
			position.y -= speed * dt;
		}

		//4 is the distance that the target is away from the position
		target = position + Vector3(4 * cos(phi * 3.141592 / 180) * cos(theta * 3.141592 / 180), 4 * sin(phi * 3.141592 / 180), 4 * cos(phi * 3.141592 / 180) * sin(theta * 3.141592 / 180));

		double xpos;
		double ypos;
		Application::GetCursorPos(&xpos, &ypos);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		theta += xoffset;
		phi += yoffset;

		if (phi > 89.0f)
			phi = 89.0f;
		if (phi < -89.0f)
			phi = -89.0f;

		Vector3 direction;
		direction.x = cos(theta * PI / 180) * cos(phi * PI / 180);
		direction.y = sin(phi * PI / 180);
		direction.z = sin(theta * PI / 180) * cos(phi * PI / 180);
		Vector3 view = (direction).Normalized();
		Vector3 right = view.Cross(up).Normalized();
		right.y = 0;
		up = right.Cross(view).Normalized();
		target = position + view;
	}
}

void Camera3::Update(Vector3 target, double dt)
{
	if (Application::IsKeyPressed(VK_LEFT)) 
		theta -= 90 * dt;
	if (Application::IsKeyPressed(VK_RIGHT))
		theta += 90 * dt;
	this->target = target;
	position = target + Vector3(-6 * cos(phi * PI / 180) * cos(theta * PI / 180), -6 * sin(phi * PI / 180), -6 * cos(phi * PI / 180) * sin(theta * PI / 180));
}
