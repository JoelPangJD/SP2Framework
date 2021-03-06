#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:

	Application();
	~Application();
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double* xpos, double* ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();
	static unsigned m_width;
	static unsigned m_height;
	static bool enableMouse;
	static int SwitchScene; 
	static void SetSwitchScene();
	void Init();
	void Run();
	void Exit();

	static bool GameEnd;


private:

	//Declare a window object
	StopWatch m_timer;
	bool firstMouse;
	float lastX, lastY;
};

#endif