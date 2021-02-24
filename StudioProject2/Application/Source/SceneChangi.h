#ifndef SCENE_CHANGI_H
#define SCENE_CHANGI_H
#include "Camera3.h"
#include "Vector3.h"
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "MatrixStack.h"
#include "Entity.h"
#include "Terrain.h"
#include <iostream>
#include <sstream>
#include <math.h>

class SceneChangi : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
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
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_TOWER,
		GEO_AIRPORT,
		GEO_STAIRCAR,
		GEO_AMBULANCE,
		GEO_FIRETRUCK,
		GEO_POLICE,
		GEO_DOORMAN,
		GEO_MISSILE,
		GEO_STRAIGHT,
		GEO_ROADL,
		GEO_ROADSPLIT,
		GEO_ROADARROW,
		GEO_PLANE,
		GEO_INVENTORY,
		GEO_TEXT,
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
	Vector3 pos;

	float cooldown = 0;
	bool lighton = true;
	bool use;
	bool renderStairs = true;
	bool renderDoorman = true;
	bool welcome;
	bool atStairs;
	bool gameStart;
	bool gameEnd;
	bool takeFlight = false;
	float fps;

	float rotateL;
	float rotateR;
	float autoMove ;
	float movex;
	float movez;
	float camMove;
	float minimize;
		
	float scale = 1;

	SceneChangi();
	SceneChangi(Inventory* inventory);
	~SceneChangi();

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

	std::vector<Terrain*> terrains;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderGroundMesh();
	void RenderEntity();
	void RenderUI();
	void RenderRoad();
	void RenderWords();
	void RenderMissile();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);

};

#endif