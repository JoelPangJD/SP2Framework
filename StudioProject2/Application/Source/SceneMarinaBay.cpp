#include "SceneMarinaBay.h"
#include "GL\glew.h"
#include "Application.h"
#include "LoadTGA.h"
#include "Utility.h"
#include "shader.hpp"
#include <Mtx44.h>
#include"MeshBuilder.h"

SceneMarinaBay::SceneMarinaBay(Inventory* inventory)
{
	this->inventory = inventory;
}

SceneMarinaBay::~SceneMarinaBay()
{
}

void SceneMarinaBay::Init()
{
	//======Initialising Buttons========
	buttonList.push_back(new Button(0, 11, 20, 5.5, true));	//attack
	buttonList.push_back(new Button(0, 5.5, 20, 5.5, true));	//items
	buttonList.push_back(new Button(0, 0, 20, 5.5, true));	//run
	buttonList.push_back(new Button(21, 8.25, 30, 8.25));	//attack1
	buttonList.push_back(new Button(21, 0, 30, 8.25));	//attack2
	buttonList.push_back(new Button(53, 8.25, 30, 8.25));	//attack3

	//======Initialising variables========
	pointerX = 2;
	pointerY = 11;
	enemyHealthPos = 20.f;
	playerHealthPos = 60.f;
	enemyHealth = playerHealth = 0;
	attackScale = 1.f;
	playerTurn = true;
	playerAttack = NO_ATTACK;
	posZ = 200;
	fightLost = false;
	//dragon animations
	movement = attack = goneDown = false, idle = true;
	move = moveAngle = timer = 0;
	idleMouth = idleHands = idleBounce = idleNeck = idleHead = 0;
	enemyAttackAngle = enemyAttackMove = 0;
	idleBreath = enemyAttackScale = 1;
	idleHandsDir = idleBounceDir = idleMouthDir = idleBreathDir = idleNeckDir = idleHeadDir = 1;
	moveBack = 71.f;
	//======Matrix stack========
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//==========================
	camera.Init(Vector3(0, 8, 0), Vector3(0, 8, 5), Vector3(0, 1, 0));
	//camera.Init(Vector3(90, 40, 240), Vector3(0, 8, 240), Vector3(0, 1, 0));
	// Enable depth test		//just so i have to scroll less
	{
		glEnable(GL_DEPTH_TEST);

		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable face culling
		glEnable(GL_CULL_FACE);

		// Set background color to dark blue
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		// Generate a default VAO for now
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);

		//=============================================================================================
		m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
		m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
		m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
		m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
		m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
		m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
		m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
		m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
		m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
		m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
		m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
		m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
		m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

		m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
		m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
		m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
		m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
		m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
		m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
		m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
		m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
		m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
		m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
		m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

		m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

		glUseProgram(m_programID);
		glUniform1i(m_parameters[U_NUMLIGHTS], 2);

		Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
			m_parameters[U_MATERIAL_DIFFUSE],
			m_parameters[U_MATERIAL_SPECULAR],
			m_parameters[U_MATERIAL_SHININESS]);

		// Get a handle for our "textColor" uniform
		m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
		//==================Initialise light0===========
		light[0].type = Light::LIGHT_POINT;
		light[0].position.Set(0, 0, 90);
		light[0].color.Set(0.24725f, 0.1995f, 0.0745f);
		light[0].power = 0;
		light[0].kC = 1.f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(45));
		light[0].cosInner = cos(Math::DegreeToRadian(30));
		light[0].exponent = 3.f;
		light[0].spotDirection.Set(0.5f, 1.f, 0.f);

		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

		//==================Initialise light1===========
		light[1].type = Light::LIGHT_DIRECTIONAL;
		light[1].position.Set(-10, 20, 0);
		light[1].color.Set(1, 1, 1);
		light[1].power = 1.3f;
		light[1].kC = 1.5f;
		light[1].kL = 0.01f;
		light[1].kQ = 0.001f;
		light[1].cosCutoff = cos(Math::DegreeToRadian(45));
		light[1].cosInner = cos(Math::DegreeToRadian(30));
		light[1].exponent = 3.f;
		light[1].spotDirection.Set(0.f, 1.f, 0.f);

		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
		glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
		glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

		meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
		meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.0f);

		meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1.f, 1.f, 1.0f), 1);
		meshList[GEO_CUBE]->material.kAmbient.Set(0.921f, 0.808f, 0.616f);
		meshList[GEO_CUBE]->material.kDiffuse.Set(0.921f, 0.808f, 0.616f);
		meshList[GEO_CUBE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CUBE]->material.kShininess = 0.4f;

		meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", Color(1.0, 1.0, 1.0), 10, 10);
		meshList[GEO_SPHERE]->material.kAmbient.Set(0.917f, 0.835f, 0.635f);
		meshList[GEO_SPHERE]->material.kDiffuse.Set(0.917f, 0.835f, 0.635f);
		meshList[GEO_SPHERE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SPHERE]->material.kShininess = 0.4f;
		meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Lightball", Color(1, 1, 1), 10, 10, 10);

		meshList[GEO_SLOPE] = MeshBuilder::GenerateTriangle("slope", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_SLOPE]->material.kAmbient.Set(0.886f, 0.788f, 0.569f);
		meshList[GEO_SLOPE]->material.kDiffuse.Set(0.886f, 0.788f, 0.569f);
		meshList[GEO_SLOPE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_SLOPE]->material.kShininess = 0.4f;

		meshList[GEO_TRIANGLE] = MeshBuilder::Generate2dTriangle("2d triangle", Color(1, 1, 1), 1, 1);
		meshList[GEO_TRIANGLE]->material.kAmbient.Set(0.886f, 0.788f, 0.569f);
		meshList[GEO_TRIANGLE]->material.kDiffuse.Set(0.886f, 0.788f, 0.569f);
		meshList[GEO_TRIANGLE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_TRIANGLE]->material.kShininess = 0.4f;

		meshList[GEO_TRIANGLEBACK] = MeshBuilder::Generate2dTriangle("2d triangle back", Color(1, 1, 1), 1, 1, true);
		meshList[GEO_TRIANGLEBACK]->material.kAmbient.Set(0.886f, 0.788f, 0.569f);
		meshList[GEO_TRIANGLEBACK]->material.kDiffuse.Set(0.886f, 0.788f, 0.569f);
		meshList[GEO_TRIANGLEBACK]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_TRIANGLEBACK]->material.kShininess = 0.4f;

		meshList[GEO_CRESCENT] = MeshBuilder::GenerateCrescent("crescent", Color(1, 1, 1), 1);

		meshList[GEO_FRUSTUM] = MeshBuilder::GenerateFrustum("frustum", Color(1, 1, 1), 1, 1, 0.65, 1, 1);
		meshList[GEO_FRUSTUM]->material.kAmbient.Set(0.917f, 0.835f, 0.635f);
		meshList[GEO_FRUSTUM]->material.kDiffuse.Set(0.917f, 0.835f, 0.635f);
		meshList[GEO_FRUSTUM]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_FRUSTUM]->material.kShininess = 0.4f;

		meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(1, 1, 1), 1, 36, 1, 1);
		meshList[GEO_CYLINDER]->material.kAmbient.Set(0.278f, 0.345f, 0.212f);
		meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.278f, 0.345f, 0.212f);
		meshList[GEO_CYLINDER]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_CYLINDER]->material.kShininess = 0.4f;

		meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(1, 1, 1), 1, 20);
		meshList[GEO_CONE]->material.kAmbient.Set(0.647f, 0.525f, 0.462f);
		meshList[GEO_CONE]->material.kDiffuse.Set(0.647f, 0.525f, 0.462f);
		meshList[GEO_CONE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_CONE]->material.kShininess = 0.1f;

		meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("hemisphere", Color(1, 1, 1), 18, 36, 1);
		meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.278f, 0.345f, 0.212f);
		meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.278f, 0.345f, 0.212f);
		meshList[GEO_HEMISPHERE]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);
		meshList[GEO_HEMISPHERE]->material.kShininess = 0.4f;

		meshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("Testing", Color(1, 1, 1), 1.0f);
		meshList[GEO_INVENTORY]->textureID = LoadTGA("Image//inventory.tga");


		//Skybox quads
		meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.0f);
		meshList[GEO_LEFT]->textureID = LoadTGA("Image//Marina//negx.tga");
		meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.0f);
		meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Marina//posx.tga");
		meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.0f);
		meshList[GEO_FRONT]->textureID = LoadTGA("Image//Marina//posz.tga");
		meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.0f);
		meshList[GEO_BACK]->textureID = LoadTGA("Image//Marina//negz.tga");
		meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.0f);
		meshList[GEO_TOP]->textureID = LoadTGA("Image//Marina//posy.tga");
		meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.0f);
		meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Marina//negy.tga");
		//main boat
		meshList[GEO_BOAT] = MeshBuilder::GenerateOBJMTL("boat", "OBJ//Marina//boat2.obj", "OBJ//Marina//boat.mtl");

		//NPCs
		meshList[GEO_MC] = MeshBuilder::GenerateOBJMTL("MC", "OBJ//Marina//character.obj", "OBJ//Marina//defaultCharacter.mtl");
		meshList[GEO_MC]->textureID = LoadTGA("Image//Marina//mc.tga");
		meshList[GEO_ARM] = MeshBuilder::GenerateOBJMTL("MC", "OBJ//Marina//character_arm.obj", "OBJ//Marina//defaultCharacter.mtl");
		meshList[GEO_ARM]->textureID = LoadTGA("Image//Marina//mc.tga");
		meshList[GEO_ROBOT] = MeshBuilder::GenerateOBJMTL("Robot", "OBJ//Marina//defaultCharacter.obj", "OBJ//Marina//defaultCharacter.mtl");
		meshList[GEO_ROBOT]->textureID = LoadTGA("Image//Marina//robot.tga");
		meshList[GEO_ADVENTURER] = MeshBuilder::GenerateOBJMTL("Robot", "OBJ//Marina//defaultCharacter.obj", "OBJ//Marina//defaultCharacter.mtl");
		meshList[GEO_ADVENTURER]->textureID = LoadTGA("Image//Marina//adventurer.tga");
		meshList[GEO_GIRL] = MeshBuilder::GenerateOBJMTL("Robot", "OBJ//Marina//defaultCharacter.obj", "OBJ//Marina//defaultCharacter.mtl");
		meshList[GEO_GIRL]->textureID = LoadTGA("Image//Marina//girl.tga");
		meshList[GEO_ORC] = MeshBuilder::GenerateOBJMTL("Robot", "OBJ//Marina//defaultCharacter.obj", "OBJ//Marina//defaultCharacter.mtl");
		meshList[GEO_ORC]->textureID = LoadTGA("Image//Marina//orc.tga");
		meshList[GEO_BADGUY] = MeshBuilder::GenerateOBJMTL("MC", "OBJ//Marina//character.obj", "OBJ//Marina//defaultCharacter.mtl");
		meshList[GEO_BADGUY]->textureID = LoadTGA("Image//CityCenter//teacher.tga");

		//environment
		meshList[GEO_WATER] = MeshBuilder::GenerateQuad("quad", 1.0f, 1.0f, Color(1, 1, 1), 10);
		meshList[GEO_WATER]->textureID = LoadTGA("Image//watertexture.tga");
		meshList[GEO_WATER]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_WATER]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
		meshList[GEO_WATER]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
		meshList[GEO_TREE] = MeshBuilder::GenerateOBJMTL("short tree", "OBJ//Marina//palm_tree_short.obj", "OBJ//Marina//palm_tree_short.mtl");
		meshList[GEO_WATER]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_TALLTREE] = MeshBuilder::GenerateOBJMTL("tree", "OBJ//Marina//big_tree.obj", "OBJ//Marina//big_tree.mtl");
		meshList[GEO_CHAIR] = MeshBuilder::GenerateOBJMTL("chair", "OBJ//Marina//modifiedchair.obj", "OBJ//Marina//modifiedchair.mtl");

		//fight
		meshList[GEO_LAYOUT] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
		meshList[GEO_LAYOUT]->textureID = LoadTGA("Image//Marina//fight_layout.tga");
		meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
		meshList[GEO_TEXTBOX]->textureID = LoadTGA("Image//Marina//textbox.tga");
		meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("quad", Color(0, 1, 0), 1.f);
		meshList[GEO_LOSTHEALTH] = MeshBuilder::GenerateQuad("quad", Color(1, 0, 0), 1.f);
		meshList[GEO_SWORD] = MeshBuilder::GenerateOBJMTL("sword", "OBJ//Marina//Short Sword.obj", "OBJ//Marina//Short Sword.mtl");
		meshList[GEO_SWORD]->textureID = LoadTGA("Image//Marina//Short Sword.tga");

		//text
		meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
		meshList[GEO_TEXT]->textureID = LoadTGA("Image//Marina//ExportedFont.tga");
		meshList[GEO_HEADER] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
		meshList[GEO_HEADER]->textureID = LoadTGA("Image//Marina//header.tga");

		//terrain
		terrains.push_back(new Terrain(Vector3(-64.5, 0, 25), 0, 1, 5, 5, 440, "boat side wall"));
		terrains.push_back(new Terrain(Vector3(64.5, 0, 25), 0, 1, 5, 5, 440, "boat side wall"));
		terrains.push_back(new Terrain(Vector3(0, 0, -194), 0, 1, 5, 125, 5, "boat back wall"));
		for (int i = 0; i < 13; ++i)	//curved hull
		{
			terrains.push_back(new Terrain(Vector3(63 - i * 3, 0, 249 + i * 4), 0, 1, 5, 5, 10, "boat hull wall"));
			terrains.push_back(new Terrain(Vector3(-63 + i * 3, 0, 249 + i * 4), 0, 1, 5, 5, 10, "boat hull wall"));
		}
		for (int i = 0; i < 9; ++i)		//curved front part of hull
		{
			terrains.push_back(new Terrain(Vector3(24 - i * 3, 0, 299 + i * 1.5), 0, 1, 5, 5, 10, "boat hull front"));
			terrains.push_back(new Terrain(Vector3(-24 + i * 3, 0, 299 + i * 1.5), 0, 1, 5, 5, 10, "boat hull front"));
		}
		terrains.push_back(new Terrain(Vector3(0, 0, 115), 0, 1, 10, 92, 140, "captain's space"));
		terrains.push_back(new Terrain(Vector3(47, 0, 0), 0, 1, 10, 30, 320, "infinity pool"));
		terrains.push_back(new Terrain(Vector3(27, 0, -70), 0, 1, 10, 20, 180, "infinity pool"));

		//items/npcs
		items.push_back(new InteractableObject(Vector3(-50, 5, -100), 90, 0.7, 7, "robot", "Robot", false));
		items.push_back(new InteractableObject(Vector3(-22, 5, -160), 180, 0.7, 7, "girl", "Hostess", false));
		items.push_back(new InteractableObject(Vector3(0, 5, 300), 0, 0.7, 5, "badguy", "Thief", false));
		items.push_back(new InteractableObject(Vector3(-30, 5, 44), 180, 0.7, 7, "adventurer", "Adventurer", false));
		items.push_back(new InteractableObject(Vector3(15, 5, -70), 270, 0.7, 7, "orc", "Orc", false));
		items.push_back(new InteractableObject(Vector3(32, 5, 30), 0, 0.7, 5, "pool", "Infinity Pool", false));
		items.push_back(new InteractableObject(Vector3(50, 5, -160), 0, 0.7, 7, "pool", "Infinity Pool", false));
	}

}



