
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
	static bool SwitchScene; 
	static void SetSwitchScene();
	void Init();
	void Run();
	void Exit();


private:

	//Declare a window object
	StopWatch m_timer;
};

#endif