
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneMain.h"
#include "SceneMuseum.h"
#include "SceneGarden.h"
#include "SceneChangi.h"
#include "SceneMarinaBay.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}
int Application::SwitchScene = 0;
bool Application::enableMouse = false;//enable cursor to show or not to show

unsigned Application::m_width;
unsigned Application::m_height;
void resize_callback(GLFWwindow* window, int w, int h)
{
	Application::m_width = w;
	Application::m_height = h;
	glViewport(0, 0, w, h);
}
bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}
void Application::GetCursorPos(double* xpos, double* ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}
int Application::GetWindowWidth()
{
	return m_width;
}
int Application::GetWindowHeight()
{
	return m_height;
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_width = 800;
	m_height = 600;
	m_window = glfwCreateWindow(m_width, m_height, "Test Window", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	//Add mouse inputs
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	firstMouse = true;

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	SwitchScene = false;
}

void Application::SetSwitchScene()
{
	//SwitchScene = true;
}

void Application::Run()
{
	//Main Loop
	Scene* scene0 = new SceneMain();
	Scene* scene1 = new SceneMuseum();
	Scene* scene2 = new SceneChangi();
	Scene* scene3 = new SceneMarinaBay();
	Scene* scene4 = new SceneGarden();

	Scene* scene = scene0;
	scene1->Init();
	scene2->Init();
	scene3->Init();
	scene4->Init();
	scene0->Init();

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		if (IsKeyPressed(VK_F1)) {
			SwitchScene = 1;
		}
		else if (IsKeyPressed(VK_F2)) {
			SwitchScene = 2;
		}
		else if (IsKeyPressed(VK_F3)) {
			SwitchScene = 3;
		}
		else if (IsKeyPressed(VK_F4)) {
			SwitchScene = 4;
		}
		else if (IsKeyPressed(VK_RETURN)) {
			SwitchScene = 0;
		}

		switch (SwitchScene) {
			case(0): {
				scene = scene0; break;
			}
			case(1): {
				scene = scene1; break;
			}
			case(2): {
				scene = scene2; break;
			}
			case(3): {
				scene = scene3; break;
			}
			case(4): {
				scene = scene4; break;
			}
		}

		if (enableMouse == false) {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else if (enableMouse == true) {
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		scene->Update(m_timer.getElapsedTime());
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms. 

	} //Check if the ESC key had been pressed or if the window had been closed
	scene0->Exit();
	scene1->Exit();
	scene2->Exit();
	scene3->Exit();
	scene4->Exit();
	delete scene0;
	delete scene1;
	delete scene2;
	delete scene3;
	delete scene4;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