void SceneMarinaBay::Update(double dt)
{
	fps = 1.f / dt;
	if (!fight)	//not in fight
	{
		if ((!firstEnter || camera.position.z<200) && !fightLost)	//dramatic first entry of bad guy not triggered yet
			this->Scene::movement(camera, terrains, dt);
		string trigger = this->interact(camera, items, true);
		if (trigger == "battleStart" && !fightLost && !fightOver)
		{
			fight = true;
			fightInit = true;
		}
	}
	else
	{
		int count = 0;
		for (auto it = buttonList.begin(); it != buttonList.end(); ++it)
		{
			if ((*it)->active)
			{
				(*it)->updateButton();
				if ((*it)->isClickedOn())
				{
					playerAction = static_cast<ACTION_TYPE>(count);	//makes player action = the button number
					actionSelected = true;
					break;
				}
			}
			++count;
		}
	}
	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	else if (Application::IsKeyPressed('2'))
 		glDisable(GL_CULL_FACE);
	else if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	else if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	if (Application::IsKeyPressed('Z'))
		lighton = false;						//to test whether colours and stuff are working properly
	else if (Application::IsKeyPressed('X'))
		lighton = true;
	if (Application::IsKeyPressed('I'))	//debug buttons
		x -= 10 * dt;
	if (Application::IsKeyPressed('K'))
		x += 10 * dt;
	if (Application::IsKeyPressed('J'))
		z += 10 * dt;
	if (Application::IsKeyPressed('L'))
		z -= 10 * dt;
	if (Application::IsKeyPressed('O'))
		scale += 10*dt;
	if (Application::IsKeyPressed('P'))
		scale -= 10 * dt;
	if (Application::IsKeyPressed('H'))	//test for attack button
	{
		attackSelected = true;
		playerAction = A_ATTACK3;
	}
	else if (Application::IsKeyPressed('G'))	//test for dragon attack
	{
		enemyTurn = true;
		playerTurn = false;
		enemyAttack = BITE;
	}
	if (Application::IsKeyPressed('U'))
		fight = false;
	else if (Application::IsKeyPressed('Y') && cooldown<=0)
	{
		fight = true;
		fightInit = true;
		attacksList.push_back(BIG);
		attacksList.push_back(ROCKET_PUNCH);
		attacksList.push_back(MIND_POWERS);
		cooldown = 1.f;
	}
	else if (Application::IsKeyPressed('N'))
		hitboxshow = true;
	else if (Application::IsKeyPressed('M'))
		hitboxshow = false;
	else if (Application::IsKeyPressed('E'))
	{
		if (firstEnter && camera.position.z >= posZ)
			firstEnter = false;
		else if (fightIntro && fight && cooldown<=0)
			fightIntro = false;
		else if (fightLost)		//resetting fight vars
		{
			fightLost = false;
			fightOver = false;
			enemyAttack = SPEAR;
			enemyHealth = playerHealth = 0;
			enemyHealthLost = playerHealthLost = 0;
			enemyHealthPos = 20.f;
			playerHealthPos = 60.f;
			attackScale = 1.f;
			playerTurn = true;
			enemyTurn = false;
			playerAttack = NO_ATTACK;
			Application::enableMouse = false;
			movement = attack = goneDown = false, idle = true;
			move = moveAngle = timer = 0;
			idleMouth = idleHands = idleBounce = idleNeck = idleHead = 0;
			enemyAttackAngle = enemyAttackMove = 0;
			idleBreath = enemyAttackScale = 1;
			idleHandsDir = idleBounceDir = idleMouthDir = idleBreathDir = idleNeckDir = idleHeadDir = 1;
			moveBack = 71.f;
			for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
			{
				if ((*it)->gettype() == "badguy3")
					(*it)->settype("badguy2");
			}
			for (unsigned int i = 0; i < A_RUN + 1; ++i)
			{
				buttonList[i]->active = true;
			}
		}
		else if (fightWon)
		{
			Application::enableMouse = true;
			fightWon = false;
			for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
			{
				if ((*it)->gettype() == "badguy2")
				{
					items.erase(items.begin() + distance(items.begin(), it));
					break;
				}
			}
			//render the gameWin menu here
		}
		else if (triedToRun)
		{
			playerTurn = false;
			enemyTurn = true;
			triedToRun = false;
			fightSelected = false;
		}
	}

	if (fight)
	{
		if (fightInit)
		{
			prevCam = camera;
			camera.Init(Vector3(90, 40, 240), Vector3(0, 8, 240), Vector3(0, 1, 0));
			Application::enableMouse = true;
			fightIntro = true;
			cooldown = 0.5f;
			fightInit = false;
		}


		if (actionSelected && fight && !attackSelected)	//if action was selected and in fight and attack is not playing
		{
			switch (playerAction)
			{
			case (A_ATTACK1):									//temporary solution
				attackSelected = true;
				fightSelected = false;
				break;
			case (A_ATTACK2):
				attackSelected = true;
				fightSelected = false;
				break;
			case (A_ATTACK3):
				attackSelected = true;
				fightSelected = false;
				break;
			case (A_ATTACK):
				fightSelected = true;
				//enables the buttons after fight is selected depending on what has been unlocked
				for (unsigned int i = 0; i < attacksList.size(); ++i)
				{
					buttonList[i + A_ATTACK1]->active = true;
				}
				break;
			case (A_ITEMS):			//if no time will just get rid of
				itemsSelected = true;
				break;
			case (A_RUN):
				triedToRun = true;
				break;
			default:
				cout << "playeraction broke";
			}
			actionSelected = false;
			if (playerAction > A_ITEMS)	//if doing an action that would end the turn
			{
				for (unsigned int i = 0; i < buttonList.size(); ++i)
				{
					buttonList[i]->active = false;
				}
			}
		}

		//player attack animations
		if (attackSelected && playerTurn)
		{	//gets attack player is trying to execute
			playerAttack = attacksList[playerAction - A_ATTACK1];
			switch (playerAttack)
			{
			case (BIG):			//MC goes big
				if (!attackHit)	//when attack hasnt hit enemy yet
				{
					if (attackScale < 8)		//go big
						attackScale += 7 * dt;
					else if (attackAngle < 90)		//rotate onto enemy
						attackAngle += 600 * dt;
					else
						attackHit = true;		//has hit enemy
				}
				else
				{
					if (attackAngle > 0)
						attackAngle -= 600 * dt;
					else if (attackAngle < 0)	//so that the rotation doesnt go too far back
						attackAngle = 0.f;
					else if (attackScale > 1.f)
						attackScale -= 7 * dt;
					else			//resolution of attack
					{
						enemyHealthLost = 35.f;
						attackHit = false;
						playerTurn = false;
						enemyTurn = true;
						attackSelected = false;
						playerAttack = NO_ATTACK;
					}
				}
				break;
			case (ROCKET_PUNCH):
				if (!attackHit)
				{
					if (attackTranslateY < 30)
						attackTranslateY += 30 * dt;	//goes up so it gets a good angle to hit 
					else if (attackAngle < 90)
						attackAngle += 100 * dt;
					else if (attackTranslateZ < 150)
						attackTranslateZ += 300 * dt;	//goes fast towards enemy
					else
					{
						attackHit = true;
						attackTranslateZ = 0.f;
						attackTranslateY = 300.f;		//spawns it high above player
						attackAngle = 0.f;
					}
				}
				else
				{
					if (attackTranslateY > 0)
						attackTranslateY -= 150 * dt;	//rapidly descends towards player
					else
					{
						attackTranslateY = 0.f;			//resets value of arm and resolves attack
						enemyHealthLost = 25.f;
						attackHit = false;
						playerTurn = false;
						enemyTurn = true;
						attackSelected = false;
						playerAttack = NO_ATTACK;
					}
				}
				break;
			case (MIND_POWERS):
				if (!attackHit)
				{
					if (attackTranslateY < 30 && attackTranslateZ <= 0)
					{
						attackTranslateY += 30 * dt;
					}
					else if (attackTranslateZ < 100)
					{
						int attackSpeed = 400;
						attackTranslateZ += attackSpeed * dt;
						attackTranslateY -= attackSpeed * 0.25 * dt;
					}
					else
					{
						attackHit = true;
						enemyHealthLost = 35.f;
					}
				}
				else
				{
					attackTranslateY = 0;
					attackTranslateZ = 0;
					attackHit = false;
					playerTurn = false;
					enemyTurn = true;
					attackSelected = false;
					playerAttack = NO_ATTACK;
				}
			}
		}
		//enemy turn actions
		else if (enemyTurn && enemyHealthLost <= 0 && playerHealthLost <= 0)	//if health not still decreasing
		{
			if (!enemyAlrAttacked)
			{
				switch (enemyAttack)	//handles enemy attacks
				{
				case (SPEAR):
					if (!enemyAnimPlaying)	//check to make sure animation isn't already playing
					{
						idle = false;
						attack = true;
						enemyAnimPlaying = true;
					}
					if (enemyAttackHit)
					{
						playerHealthLost = 20.f;
						enemyAlrAttacked = true;
					}
					break;
				case (DIG):			//dig attack requires more work 
					if (!enemyAnimPlaying)
					{
						idle = false;
						movement = true;
						enemyAnimPlaying = true;
					}
					if (enemyAttackHit)
					{
						playerHealthLost = 40.f;
						enemyAlrAttacked = true;
					}
					break;
				case (BITE):
					if (!enemyAnimPlaying)
					{
						idle = false;
						bite = true;
						enemyAnimPlaying = true;
					}
					if (enemyAttackHit)
					{
						playerHealthLost = 35.f;
						enemyAlrAttacked = true;
					}
					break;
				}
			}
			else if (enemyAttackHit && playerHealthLost <= 0)	//ends enemy's turn and switches enemy's next attack
			{
				enemyAttack = static_cast<ENEMY_ATTACKS>((enemyAttack + 1) % NUM_EATTACKS);	//moves to the next attack
				attackSelected = false;
				playerTurn = true;
				enemyTurn = false;
				enemyAttackHit = false;
				enemyAlrAttacked = false;
				enemyAnimPlaying = false;
				for (unsigned int i = 0; i < A_RUN + 1; ++i)	//reenabling the 3 main buttons
				{
					buttonList[i]->active = true;
				}
			}
		}

		//dragon animations
		{
			if (idle == true)
			{
				idleBounce += idleBounceDir * 10 * dt;
				idleBreath += idleBreathDir * 0.1 * dt;
				idleHands += idleHandsDir * 20 * dt;
				idleMouth += idleMouthDir * 10 * dt;
				idleNeck += idleNeckDir * 0.1 * dt;
				idleHead += idleHeadDir * 10 * dt;

				if (idleBounce > 10)
					idleBounceDir = -1;
				else if (idleBounce < 0)
					idleBounceDir = 1;

				if (idleBreath > 1.1)
					idleBreathDir = -1;
				else if (idleBreath < 1)
					idleBreathDir = 1;

				if (idleHands > 20)
					idleHandsDir = -1;
				else if (idleHands < 0)
					idleHandsDir = 1;

				if (idleMouth > 10)
					idleMouthDir = -1;
				else if (idleMouth < 0)
					idleMouthDir = 1;

				if (idleNeck > 0)
					idleNeckDir = -1;
				else if (idleNeck < -0.1)
					idleNeckDir = 1;

				if (idleHead > 10)
					idleHeadDir = -1;
				else if (idleHead < 0)
					idleHeadDir = 1;
			}
			else if (movement == true)
			{
				if (goneDown == false)
				{
					if (move > -100)
					{
						move -= 50 * dt;
						moveAngle += 45 * dt;
					}
					else
					{
						goneDown = true;
						moveAngle = 45;
					}
				}
				else	//after it has gone down
				{
					if (timer < 15)	//timer before going up
					{
						timer += 10 * dt;
					}
					else if (move < 0)
						move += 50 * dt;
					else if (moveAngle > 0)
					{
						enemyAttackHit = true;
						moveAngle -= 70 * dt;
						if (moveBack > 0)
							moveBack -= 100 * dt;
					}
					else
					{
						movement = false;
						goneDown = false;
						idle = true;
						timer = moveAngle = move = 0;
						moveBack = 71.f;
					}
				}
			}
			else if (attack == true)
			{
				if (revert == true)	//goes to initial position
				{
					enemyAttackAngle -= 20 * dt;
					enemyAttackMove -= 20 * dt;
					enemyAttackScale = 1;
					if (enemyAttackAngle <= 0 && enemyAttackMove <= 0)
					{
						attack = false;
						revert = false;
						idle = true;
					}
				}
				else {
					if (enemyAttackMove < 45)	//leans forward
					{
						enemyAttackAngle -= 50 * dt;
						enemyAttackMove += 50 * dt;
					}
					else if (enemyAttackAngle < 45)	//sweeps
					{
						enemyAttackAngle += 500 * dt;
						enemyAttackScale += 200 * dt;
					}
					else
					{
						revert = true;
						enemyAttackHit = true;
					}
				}
			}
			else if (bite == true)
			{
				if (biteRearedBack == false)	//rearing back
				{
					if (enemyAttackMove > -30)	//leans back
					{
						enemyAttackMove -= 30 * dt;
					}
					else
						biteRearedBack = true;
				}
				else if (revert == false)			//rapidly lunges forward
				{
					if (enemyAttackMove < 70)
					{
						enemyAttackMove += 300 * dt;
						enemyAttackAngle += 70 * dt;
					}
					else
					{
						revert = true;
					}
				}
				else							//moving back to original position
				{
					if (enemyAttackAngle > -2)		//chomps jaw down
					{
						enemyAttackAngle -= 175 * dt;
					}
					else if (enemyAttackMove > 0)	//moves back to original position
					{

						enemyAttackMove -= 60 * dt;
					}
					else					//animation finished
					{
						enemyAttackHit = true;
						bite = false;
						revert = false;
						biteRearedBack = false;
						idle = true;
					}
				}
			}
		}

		if (playerHealthLost > 0)
		{	//health total will be as if it is equal to 100 so since the scaling of health is 20 the speed would be the same as health lost / 5
			int speedOfHealthLost = 20.f;
			playerHealthLost -= speedOfHealthLost * dt;
			playerHealth += speedOfHealthLost * 0.2 * dt;
			playerHealthPos += speedOfHealthLost * 0.1 * dt;	//dependent on health's scaling
		}
		else if (enemyHealthLost > 0)
		{
			int speedOfHealthLost = 20.f;
			enemyHealthLost -= speedOfHealthLost * dt;
			enemyHealth += speedOfHealthLost * 0.2 * dt;
			enemyHealthPos -= speedOfHealthLost * 0.1 * dt;	//dependent on health's scaling
		}
		
		//fight resolutions
		if (playerHealth >= 20 || enemyHealth >= 20)
		{
			fightOver = true;
			fight = false;
			for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
			{
				if ((*it)->gettype() == "badguy3")
					(*it)->settype("badguy2");
			}

			if (enemyHealth >= 20)
			{
				fightWon = true;
				camera = prevCam;
			}
			else
			{
				fightLost = true;	//spawns player away from boss
				camera.Init(Vector3(-57, 8, 157), Vector3(-57, 8, 158), Vector3(0, 1, 0));
			}
		}
	}

	for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
	{
		if (!bigAdded && (*it)->gettype() == "girl2")	//adds big attack
		{
			attacksList.push_back(BIG);
			inventory->additem(new InteractableObject(Vector3(0, 0, 0), 0, 0, 0, "Mushroom", "Mushroom", true));
			bigAdded = true;
		}
		else if (!punchAdded && (*it)->gettype() == "robot2")	//adds punch attack
		{
			attacksList.push_back(ROCKET_PUNCH);
			inventory->additem(new InteractableObject(Vector3(0, 0, 0), 0, 0, 0, "Robot Arm", "Robot Arm", true));
			punchAdded = true;
		}
		else if (!mindAdded && (*it)->gettype() == "orc3")	//adds mind swords attack
		{
			attacksList.push_back(MIND_POWERS);
			mindAdded = true;
		}
		if (attacksList.size() > 0 && (*it)->gettype() == "badguy")
			(*it)->updatedialogue("badguy2");
		else if (fightLost && (*it)->gettype() == "badguy3")
			(*it)->settype("badguy2");	//changes type but not dialogue so players won't have to sit through everything
	}

	//==================Updating timers===========
	if (cooldown > 0)
		cooldown -= dt;
}

