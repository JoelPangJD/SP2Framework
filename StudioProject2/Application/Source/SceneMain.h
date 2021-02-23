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
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_INVENTORY,
		GEO_TEXT,
		TREE,
		RoadTileStraight,
		RoadStraightBarrier,
		RoadCross,
		RoadCrossBarrier,
		Museum,
		MBS,
		Changi,
		Lamp,
		Sprite,
		GroundMesh,
		Panel,
		Red,
		Green,
		Teacher,
		Friend,
		Header,
		Textbox,
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

	bool inFrontofMuseum, inFrontofChangi, inFrontofMarina, inFrontofGarden;
	bool minigameMuseum, minigameChangi, minigameMarina, minigameGarden;

	void RenderSkybox();
	void RenderUI();
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, int sizex, int sizey);

	void RenderMinigame();
	void RenderNPCDialogue(std::string NPCText, std::string headerText);
	void updateMinigame(double dt);

	void updateDialogue();
	std::stringstream interactText;
	bool inDialogue;
	vector<string> dialogue;
	vector<string>::iterator currentLine;
	string name;
	float cooldown;

	std::vector<InteractableObject*> items;

	Vector3* grids[9];
	Button gridButton[9];
	std::string colorGrid[9];
	bool pass;
};

#endif