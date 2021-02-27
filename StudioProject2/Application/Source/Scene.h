#include "Mesh.h"
#ifndef SCENE_H
#define SCENE_H
#include "MatrixStack.h"
#include "inventory.h"
#include "Terrain.h"
#include "InteractableObject.h"
#include "Camera3.h"
#include "Button.h"

class Scene
{
private:
	enum GEOMETRY_TYPE
	{
		//UI
		GEO_INVENTORY,
		GEO_ACTIONS,
		GEO_PRESSE,
		GEO_PAUSE,
		GEO_HELP,
		GEO_MENU,
		GEO_WIN,

		//text 
		GEO_TEXT,
		GEO_TEXTBOX,
		GEO_HEADER,
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

	bool riddleStarted = false, riddleSolved = false;	//only for MarinaBay because I can't think of a better method

protected: //for SceneMuseum 
	bool place1 = false;
	bool place2 = false;
	bool place3 = false;

public:
	Scene();
	~Scene() {}
	
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	
	Mesh* baseMeshList[NUM_GEOMETRY];

	Inventory* inventory; //Inventory is shared between all scenes

	//Variables for text
	std::ostringstream interacttext;
	bool indialogue = false;
	bool inmenu = false;
	int menutype; // 0 for main menu, 1 for pause menu, 2 for helps screen
	bool CantUse = true;
	vector<string> dialogue;
	vector<string>::iterator currentline;
	string name;
	Button button;

	//For Menu,pause and help
	bool Help = false;
	bool GameWin = false;


	//For scene museum
	bool EndGame1 = false;
	bool Preview = false;
	bool ShowAnswer = false;
	bool FoundAnswer = false;
	bool ToExit = false;

	void RenderMesh(Mesh* mesh, bool enableLight, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[]);
	void RenderUI(float &cooldown, float fps, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[]);
	void RenderText(Mesh* mesh, std::string text, Color color, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[]);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[]);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[]);
	void RenderNPCDialogue(std::string NPCText, std::string headerText, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[]);
	void RenderMinigameIntro(std::string MinigamedescriptionText, std::string MinigamenameText, float fontsize, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[]); // pass minigame name, description and description font size, try fontsize 6 
	void movement(Camera3 &camera, vector<Terrain*> terrains, double dt);
	string interact(Camera3 &camera, vector<InteractableObject*>& items, bool MarinaBay = false);	//bool is just so that the other scenes don't waste time checking
	//returns a string to trigger scene specific actions

};

#endif