void SceneMarinaBay::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -499);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 499);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -499, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 499, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(499, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-499, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneMarinaBay::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	//====For Matrix stack===
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y,
		camera.position.z, camera.target.x, camera.target.y,
		camera.target.z, camera.up.x, camera.up.y, camera.up.z
	);
	//LIGHT====================================================
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	//LIGHT1====================================================
	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	//Skybox
	RenderSkybox();

	//========================================================

	RenderMesh(meshList[GEO_AXES], false, modelStack, viewStack, projectionStack, m_parameters);
	
	//infinity pool (to be changed to fix the need for face culling, maybe)
	modelStack.PushMatrix();
	modelStack.Translate(47, 0.0001, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(30, 317, 1);
	//glDisable(GL_CULL_FACE);
	RenderMesh(meshList[GEO_WATER], true, modelStack, viewStack, projectionStack, m_parameters);
	//glEnable(GL_CULL_FACE);
	modelStack.PopMatrix();

	//boat
	modelStack.PushMatrix();
	modelStack.Translate(0, -27.7, 0);
	modelStack.Scale(40, 25, 50);
	RenderMesh(meshList[GEO_BOAT], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	//renders all the items/npcs
	for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
	{
		modelStack.PushMatrix();						//minus 5 to make the hitbox line up properly
		modelStack.Translate((*it)->getposition().x, (*it)->getposition().y - 5, (*it)->getposition().z);
		modelStack.Rotate((*it)->getangle(), 0, 1, 0);
		modelStack.Scale((*it)->getscale(), (*it)->getscale(), (*it)->getscale());
		if ((*it)->gettype() == "robot" || (*it)->gettype() == "robot2")
			RenderMesh(meshList[GEO_ROBOT], true, modelStack, viewStack, projectionStack, m_parameters);
		else if ((*it)->gettype() == "girl" || (*it)->gettype() == "girl2")
			RenderMesh(meshList[GEO_GIRL], true, modelStack, viewStack, projectionStack, m_parameters);
		else if ((*it)->gettype() == "orc" || (*it)->gettype() == "orc2" || (*it)->gettype() == "orc3")
			RenderMesh(meshList[GEO_ORC], true, modelStack, viewStack, projectionStack, m_parameters);
		else if ((*it)->gettype() == "adventurer" || (*it)->gettype() == "adventurer2" || (*it)->gettype() == "adventurer3")
			RenderMesh(meshList[GEO_ADVENTURER], true, modelStack, viewStack, projectionStack, m_parameters);
		else if ((*it)->gettype()=="badguy" || (*it)->gettype()=="badguy2" || (*it)->gettype()=="badguy3")
			RenderMesh(meshList[GEO_BADGUY], true, modelStack, viewStack, projectionStack, m_parameters);

		modelStack.PopMatrix();
		if (hitboxshow)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*it)->getposition().x, (*it)->getposition().y + 5, (*it)->getposition().z);
			modelStack.Scale((*it)->getradius(), (*it)->getradius(), (*it)->getradius());
			RenderMesh(meshList[GEO_SPHERE], false, modelStack, viewStack, projectionStack, m_parameters);
			modelStack.PopMatrix();
		}
	}
	


	if (!fight)	//no need to be rendered while in fight
	{	//infinity poolside
		for (int z = 0; z > -170; z -= 30)
		{
			//trees
			modelStack.PushMatrix();
			modelStack.Translate(23, 0, z);
			modelStack.Scale(10, 20, 10);
			RenderMesh(meshList[GEO_TREE], true, modelStack, viewStack, projectionStack, m_parameters);
			modelStack.PopMatrix();

			//all the chairs

			//chair right after tree
			modelStack.PushMatrix();
			modelStack.Translate(23, 0, z + 5);
			modelStack.Rotate(-90, 0, 1, 0);
			modelStack.Scale(0.2, 0.2, 0.2);
			RenderMesh(meshList[GEO_CHAIR], true, modelStack, viewStack, projectionStack, m_parameters);
			modelStack.PopMatrix();

			//chair in the middle of trees
			modelStack.PushMatrix();
			modelStack.Translate(23, 0, z + 15);
			modelStack.Rotate(-90, 0, 1, 0);
			modelStack.Scale(0.2, 0.2, 0.2);
			RenderMesh(meshList[GEO_CHAIR], true, modelStack, viewStack, projectionStack, m_parameters);
			modelStack.PopMatrix();

			//chair directly before the tree
			modelStack.PushMatrix();
			modelStack.Translate(23, 0, z - 5);
			modelStack.Rotate(-90, 0, 1, 0);
			modelStack.Scale(0.2, 0.2, 0.2);
			RenderMesh(meshList[GEO_CHAIR], true, modelStack, viewStack, projectionStack, m_parameters);
			modelStack.PopMatrix();
		}
		
	}
	


	if (fight)
	{
		//mc
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 200);
		modelStack.Scale(0.7f, 0.7f, 0.7f);
		{
			//mc body
			modelStack.PushMatrix();
			if (playerAttack == BIG)
			{
				modelStack.Rotate(attackAngle, 1, 0, 0);
				modelStack.Scale(attackScale, attackScale, attackScale);
			}
			RenderMesh(meshList[GEO_MC], true, modelStack, viewStack, projectionStack, m_parameters);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			if (playerAttack == BIG)
			{
				modelStack.Rotate(attackAngle, 1, 0, 0);
				modelStack.Scale(attackScale, attackScale, attackScale);
			}
			else if (playerAttack == ROCKET_PUNCH)
			{
				modelStack.Translate(0, attackTranslateY, attackTranslateZ);
				modelStack.Rotate(-attackAngle, 1, 0, 0);
			}
			RenderMesh(meshList[GEO_ARM], true, modelStack, viewStack, projectionStack, m_parameters);
			modelStack.PopMatrix();

			//sword
			if (playerAttack == MIND_POWERS)
			{
				for (int i = -10; i <= 0; i += 10)
				{
					for (int y = 5; y <= 10; y += 5)
					{
						modelStack.PushMatrix();
						modelStack.Translate(-i, y, i);	//for looped
						modelStack.Translate(0, attackTranslateY, attackTranslateZ);
						modelStack.Rotate(90, 0, 1, 0);
						modelStack.Rotate(100, 0, 0, 1);
						modelStack.Scale(2, 2, 2);
						RenderMesh(meshList[GEO_SWORD], true, modelStack, viewStack, projectionStack, m_parameters);
						modelStack.PopMatrix();
					}
				}
			}
		}
		modelStack.PopMatrix();

		//dragon
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0, 271);
			if (goneDown == true)
				modelStack.Translate(0, 0, -moveBack);
			modelStack.Rotate(90, 0, 1, 0);
			{
				//bottom body joint
				modelStack.PushMatrix();
				modelStack.Translate(0, 0, 0);
				if (idle == true)
				{
					modelStack.Rotate(idleBounce, 0, 0, 1);
				}
				else if (movement == true)
				{
					modelStack.Translate(0, move, 0);
					modelStack.Rotate(-moveAngle, 0, 0, 1);
				}
				else if (attack == true)
				{
					modelStack.Rotate(-enemyAttackMove, 0, 0, 1);
				}
				else if (bite == true)
				{
					modelStack.Rotate(-enemyAttackMove, 0, 0, 1);
				}
				modelStack.Rotate(0, 0, 0, 1);
				modelStack.Scale(10, 10, 10);
				meshList[GEO_SPHERE]->material.kAmbient.Set(0.886, 0.788, 0.569);
				meshList[GEO_SPHERE]->material.kDiffuse.Set(0.886, 0.788, 0.569);
				RenderMesh(meshList[GEO_SPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
				{
					//bottom connecting body
					{
						//stomach
						modelStack.PushMatrix();
						modelStack.Translate(-1.5, 1.5, 0);
						modelStack.Rotate(45, 0, 0, 1);
						modelStack.Scale(0.96, 4.6, 0.96);
						meshList[GEO_FRUSTUM]->material.kAmbient.Set(0.886, 0.788, 0.569);
						meshList[GEO_FRUSTUM]->material.kDiffuse.Set(0.886, 0.788, 0.569);
						RenderMesh(meshList[GEO_FRUSTUM], true, modelStack, viewStack, projectionStack, m_parameters);
						modelStack.PopMatrix();
					}

					//middle joint
					modelStack.PushMatrix();
					modelStack.Translate(-1.45, 1.4, 0);
					if (movement == true)
						modelStack.Rotate(moveAngle * 1.5, 0, 0, 1);
					modelStack.Rotate(0, 0, 0, 1);
					modelStack.Scale(0.6, 0.6, 0.6);
					RenderMesh(meshList[GEO_SPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
					{
						//middle spine
						modelStack.PushMatrix();
						modelStack.Translate(0.34, 0.8, 0);
						modelStack.Rotate(-22, 0, 0, 1);
						if (idle == true)
						{
							modelStack.Scale(idleBreath, 1, idleBreath);
						}
						modelStack.Scale(1., 2., 1.);
						meshList[GEO_CYLINDER]->material.kAmbient.Set(0.886, 0.788, 0.569);
						meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.886, 0.788, 0.569);
						RenderMesh(meshList[GEO_CYLINDER], true, modelStack, viewStack, projectionStack, m_parameters);
						modelStack.PopMatrix();


						//middle upper joint
						modelStack.PushMatrix();
						modelStack.Translate(0.7, 1.7, 0);
						if (idle == true)
						{
							modelStack.Rotate(-idleBounce, 0, 0, 1);
						}
						else if (movement == true)
							modelStack.Rotate(1.25 * moveAngle, 0, 0, 1);
						else if (bite == true)
							modelStack.Rotate(enemyAttackMove, 0, 0, 1);
						modelStack.Rotate(0, 0, 0, 1);
						RenderMesh(meshList[GEO_SPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
						{
							int arms = 1;
							for (int i = 0; i < 2; ++i)
							{
								//upper upperarm	(name'll probably be changed at some point)
								modelStack.PushMatrix();
								modelStack.Translate(0, 0, arms * 1.2);
								if (movement == true)
									modelStack.Rotate(-moveAngle, 0, 0, 1);
								modelStack.Rotate(arms * -30, 1, 0, 0);
								modelStack.Scale(0.5, 0.8, 0.5);
								RenderMesh(meshList[GEO_SPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
								{
									//lower upperarm
									modelStack.PushMatrix();
									modelStack.Translate(0, -1.5, 0);
									modelStack.Rotate(90, 1, 1, 0);
									modelStack.Scale(0.8, 0.7, 0.8);
									RenderMesh(meshList[GEO_SPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
									modelStack.PopMatrix();

									//elbow joint
									modelStack.PushMatrix();
									modelStack.Translate(0, -2.5, 0);
									if (idle == true)
									{
										modelStack.Rotate(idleHands, 0, 0, 1);
									}
									modelStack.Scale(1, 0.625, 1);
									modelStack.Scale(0.5, 0.5, 0.5);
									RenderMesh(meshList[GEO_SPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
									{
										//forearm
										modelStack.PushMatrix();
										modelStack.Translate(2, -1.0, 0);
										modelStack.Rotate(60, 0, 0, 1);
										modelStack.Scale(1.1, 2.5, 1.1);
										RenderMesh(meshList[GEO_SPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
										modelStack.PopMatrix();

										//knuckle
										modelStack.PushMatrix();
										modelStack.Translate(3.7, -2.0, 0);
										modelStack.Rotate(120, 0, 0, 1);
										modelStack.Scale(0.8, 0.8, 0.8);
										RenderMesh(meshList[GEO_SPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
										{
											//hand
											modelStack.PushMatrix();
											modelStack.Translate(-1.0, -1.0, 0);
											modelStack.Scale(2.5, 1, 2.5);
											meshList[GEO_CUBE]->material.kAmbient.Set(0.921, 0.808, 0.616);
											meshList[GEO_CUBE]->material.kDiffuse.Set(0.921, 0.808, 0.616);
											RenderMesh(meshList[GEO_CUBE], true, modelStack, viewStack, projectionStack, m_parameters);
											{
												//first finger
												modelStack.PushMatrix();
												modelStack.Translate(-.5, .0, 0);
												modelStack.Rotate(90, 0, 0, 1);
												modelStack.Scale(0.5, 0.5, 0.1);
												RenderMesh(meshList[GEO_CYLINDER], true, modelStack, viewStack, projectionStack, m_parameters);
												modelStack.PopMatrix();

												modelStack.PushMatrix();
												modelStack.Translate(-.5, .0, 0.4);
												modelStack.Rotate(90, 0, 0, 1);
												modelStack.Scale(0.5, 0.5, 0.1);
												RenderMesh(meshList[GEO_CYLINDER], true, modelStack, viewStack, projectionStack, m_parameters);
												modelStack.PopMatrix();

												modelStack.PushMatrix();
												modelStack.Translate(-.5, .0, -0.4);
												modelStack.Rotate(90, 0, 0, 1);
												modelStack.Scale(0.5, 0.5, 0.1);
												RenderMesh(meshList[GEO_CYLINDER], true, modelStack, viewStack, projectionStack, m_parameters);
												modelStack.PopMatrix();
											}
											modelStack.PopMatrix();
										}
										modelStack.PopMatrix();
									}
									modelStack.PopMatrix();
								}
								modelStack.PopMatrix();
								arms = -1;
							}

							//neck
							modelStack.PushMatrix();
							modelStack.Translate(1.57, 0.49, 0);
							if (idle == true)
								modelStack.Translate(idleNeck, idleNeck, idleNeck);
							modelStack.Rotate(-73, 0, 0, 1);
							modelStack.Scale(1., 3.5, 1.);
							RenderMesh(meshList[GEO_FRUSTUM], true, modelStack, viewStack, projectionStack, m_parameters);
							modelStack.PopMatrix();

							//neck joint
							modelStack.PushMatrix();
							modelStack.Translate(1.7, 0.5, 0);
							if (idle == true)
							{
								modelStack.Translate(idleNeck, idleNeck, idleNeck);
								modelStack.Rotate(idleHead, 0, 0, 1);
							}
							else if (movement == true)
								modelStack.Rotate(moveAngle * 1.5, 0, 0, 1);
							else if (attack == true)
							{
								modelStack.Rotate(enemyAttackMove, 0, 0, 1);
								modelStack.Rotate(enemyAttackAngle, 0, 1, 0);
							}
							modelStack.Rotate(0, 0, 0, 1);
							modelStack.Scale(.7, .7, .7);
							RenderMesh(meshList[GEO_SPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
							{
								//head
								modelStack.PushMatrix();
								modelStack.Translate(1, 0, 0);
								modelStack.Scale(1.4, 1.4, 1.4);
								{
									if (attack == true)
									{
										//spear
										modelStack.PushMatrix();
										modelStack.Translate(1.6, -1.3, 0);
										modelStack.Rotate(-125, 0, 0, 1);
										modelStack.Scale(0.1, 3.5, 0.1);
										meshList[GEO_CYLINDER]->material.kAmbient.Set(0.53, 0.36, 0.24);
										meshList[GEO_CYLINDER]->material.kDiffuse.Set(0.53, 0.36, 0.24);
										RenderMesh(meshList[GEO_CYLINDER], true, modelStack, viewStack, projectionStack, m_parameters);
										{
											//connecting cone/spear head depending
											modelStack.PushMatrix();
											modelStack.Translate(0, 0.56, 0);
											modelStack.Rotate(0, 0, 0, 1);
											modelStack.Scale(1, 0.142, 1);
											meshList[GEO_CONE]->material.kAmbient.Set(0.77, 0.78, 0.78);
											meshList[GEO_CONE]->material.kDiffuse.Set(0.77, 0.78, 0.78);
											RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);
											modelStack.PopMatrix();
										}
										modelStack.PopMatrix();
									}

									//lower jaw
									{
										modelStack.PushMatrix();
										modelStack.Translate(0, 0.1, 0);
										if (idle == true)
										{
											modelStack.Rotate(-idleMouth, 0, 0, 1);
										}
										modelStack.Rotate(-30, 0, 0, 1);
										modelStack.Scale(1, 0.6, 0.8);
										meshList[GEO_CUBE]->material.kAmbient.Set(0.835, 0.682, 0.529);
										meshList[GEO_CUBE]->material.kDiffuse.Set(0.835, 0.682, 0.529);
										RenderMesh(meshList[GEO_CUBE], true, modelStack, viewStack, projectionStack, m_parameters);
										{
											//middle lower lip
											modelStack.PushMatrix();
											modelStack.Translate(1., -0.6, 0);
											modelStack.Rotate(-20, 0, 0, 1);
											modelStack.Scale(2.5, 0.15, 0.3);
											RenderMesh(meshList[GEO_CUBE], true, modelStack, viewStack, projectionStack, m_parameters);
											{
												//tongue
												modelStack.PushMatrix();
												modelStack.Translate(0, 0.1, 0);
												modelStack.Scale(0.2, 1, 1);
												meshList[GEO_HEMISPHERE]->material.kAmbient.Set(0.380, 0.278, 0.314);
												meshList[GEO_HEMISPHERE]->material.kDiffuse.Set(0.380, 0.278, 0.314);
												RenderMesh(meshList[GEO_HEMISPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
												modelStack.PopMatrix();

												//lower teeth
												meshList[GEO_CONE]->material.kAmbient.Set(0.647, 0.525, 0.462);
												meshList[GEO_CONE]->material.kDiffuse.Set(0.647, 0.525, 0.462);
												for (int i = 0; i < 2; ++i) {
													int teeth = 1;
													if (i % 2 == 0)
														teeth = -1;

													modelStack.PushMatrix();
													modelStack.Translate(0.1, 0.4, 0.9 * teeth);
													modelStack.Scale(0.1, 0.1, 0.1);	//to make it really small
													modelStack.Scale(0.5, 10, 1);
													RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//back tooth
													modelStack.PopMatrix();

													modelStack.PushMatrix();
													modelStack.Translate(0.15, 0.4, 0.8 * teeth);
													modelStack.Scale(0.1, 0.1, 0.1);
													modelStack.Scale(0.5, 5, 1);
													RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//2nd back tooth
													modelStack.PopMatrix();

													modelStack.PushMatrix();
													modelStack.Translate(0.4, 0.4, 0.4 * teeth);
													modelStack.Scale(0.1, 0.1, 0.1);
													modelStack.Scale(0.5, 5, 1);
													RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//front tooth
													modelStack.PopMatrix();
												}
											}
											modelStack.PopMatrix();

											//left lower lip(?)
											modelStack.PushMatrix();
											modelStack.Translate(1., -0.6, 0.2);
											modelStack.Rotate(-20, 0, 0, 1);
											modelStack.Rotate(10, 0, 1, 0);
											modelStack.Scale(2.3, 0.15, 0.3);
											RenderMesh(meshList[GEO_CUBE], true, modelStack, viewStack, projectionStack, m_parameters);
											modelStack.PopMatrix();

											//right lower lip(?)
											modelStack.PushMatrix();
											modelStack.Translate(1., -0.6, -0.2);
											modelStack.Rotate(-20, 0, 0, 1);
											modelStack.Rotate(-10, 0, 1, 0);
											modelStack.Scale(2.3, 0.15, 0.3);
											RenderMesh(meshList[GEO_CUBE], true, modelStack, viewStack, projectionStack, m_parameters);
											modelStack.PopMatrix();
										}
										modelStack.PopMatrix();
									}

									//sides of lower jaw
									for (int i = 0; i < 2; ++i)
									{
										int sides = 1;
										if (i % 2 == 0)
											sides = -1;
										modelStack.PushMatrix();
										modelStack.Translate(0, 0.1, sides * 0.35);
										modelStack.Rotate(-30, 0, 0, 1);
										modelStack.Rotate(sides * 10, 0, 1, 0);
										modelStack.Scale(1, 0.6, 0.4);
										RenderMesh(meshList[GEO_CUBE], true, modelStack, viewStack, projectionStack, m_parameters);
										modelStack.PopMatrix();
									}

									//upper jaw
									{
										modelStack.PushMatrix();
										modelStack.Translate(-0.03, 0.185, 0);
										modelStack.Rotate(-35, 0, 0, 1);
										if (bite == true)
											modelStack.Rotate(enemyAttackAngle, 0, 0, 1);
										modelStack.Scale(1.3, 0.6, 1);
										meshList[GEO_CUBE]->material.kAmbient.Set(0.886, 0.788, 0.569);	//making it the top shiny colour
										meshList[GEO_CUBE]->material.kDiffuse.Set(0.886, 0.788, 0.569);
										RenderMesh(meshList[GEO_CUBE], true, modelStack, viewStack, projectionStack, m_parameters);
										{
											modelStack.PushMatrix();
											modelStack.Scale(0.769, 1, 1);	//making everything smaller
											//eyes and mouth
											{
												//eyes
												for (int i = 0; i < 2; ++i)
												{
													int eyes = 1;
													if (i % 2 == 0)
														eyes = -1;
													modelStack.PushMatrix();
													modelStack.Translate(0.1, 0, eyes * 0.57);
													modelStack.Rotate(35, 0, 0, 1);
													modelStack.Scale(0.15, 0.2, 0.15);
													meshList[GEO_SPHERE]->material.kAmbient.Set(0.984, 0.827, 0.494);
													meshList[GEO_SPHERE]->material.kDiffuse.Set(0.984, 0.827, 0.494);
													RenderMesh(meshList[GEO_SPHERE], true, modelStack, viewStack, projectionStack, m_parameters);
													modelStack.PopMatrix();
												}

												//middle upper lip
												modelStack.PushMatrix();
												modelStack.Translate(1., -0.2, 0);
												modelStack.Scale(2.7, 0.15, 0.4);
												RenderMesh(meshList[GEO_CUBE], true, modelStack, viewStack, projectionStack, m_parameters);

												//upper teeth
												{
													meshList[GEO_CONE]->material.kAmbient.Set(0.921, 0.808, 0.616);
													meshList[GEO_CONE]->material.kDiffuse.Set(0.921, 0.808, 0.616);
													for (int i = 0; i < 2; ++i)
													{
														int teeth = 1;
														if (i % 2 == 0)
															teeth = -1;
														modelStack.PushMatrix();
														modelStack.Translate(0.15, -0.72, 0.8 * teeth);
														modelStack.Rotate(180, 1, 0, 0);
														modelStack.Scale(0.1, 0.5, 0.1);
														modelStack.Scale(0.5, 5, 2);
														RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//front tooth
														modelStack.PopMatrix();

														modelStack.PushMatrix();
														modelStack.Translate(0.05, -1.1, 0.9 * teeth);
														modelStack.Rotate(180, 1, 0, 0);
														modelStack.Scale(0.1, 0.5, 0.1);
														modelStack.Scale(1, 7, 2);
														RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//large back tooth
														modelStack.PopMatrix();

														modelStack.PushMatrix();
														modelStack.Translate(0.02, -0.72, 1.05 * teeth);
														modelStack.Rotate(180, 1, 0, 0);
														modelStack.Scale(0.1, 0.5, 0.1);
														modelStack.Scale(0.7, 5, 2);
														RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//back tooth
														modelStack.PopMatrix();
													}
												}


												//upper spikes
												{
													for (int i = 0; i < 2; ++i)
													{
														int spikes = 1;
														if (i % 2 == 0)
															spikes = -1;
														modelStack.PushMatrix();
														modelStack.Translate(-0.2, -2.5, 1.3 * spikes);
														modelStack.Rotate(180, 1, 0, 0);
														modelStack.Scale(0.1, 0.5, 0.1);
														modelStack.Scale(0.5, 5, 1.15);
														RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//front bottom spike
														modelStack.PopMatrix();

														modelStack.PushMatrix();
														modelStack.Translate(-0.3, -2.5, 1.4 * spikes);
														modelStack.Rotate(180, 1, 0, 0);
														modelStack.Scale(0.1, 0.5, 0.1);
														modelStack.Scale(0.6, 5, 1.2);
														RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//front mid bottom spike
														modelStack.PopMatrix();

														modelStack.PushMatrix();
														modelStack.Translate(-0.4, -2.5, 1.55 * spikes);
														modelStack.Rotate(180, 1, 0, 0);
														modelStack.Scale(0.1, 0.5, 0.1);
														modelStack.Scale(0.6, 5, 1.25);
														RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//back mid bottom spike
														modelStack.PopMatrix();

														modelStack.PushMatrix();
														modelStack.Translate(-0.5, -2.5, 1.65 * spikes);
														modelStack.Rotate(180, 1, 0, 0);
														modelStack.Scale(0.1, 0.5, 0.1);
														modelStack.Scale(0.6, 5, 1.3);
														RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//back bottom spike
														modelStack.PopMatrix();

														modelStack.PushMatrix();
														modelStack.Translate(-0.7, -1.5, 1.55 * spikes);
														modelStack.Rotate(90, 0, 0, 1);
														modelStack.Scale(0.1, 0.5, 0.1);
														modelStack.Scale(8, 0.5, 3);
														RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//bottom back spike
														modelStack.PopMatrix();

														modelStack.PushMatrix();
														modelStack.Translate(-0.8, 3.1, 1.35 * spikes);
														modelStack.Rotate(90, 0, 0, 1);
														modelStack.Scale(0.1, 0.5, 0.1);
														modelStack.Scale(15, 1.1, 5);
														RenderMesh(meshList[GEO_CONE], true, modelStack, viewStack, projectionStack, m_parameters);	//top back spike
														modelStack.PopMatrix();
													}
												}

												modelStack.PopMatrix();

												//sides of upper lip
												for (int i = 0; i < 2; ++i)
												{
													int sides = 1;
													if (i % 2 == 0)
														sides = -1;
													modelStack.PushMatrix();
													modelStack.Translate(0.8, -0.2, sides * 0.31);
													modelStack.Rotate(sides * 10, 0, 1, 0);
													modelStack.Scale(2.7, 0.15, 0.4);
													RenderMesh(meshList[GEO_CUBE], true, modelStack, viewStack, projectionStack, m_parameters);
													modelStack.PopMatrix();
												}
											}

											//upper sloped part of face
											modelStack.PushMatrix();
											modelStack.Translate(1.4, 0.15, 0);
											modelStack.Scale(1.8, 0.6, 0.35);
											RenderMesh(meshList[GEO_SLOPE], true, modelStack, viewStack, projectionStack, m_parameters);
											modelStack.PopMatrix();

											//sides of sloped part of face
											for (int i = 0; i < 2; ++i)
											{
												int sides = 1;
												if (i % 2 == 0)
												{
													sides = -1;
												}
												modelStack.PushMatrix();
												modelStack.Translate(1.42, -0.13, sides * 0.36);
												modelStack.Rotate(-18, 0, 0, 1);
												modelStack.Rotate(sides * -35, 1, 0, 0);
												modelStack.Scale(1.8, 0.7, 1);
												if (i == 0)
													RenderMesh(meshList[GEO_TRIANGLEBACK], true, modelStack, viewStack, projectionStack, m_parameters);
												else
													RenderMesh(meshList[GEO_TRIANGLE], true, modelStack, viewStack, projectionStack, m_parameters);
												modelStack.PopMatrix();
											}
											modelStack.PopMatrix();
										}
										modelStack.PopMatrix();
									}

									//sides of upper jaw
									for (int i = 0; i < 2; ++i)
									{
										int sides = 1;
										if (i % 2 == 0)
											sides = -1;
										modelStack.PushMatrix();
										modelStack.Translate(-0.05, 0.2, sides * 0.36);
										modelStack.Rotate(-35, 0, 0, 1);
										modelStack.Rotate(sides * 10, 0, 1, 0);
										modelStack.Scale(1.3, 0.6, 0.6);
										RenderMesh(meshList[GEO_CUBE], true, modelStack, viewStack, projectionStack, m_parameters);
										modelStack.PopMatrix();
									}
								}
								modelStack.PopMatrix();
							}
							modelStack.PopMatrix();
						}
						modelStack.PopMatrix();
					}
					modelStack.PopMatrix();
				}
				modelStack.PopMatrix();

				//swipe effect
				if (enemyAttackScale > 1)
				{
					modelStack.PushMatrix();
					modelStack.Translate(23, 1, 0);
					modelStack.Rotate(45, 0, 1, 0);
					modelStack.Scale(enemyAttackScale, enemyAttackScale, enemyAttackScale);
					RenderMesh(meshList[GEO_CRESCENT], false, modelStack, viewStack, projectionStack, m_parameters);
					modelStack.PopMatrix();
				}
			}
			modelStack.PopMatrix();
		}
	}
	//terrain hitbox checking, to remove in final
	for (std::vector<Terrain*>::iterator it = terrains.begin(); it != terrains.end(); it++)
	{
		if (hitboxshow)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*it)->getposition().x + x, (*it)->getposition().y + (*it)->getheight() * 0.5, (*it)->getposition().z + z);
			modelStack.Scale((*it)->getxwidth(), (*it)->getheight(), (*it)->getzwidth());
			RenderMesh(meshList[GEO_CUBE], false, modelStack, viewStack, projectionStack, m_parameters);
			modelStack.PopMatrix();
		}
	}
	
	//text stuff
	if (fight && !attackSelected)
	{
		//enemy
		RenderTextOnScreen(meshList[GEO_TEXT], "Dragon", Color(0, 1, 0), 5, 0, 55, modelStack, viewStack, projectionStack, m_parameters);
		RenderMeshOnScreen(meshList[GEO_HEALTH], 10, 53, 20, 2, modelStack, viewStack, projectionStack, m_parameters);
		RenderMeshOnScreen(meshList[GEO_LOSTHEALTH], enemyHealthPos, 53, enemyHealth, 2, modelStack, viewStack, projectionStack, m_parameters);
		//MC
		RenderTextOnScreen(meshList[GEO_TEXT], "You", Color(0, 1, 0), 5, 72, 55, modelStack, viewStack, projectionStack, m_parameters);
		RenderMeshOnScreen(meshList[GEO_HEALTH], 70, 53, 20, 2, modelStack, viewStack, projectionStack, m_parameters);
		RenderMeshOnScreen(meshList[GEO_LOSTHEALTH], playerHealthPos, 53, playerHealth, 2, modelStack, viewStack, projectionStack, m_parameters);
		if (playerTurn)
		{
			//fighting layout
			RenderMeshOnScreen(meshList[GEO_LAYOUT], 40, 15, 80, 30, modelStack, viewStack, projectionStack, m_parameters);
			RenderTextOnScreen(meshList[GEO_TEXT], "Attack", Color(0, 0, 0), 4, 4, 11, modelStack, viewStack, projectionStack, m_parameters);
			RenderTextOnScreen(meshList[GEO_TEXT], "Items", Color(0, 0, 0), 4, 4, 6, modelStack, viewStack, projectionStack, m_parameters);
			RenderTextOnScreen(meshList[GEO_TEXT], "Run", Color(0, 0, 0), 4, 4, 1, modelStack, viewStack, projectionStack, m_parameters);
			RenderTextOnScreen(meshList[GEO_TEXT], ">", Color(0, 0, 0), 4, pointerX, pointerY, modelStack, viewStack, projectionStack, m_parameters);
			if (fightSelected)	
			{	
				string attack1 = EnumToStr(attacksList[0]);
				RenderTextOnScreen(meshList[GEO_TEXT], attack1, Color(0, 0, 0), 4, 25, 9.5, modelStack, viewStack, projectionStack, m_parameters);
				if (attacksList.size() > 1)	//checks if theres 2 attacks in the vector
				{
					string attack2 = EnumToStr(attacksList[1]);
					RenderTextOnScreen(meshList[GEO_TEXT], attack2, Color(0, 0, 0), 4, 25, 2.5, modelStack, viewStack, projectionStack, m_parameters);
				}
				if (attacksList.size() > 2)	//check for 3 attacks in vector
				{
					string attack3 = EnumToStr(attacksList[2]);
					RenderTextOnScreen(meshList[GEO_TEXT], attack3, Color(0, 0, 0), 4, 55, 9.5, modelStack, viewStack, projectionStack, m_parameters);
				}
			}
			else if (itemsSelected)
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "Placeholder3", Color(0, 0, 0), 4, 25, 8, modelStack, viewStack, projectionStack, m_parameters);
				RenderTextOnScreen(meshList[GEO_TEXT], "Placeholder4", Color(0, 0, 0), 4, 25, 8, modelStack, viewStack, projectionStack, m_parameters);
			}
		}
		/*for (auto it = buttonList.begin(); it != buttonList.end(); ++it)
			RenderMeshOnScreen(meshList[GEO_QUAD], (*it)->positionX + (*it)->width * 0.5, (*it)->positionY + (*it)->height * 0.5, (*it)->width, (*it)->height);*/
		

		/*double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x / 10;
		float posY = 60 - y / 10;
		std::ostringstream ss;
		ss << "x: " << posX;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 35, 29);
		ss.str("");
		ss << "y: " << posY;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 35, 27);*/
	}
	else if (!fight)
	{
		Scene::RenderUI(cooldown, fps, modelStack, viewStack, projectionStack, m_parameters);
	}


	if (firstEnter && camera.position.z >= posZ)
	{
		RenderNPCDialogue("So, you chased me all the way here. If you really think you can take me, then step forward but be prepared I'm not going to go down easy", "???", modelStack, viewStack, projectionStack, m_parameters);
	}
	else if (fightLost)
	{
		if (attacksList.size() < 3)
			RenderNPCDialogue("More attacks can be gained from the previous section. Also, a 2-on-1 makes it hard to run.", "Tips", modelStack, viewStack, projectionStack, m_parameters);
		else
			RenderNPCDialogue("A 2-on-1 makes it quite hard to run.", "Tips", modelStack, viewStack, projectionStack, m_parameters);
	}
	else if (fightWon)
		RenderNPCDialogue("Agh, you've killed my prized possession. It is my loss. Take your wallet back.", "Thief", modelStack, viewStack, projectionStack, m_parameters);
	else if (triedToRun)
		RenderNPCDialogue("Nice try.", "Thief", modelStack, viewStack, projectionStack, m_parameters);

	if (fightIntro)
		RenderMinigameIntro("This game is a turn-based gamemode, if your healthbar turns all red you'll lose. Similarly, if your opponent's bar turns all red they'll lose. You have access to a few options on the bottom of the screen that can be done in a turn, just click on the buttons and they will either show more actions or do an action that ends your turn.", "Turn-based fight", 4, modelStack, viewStack, projectionStack, m_parameters);
	//else if (talkIntro)

}

string SceneMarinaBay::EnumToStr(ATTACK enumToConvert)	//function to convert enums from ATTACK to string
{
	switch (enumToConvert)
	{
	case (BIG):
		return "BIG";
	case (ROCKET_PUNCH):
		return "ROCKET PUNCH";
	case (MIND_POWERS):
		return "MIND POWERS";
	}
}

void SceneMarinaBay::Exit()
{
	//clearing the vector new pointers
	for (auto it = buttonList.begin(); it != buttonList.end(); ++it)
		delete (*it);
	for (auto it = terrains.begin(); it != terrains.end(); ++it)
		delete (*it);
	for (auto it = items.begin(); it != items.end(); ++it)
		delete (*it);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
