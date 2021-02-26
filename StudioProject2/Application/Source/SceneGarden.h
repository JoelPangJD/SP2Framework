#ifndef SCENE_GARDEN_H
#define SCENE_GARDEN_H
#include "Camera3.h"
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "MatrixStack.h"
#include "Entity.h"
#include "InteractableObject.h"
#include "Terrain.h"
#include "Inventory.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <math.h>

class SceneGarden : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_CUBE,
		GEO_HEMISPHERE,
		GEO_CONE,
		GEO_PYRAMID,
		GEO_SPHERE,
		GEO_LIGHTBALL,
		GEO_TORUSGAME,
		GEO_TORUSPLAYER,

		//Skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		//Minigame
		GEO_YARNBACKGROUND,
		GEO_PLAYERYARN,
		GEO_OBJECTIVEYARN,

		//Objs
		GEO_GRASSFLOOR,
		GEO_CAT,
		GEO_YARN,
		GEO_POND,
		GEO_PONDBED,
		GEO_PATH,
		GEO_PATHT,
		GEO_STICK,
		GEO_FISH,
		GEO_GAZEBO,
		GEO_TREE1,
		GEO_TREE2,
		GEO_LILY1,
		GEO_LILY2,
		NUM_GEOMETRY,

	};

	enum MATERIALS_TYPE
	{
		M_PUPIL,
		M_FISH1,
		M_FISH2,
		M_TORUSGOOD,
		M_TORUSBAD,
		M_TORUSNEUTRAL,
		M_TOTAL
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
	bool hitboxshow = false;
	float cooldown = 0; //cooldown time for actions
	float fps;
	//Minigame variables
	int minigame = 0; //0 for no minigame, 1 minigame intro screen, 2 for minigame one, 3 for minigame 2 intro, 4 for minigame 2
	float circlescale1, circlescale2, circlespeed; //Circlescale1 is the size of the shrinking circle for the minigame1, circlescale2 is for the constant circle and circlespeed is the speed at which the circle gets smaller
	int progress = 0;
	//Minigame2 variables
	double cursorx = 0, cursory = 0;
	float playerx = 40, playery = 30;
	float objectivex, objectivey;
	float obstaclex[8] = { 50.f,30.f,60.f,20.f,40.f,40.f,-1.f,81.f };
	float obstacley[8] = { 45.f,45.f,20.f,15.f,61.f,-1.f,30.f,30.f };
	float obstaclewidth[8] = { 2.f,20.f,20.f,2.f,80.f,80.f,1.f,1.f };
	float obstacleheight[8] = { 15.f,2.f,2.f,15.f,1.f,1.f,60.f,60.f };

	SceneGarden();
	SceneGarden(Inventory* inventory);
	~SceneGarden();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Camera3 prevcamera;
	MS modelStack, viewStack, projectionStack; //The matrix stacks
	unsigned m_parameters[U_TOTAL];
	unsigned m_vertexArrayID;
	unsigned m_programID;
	Mesh* meshList[NUM_GEOMETRY];
	Light light[2];	
	Material materialList[M_TOTAL];

	float movex = 0, movez = 0;
	float scale = 1;

	std::vector<InteractableObject*> items;
	std::vector<Terrain*> terrains;

	void RenderSkybox();

	void Renderminigame1();
	void Renderminigame2();
	void Renderfish();
	bool fishright = true;
	float fishAngle = 0;
};

#endif