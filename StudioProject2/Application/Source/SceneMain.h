#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H
#include "Camera3.h"
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "MatrixStack.h"
#include "Entity.h"
#include <iostream>
#include <sstream>
#include <math.h>
#include "Button.h"
#include "Terrain.h"
#include "InteractableObject.h"

class SceneMain : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_CUBE,
		GEO_SPHERE,
		GEO_LIGHTBALL,

		//skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		//other necessary things
		GEO_GROUNDMESH,
		GEO_INVENTORY,
		GEO_TEXT,
		GEO_HEADER,
		GEO_TEXTBOX,

		//roads
		GEO_ROADSTRAIGHT,
		GEO_ROADCROSS,
		GEO_ROADCURVESPLITRIGHT,
		GEO_ROAD4WAY,
		GEO_ROADJUNCTION,
		GEO_ROADCURVED,
		GEO_ROADINTERSECT,

		//buildings
		GEO_MUSEUM,
		GEO_MBS,
		GEO_CHANGI,
		GEO_BUILDING,
		GEO_BUILDING1,
		GEO_BUILDING2,
		GEO_BUILDING3,

		//minigame
		GEO_PANEL,
		GEO_RED,
		GEO_GREEN,

		//decoration
		GEO_TREE,
		GEO_LAMP,
		GEO_CAR,
		GEO_TEACHER,
		GEO_FRIEND,
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

public:
	Camera3 camera;
	bool lighton = true;
	float fps;

	SceneMain();
	SceneMain(Inventory* inventory);
	~SceneMain();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	MS modelStack, viewStack, projectionStack; //The matrix stacks
	unsigned m_parameters[U_TOTAL];
	unsigned m_vertexArrayID;
	unsigned m_programID;
	Mesh* meshList[NUM_GEOMETRY];
	Light light[2];

	bool minigameMuseum/*, minigameChangi, minigameMarina, minigameGarden*/;
	bool firstEnter, firstRender, walletNotGone;

	void RenderSkybox();

	void RenderMinigame();
	void updateMinigame(double dt);

	float cooldown;

	float translateCarX, translateCarZ, rotateCarY;
	void updateCar(double dt);

	std::vector<InteractableObject*> items;
	std::vector<Terrain*> wall;

	Vector3* grids[9];
	Button gridButton[9];
	std::string colorGrid[9];
	bool pass;
};

#endif