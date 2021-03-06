#ifndef SCENE_MUSEUM_H
#define SCENE_MUSEUM_H
#include "Camera3.h"
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "MatrixStack.h"
#include "Entity.h"
#include <iostream>
#include <sstream>
#include <math.h>
#include "Entity.h"
#include "Terrain.h"
#include "InteractableObject.h"
#include "Inventory.h"
#include "Button.h"


class SceneMuseum : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		//BASIC GEOMETRY
		GEO_QUAD,
		GEO_CUBE,
		GEO_SPHERE,
		GEO_HEMISPHERE,
		GEO_TRUNCATEDHEMISPHERE,
		GEO_PYRAMID,
		GEO_LIGHTBALL,
		GEO_RING,
		GEO_CONE,
		GEO_CYLINDER,
		GEO_TORUS,
		//FOR SKYBOX
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_INVENTORY,
		//FOR NPC
		GEO_TEACHER,
		GEO_ANDY,
		//FOR TEXT
		GEO_TEXT,
		GEO_TEXTBOX,
		GEO_TEXTINTEXTBOX,
		GEO_HEADER,
		//BASIC OBJ FOR DECORATION
		GEO_ELEPHANT,
		GEO_STATUE1,
		GEO_STATUE2,
		GEO_PAINTING,
		GEO_PAINTING2,
		GEO_RICKSHAW,
		GEO_GLASSTABLE,
		//FOR WALLS
		GEO_WALL,
		GEO_WALLDOOR,
		GEO_WALLCURVED,
		GEO_WALLCORNER,
		//GAME 1 PHOTO
		GEO_MINIPIC1,
		GEO_MINIPIC2,
		GEO_SELECTION,
		GEO_SHOWANSWER,
		GEO_DISPLAYMINIPIC1,
		//GAME 2
		GEO_PIC,
		GEO_BOX,
		GEO_COLOR,
		GEO_ITEM1,
		GEO_KEY,
		GEO_FLAG,
		GEO_PICFLAG,
		GEO_PICKEY,
		GEO_PICBOX, 
		//GROUND MESH
		GEO_GROUND,
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
	float cooldown = 0; //cooldown time for actions

	//Animation for game2
	float TranslateBoxY = 0.0f;
	int TranslateBoxDirectional = 1;

	float movex;
	float movez;

	float TranslateFlagY = 0;
	int TranslateFlagDirectional = 1;

	float TranslateKeyY = 0;
	int TranslateKeyDirectional = 1;

	SceneMuseum();
	SceneMuseum(Inventory* inventory);
	~SceneMuseum();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Camera3 GameCam1;
	Button button;
	Button button2;
	MS modelStack, viewStack, projectionStack; //The matrix stacks
	unsigned m_parameters[U_TOTAL];
	unsigned m_vertexArrayID;
	unsigned m_programID;
	Mesh* meshList[NUM_GEOMETRY];
	Light light[2];

	//storing vectors for hitbox and items
	std::vector<InteractableObject*> items;
	std::vector<Terrain*> hitbox;

	//For buttons on exit
	float AddSize = 0;
	float MoveX = 0;
	float TotalSize = 0;
	float TotalX = 0;

	//Enable mouse
	bool MousePreview = false;

	//1st game
	bool Minigameintro = true;
	bool ShowFirstGame = false;
	void StartGame1();

	//Exit Museum
	void ExitMuseum();


	//2nd game
	void StartGame2();
	bool EndGame2 = false;

	//All introduction text
	bool RenderIntruction1 = true;
	bool RenderIntruction2= true;
	bool RenderIntruction3 = true;
	bool RenderIntruction4 = true;

	//void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderWalls();



};

#endif