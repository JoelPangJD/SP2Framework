#ifndef SCENE_MARINA_BAY_H
#define SCENE_MARINA_BAY_H
#include "Camera3.h"
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "MatrixStack.h"
#include "Entity.h"
#include "Button.h"
#include <iostream>
#include <sstream>
#include <math.h>
#include "Terrain.h"
#include "InteractableObject.h"

class SceneMarinaBay : public Scene
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
		GEO_TRIANGLE,
		GEO_TRIANGLEBACK,
		GEO_SLOPE,
		GEO_CRESCENT,
		GEO_FRUSTUM,

		//skybox
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		//NPCs
		GEO_ROBOT,
		GEO_GIRL,
		GEO_ORC,
		GEO_ADVENTURER,

		//environment
		GEO_BOAT,
		GEO_TREE,
		GEO_TALLTREE,
		GEO_WATER,
		GEO_CHAIR,

		//fight specific
		GEO_LAYOUT,
		GEO_HEALTH,
		GEO_LOSTHEALTH,
		GEO_MC,
		GEO_ARM,
		GEO_SWORD,

		//text/UI
		GEO_TEXTBOX,
		GEO_TEXT,
		GEO_HEADER,
		GEO_INVENTORY,

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
	enum ACTION_TYPE
	{
		A_ATTACK,
		A_ITEMS,
		A_RUN,
		A_ATTACK1,
		A_ATTACK2,
		A_ATTACK3,
		A_ATTACK4,
		A_ITEM1,
		A_ITEM2,
		A_ITEM3,
		A_ITEM4,
		NUM_ACTION,
	};
	enum ATTACK
	{
		BIG,
		ROCKET_PUNCH,
		MIND_POWERS,
		NO_ATTACK,
	};
	enum ENEMY_ATTACKS
	{
		SPEAR,
		DIG,
		BITE,
		NUM_EATTACKS,
	};
public:
	Camera3 camera;
	bool lighton = true;
	float fps;

	SceneMarinaBay();
	SceneMarinaBay(Inventory* inventory);
	~SceneMarinaBay();

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
	float x, z, scale=1.f;

	float cooldown = 0; //cooldown time for actions

	vector<Button*> buttonList;		//button list, button positions are the same as the action enums
	vector<ATTACK> attacksList;		//players current attacksList 
	vector<Terrain*> terrains;		//terrain list
	vector<InteractableObject*> items;	//objects/npcs list

	ACTION_TYPE playerAction, enemyAction;
	ATTACK playerAttack;
	ENEMY_ATTACKS enemyAttack;
	std::string fightText, dialogueText;	//to get rid of probably
	
	//variables for dialogue
	std::ostringstream interacttext;
	bool indialogue;
	vector<string> dialogue;
	vector<string>::iterator currentline;
	string name;
	
	//vars to check whether the attacks have already been added
	bool bigAdded = false, punchAdded = false, mindAdded = false;

	bool fight = false, fightDia, playerTurn, enemyTurn, fightSelected, itemsSelected, backSelected, attackSelected, actionSelected, attackHit;
	float cooldownTimer;
	float pointerX, pointerY, playerHealth, playerHealthPos, playerHealthLost, enemyHealth, enemyHealthPos, enemyHealthLost;
	//player's
	float attackScale, attackAngle, attackTranslateY, attackTranslateZ;
	//dragon enemy's 
	bool enemyAttackHit;
	//idle
	float idleHands, idleBounce, idleMouth, idleBreath, idleNeck, idleHead;
	int idleHandsDir, idleBounceDir, idleMouthDir, idleBreathDir, idleNeckDir, idleHeadDir;
	//spear/bite
	float enemyAttackAngle, enemyAttackMove, enemyAttackScale;
	//going down thing
	float move, moveAngle, timer, moveBack;
	//animation bools
	bool movement, goneDown, idle, attack, revert, bite, biteRearedBack;
	bool hitboxshow;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderNPCDialogue(std::string NPCText, std::string headerText);

};

#endif