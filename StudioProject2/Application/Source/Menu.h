#pragma once
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Button.h"
#include "Scene.h"
#include "MeshBuilder.h"


class Menu
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
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		//Light0
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		//Light1
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		//Text
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
private:
	Camera3 PauseCam;
	Mesh* meshList[NUM_GEOMETRY];
	MS modelStack, viewStack, projectionStack;
	unsigned m_parameters[U_TOTAL];


public:
	Camera3 camera;
	Menu();
	Menu(float _posX, float _posY, float _width, float _height);
	~Menu();
	//void renderMenuScreen();
	//void renderPause();
	//void renderHelp();

	bool pause = false;

	float posX, posY, width, height;
};