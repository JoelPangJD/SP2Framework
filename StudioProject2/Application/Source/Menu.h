#pragma once
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Button.h"
#include "Scene.h"
#include "MeshBuilder.h"
#include "LoadOBJ.h"
#include "LoadTGA.h"

class Menu : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_MENU,
		GEO_PAUSE,
		GEO_HELP,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};
private:
	Camera3 PauseCam;
	Mesh* meshList[NUM_GEOMETRY];
	MS modelStack, viewStack, projectionStack;
	unsigned m_parameters[U_TOTAL];

	float posX, posY, width, height;

public:
	Camera3 camera;
	Menu();
	Menu(float _posX, float _posY, float _width, float _height);
	~Menu();
	void renderMenuScreen();
	void renderPause();
	void renderHelp();


};