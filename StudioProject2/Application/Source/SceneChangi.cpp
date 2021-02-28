#include "SceneChangi.h"
#include "GL\glew.h"
#include "Application.h"

#include "LoadTGA.h"
#include "Utility.h"
#include "shader.hpp"
#include <Mtx44.h>
#include"MeshBuilder.h"
#include <iostream>
#include <sstream>
//testing 12132132123

SceneChangi::SceneChangi()
{
}

SceneChangi::SceneChangi(Inventory* inventory)
{
	this->inventory = inventory;
}

SceneChangi::~SceneChangi()
{
}

void SceneChangi::Init()
{
	//======Matrix stack========
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//==========================
	camera.Init(Vector3(-204, 10.3, 60), Vector3(0, 4.34, 0), Vector3(0, 1, 0));
	
	// Enable depth test
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
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
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
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
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
	light[1].position.Set(10, 20, 0);
	light[1].color.Set(1, 1, 1);
	light[1].power = 0;//1.3;
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

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1), 1.0f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//Changi//changiGround.tga");

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(0.5f, 0.2f, 0.0f), 1);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", Color(0.5, 0.5, 0.5), 10, 10, 10);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SPHERE]->material.kShininess = 1.f;
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Lightball", Color(1, 1, 1), 10, 10, 10);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");

	meshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("Testing", Color(1, 1, 1), 1.0f);
	meshList[GEO_INVENTORY]->textureID = LoadTGA("Image//inventory.tga");


	//Skybox quads
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.0f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Changi//cloudBack.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.0f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Changi//cloudFront.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.0f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Changi//cloudRight.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.0f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Changi//cloudLeft.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.0f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Changi//cloudUp.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.0f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Changi//cloudDown.tga");

	//OBJ
	meshList[GEO_TOWER] = MeshBuilder::GenerateOBJMTL("tower", "OBJ//Changi//ChangiTower.obj", "OBJ//Changi//ChangiTower.mtl");

	meshList[GEO_AIRPORT] = MeshBuilder::GenerateOBJMTL("airport", "OBJ//Changi//airport.obj", "OBJ//Changi//airport.mtl");

	meshList[GEO_PLANE] = MeshBuilder::GenerateOBJMTL("plane", "OBJ//Changi//plane.obj", "OBJ//Changi//plane.mtl");

	meshList[GEO_STAIRCAR] = MeshBuilder::GenerateOBJMTL("staircar", "OBJ//Changi//stairCar.obj", "OBJ//Changi//stairCar.mtl");

	meshList[GEO_AMBULANCE] = MeshBuilder::GenerateOBJMTL("ambulance", "OBJ//Changi//ambulance.obj", "OBJ//Changi//ambulance.mtl");

	meshList[GEO_FIRETRUCK] = MeshBuilder::GenerateOBJMTL("firetruck", "OBJ//Changi//firetruck.obj", "OBJ//Changi//firetruck.mtl");

	meshList[GEO_POLICE] = MeshBuilder::GenerateOBJMTL("police", "OBJ//Changi//police.obj", "OBJ//Changi//police.mtl");

	meshList[GEO_WOMAN] = MeshBuilder::GenerateOBJ("woman", "OBJ//Marina//defaultCharacter.obj");
	meshList[GEO_WOMAN]->textureID = LoadTGA("Image//Changi//woman.tga");

	meshList[GEO_NPC] = MeshBuilder::GenerateQuad("NPC", Color(1, 1, 1), 1.0f);
	meshList[GEO_NPC]->textureID = LoadTGA("Image//Changi//womanHead.tga");

	meshList[GEO_MISSILE] = MeshBuilder::GenerateOBJMTL("missile", "OBJ//Changi//missile.obj", "OBJ//Changi//missile.mtl");
	
	//roadOBJ
	meshList[GEO_STRAIGHT] = MeshBuilder::GenerateOBJMTL("roadStraight", "OBJ//Changi//straightRoad.obj", "OBJ//Changi//straightRoad.mtl");
	meshList[GEO_ROADSPLIT] = MeshBuilder::GenerateOBJMTL("roadSplit", "OBJ//Changi//splitRoad.obj", "OBJ//Changi//splitRoad.mtl");
	meshList[GEO_ROADL] = MeshBuilder::GenerateOBJMTL("roadL", "OBJ//Changi//roadL.obj", "OBJ//Changi//roadL.mtl");
	meshList[GEO_ROADARROW] = MeshBuilder::GenerateOBJMTL("roadArrow", "OBJ//Changi//arrowRoad.obj", "OBJ//Changi//arrowRoad.mtl");

	items.push_back(new InteractableObject(Vector3(-100, 0, 60), 0, 2, 20, "Guide", "Guide", false));
	items.push_back(new InteractableObject(Vector3(-40, 10.3, 72), 0, 2, 30, "stairs", "Stairs", false));

	terrains.push_back(new Terrain(Vector3(-84.8, 10.3, 115), 0, 0, 0, 8, 70, "Wall"));
	terrains.push_back(new Terrain(Vector3(-84.8, 10.3, -115), 0, 0, 0, 8, 70, "Wall"));
	terrains.push_back(new Terrain(Vector3(4, 7, -150), 0, 0, 0, 200, 20, "Wall"));
	terrains.push_back(new Terrain(Vector3(4, 7, 140), 0, 0, 0, 200, 15, "Wall"));
	terrains.push_back(new Terrain(Vector3(100, 10.3, 0), 0, 0, 0, 20, 300, "Wall"));
	terrains.push_back(new Terrain(Vector3(-40, 10.3, 33), 0, 0, 0, 30, 62, "stairs"));
	terrains.push_back(new Terrain(Vector3(10.3, 10.3, 398), 0, 0, 0, 70, 35, "police"));
	terrains.push_back(new Terrain(Vector3(10.3, 10.3, 347), 0, 0, 0, 80, 38, "ambulance"));
	terrains.push_back(new Terrain(Vector3(10.3, 10.3, 294), 0, 0, 0, 75, 38, "firetruck"));

	collide = false;
}



void SceneChangi::Update(double dt)
{
	fps = 1.f / dt;
	if (cooldown > 0)
		cooldown -= dt;

	movement(camera, terrains, dt);
	interact(camera, items);

	if (Application::IsKeyPressed('5'))
	{
		light[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		//to do: switch light type to DIRECTIONAL and pass the information to shader
		light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
		//to do: switch light type to SPOT and pass the information to shader
		light[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	if (Application::IsKeyPressed('Z'))
		lighton = false;						//to test whether colours and stuff are working properly
	else if (Application::IsKeyPressed('X'))
		lighton = true;

	if (camera.position.x <= -30 && camera.position.x >= -50 && camera.position.z > 60 && camera.position.z < 90) {
		atStairs = true;
		if (Application::IsKeyPressed('Q'))
		{
			
			use = true;
			renderStairs = false;
			renderDoorman = false;

		}
	}
	else
	{
		atStairs = false;
	    use = false;
	}

	if (camera.position.x == -18 && camera.position.y == 51 && camera.position.z == 0) {
		gameStart = true;

	}
	else
	{
		gameStart = false;
	}

	if (camera.position.x >= -115 && camera.position.x <= -98 && camera.position.y >= 0 && camera.position.y <= 13 && camera.position.z >= 50 && camera.position.z <= 65) {
		welcome = true;
	}
	else
	{
		welcome = false;
	}

	if (camera.position.x == -4300)
	{
		gameEnd = true;
	}

	if ((planeX + 33) >= -730 && (planeX - 33) <= -670 && (planeZ + 50) >= 91 && (planeZ - 50) <= 109 || (planeX + 33) >= -730 && (planeX - 33) <= -670 && (planeZ + 50) >= -91 && (planeZ - 50) <= -109 ||
		(planeX + 33) >= -1230 && (planeX - 33) <= -1170 && (planeZ + 50) >= 91 && (planeZ - 50) <= 109 || (planeX + 33) >= -1230 && (planeX - 33) <= -1170 && (planeZ + 50) >= 9 && (planeZ - 50) <= -9 ||
		(planeX + 33) >= -1730 && (planeX - 33) <= -1670 && (planeZ + 50) >= 91 && (planeZ - 50) <= 109 || (planeX + 33) >= -1730 && (planeX - 33) <= -1670 && (planeZ + 50) >= -91 && (planeZ - 50) <= -109 ||
		(planeX + 33) >= -2230 && (planeX - 33) <= -2170 && (planeZ + 50) >= -91 && (planeZ - 50) <= -109 || (planeX + 33) >= -2230 && (planeX - 33) <= -2170 && (planeZ + 50) >= 9 && (planeZ - 50) <= -9 ||
		(planeX + 33) >= -2730 && (planeX - 33) <= -2670 && (planeZ + 50) >= -91 && (planeZ - 50) <= -109 || (planeX + 33) >= -2730 && (planeX - 33) <= -2670 && (planeZ + 50) >= 91 && (planeZ - 50) <= 109 || 
		(planeX + 33) >= -3230 && (planeX - 33) <= -3170 && (planeZ + 50) >= 91 && (planeZ - 50) <= 109 || (planeX + 33) >= -3230 && (planeX - 33) <= -3170 && (planeZ + 50) >= 9 && (planeZ - 50) <= -9 || 
		(planeX + 33) >= -3730 && (planeX - 33) <= -3670 && (planeZ + 50) >= 91 && (planeZ - 50) <= 109 || (planeX + 33) >= -3730 && (planeX - 33) <= -3670 && (planeZ + 50) >= -91 && (planeZ - 50) <= -109 || 
		(planeX + 33) >= -4230 && (planeX - 33) <= -4170 && (planeZ + 50) >= 9 && (planeZ - 50) <= -9 || (planeX + 33) >= -4230 && (planeX - 33) <= -4170 && (planeZ + 50) >= -91 && (planeZ - 50) <= -109 )
	{
		std::cout << "Collided" << std::endl;
		collide = true;
	}
	
	camMove = 1;
}

void SceneChangi::Render()
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

	//RenderMeshOnScreen(meshList[GEO_INVENTORY], 40, 20, 30, 30);
	if (use == true) {
		camera.position.x =  -18;
		camera.position.y = 51;
		camera.position.z = 0;
		camera.theta = 180;
	}


	//Skybox
	RenderSkybox();

	//========================================================
	modelStack.LoadIdentity();

	//RenderMesh(meshList[GEO_AXES], false);

	RenderRoad();
	RenderGroundMesh();
	RenderEntity();
	RenderMissile();
	RenderWords();

	Scene::RenderUI(cooldown, fps, modelStack, viewStack, projectionStack, m_parameters);
}

void SceneChangi::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y + 200, camera.position.z);

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -499);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 499);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -499, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 499, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(499, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-499, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneChangi::RenderGroundMesh()
{

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_QUAD], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
}

void SceneChangi::RenderEntity()
{
	modelStack.PushMatrix();
	modelStack.Translate(-500, 0, 0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_TOWER], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -10, 0);
	modelStack.Scale(10, 30, 30);
	RenderMesh(meshList[GEO_AIRPORT], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	if (takeFlight) {

		if (Application::IsKeyPressed('I'))
		{
			movex -= 1;
		}
		if (Application::IsKeyPressed('K'))
		{
			movex += 1;
		}

		if (Application::IsKeyPressed('J'))
		{
			movez += 1.5;
		}
		if (Application::IsKeyPressed('L'))
		{
			movez -= 1.5;
		}

		autoMove -= 1;
		if (gameEnd == false) {
			camera.position.x = camera.position.x - camMove;
		}
		else {
			modelStack.Translate(0, 17 + autoMove, 0);
		}
		planeX = 10 + movex + autoMove;
		planeZ = -6.5 + movez;
		modelStack.Translate(planeX, 17, planeZ);
	}
	else {
		modelStack.Translate(10, 17, -6.5);
	}
	modelStack.Rotate(-90, 0, 1, 0);
	if (takeFlight != false) 
	{
		modelStack.Scale(3, 3, 3);
	}
	else 
	{
		modelStack.Scale(7, 7, 7);
	}
	RenderMesh(meshList[GEO_PLANE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(10, 0, 343);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(30,30,30);
	RenderMesh(meshList[GEO_AMBULANCE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(10, 0, 291);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_FIRETRUCK], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(10, 0, 393);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_POLICE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	if (renderStairs != false) {
		modelStack.PushMatrix();
		modelStack.Translate(-40, 2, 36);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[GEO_STAIRCAR], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();
	}

	if (renderDoorman != false) {

		modelStack.PushMatrix();
		modelStack.Translate(-100, 0, 60);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_WOMAN], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();
	}
}

void SceneChangi::RenderRoad()
{
	modelStack.PushMatrix();
	modelStack.Translate(-10, 1, 50);
	modelStack.Scale(100, 100, 100);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_ROADL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200, 1, 50);
	modelStack.Scale(100, 100, 100);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_STRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-400, 1, 50);
	modelStack.Scale(50, 100, 100);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_ROADL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
}

void SceneChangi::RenderWords()
{
	modelStack.PushMatrix();
	modelStack.Translate(-100, 20, 53);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderText(meshList[GEO_TEXT], "Touch me", Color(1, 0, 0), modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	if (gameStart == true) {
		RenderMinigameIntro("Use [I],[J],[K],[L] to move the plane. Dodge incoming missiles, 1 is enough to kill you. You will be awarded with a map of garden.", "Escape", 6, modelStack, viewStack, projectionStack, m_parameters);

		if (Application::IsKeyPressed('E'))
		{

			camera.position.x = 100;
			camera.position.y = 260;
			camera.position.z = 0;
			camera.phi = -90;

			takeFlight = true;
		}
	}

	if (gameEnd == true && camera.position.x == -4300) {
		RenderTextOnScreen(meshList[GEO_TEXT], "Goodjob, you make it out.", Color(1, 0, 0), 4, 16, 28, modelStack, viewStack, projectionStack, m_parameters);
		RenderTextOnScreen(meshList[GEO_TEXT], "[ENTER] Bck to main", Color(1, 0, 0), 4, 20, 24, modelStack, viewStack, projectionStack, m_parameters);
		if (Application::IsKeyPressed(VK_RETURN)) {
			Application::SwitchScene = 0;
		}
	}

	if (camera.position.x == -4299) {
		inventory->additem(new InteractableObject(Vector3(-4500, 0, 0), 0, 2, 10, "Map", "Garden map", false));
	}

	if (collide == true) {
		gameEnd = true;
		
		RenderTextOnScreen(meshList[GEO_TEXT], "[GAME OVER]", Color(1, 0, 0), 4, 16, 28, modelStack, viewStack, projectionStack, m_parameters);
		RenderTextOnScreen(meshList[GEO_TEXT], "[R] to retry.", Color(1, 0, 0), 4, 20, 24, modelStack, viewStack, projectionStack, m_parameters);
		if (Application::IsKeyPressed('R')) {
			takeFlight = false;
			gameStart = false;
			collide = false;
			gameEnd = false;
			renderStairs = true;
			renderDoorman = true;
			camera.Init(Vector3(-204, 10.3, 60), Vector3(0, 4.34, 0), Vector3(0, 1, 0));
			movex = 0;
			movez = 0;
			planeX = 10;
			planeZ = -6.5;
			autoMove = 0;
		}
	}
}

void SceneChangi::RenderMissile()
{
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-700, 30, 100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-700, 30, -100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-1200, 30, 100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-1200, 30, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-1700, 30, 100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-1700, 30, -100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-2200, 30, -100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-2200, 30, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-2700, 30, -100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-2700, 30, 100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-3200, 30, 100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-3200, 30, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-3700, 30, 100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-3700, 30, -100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-4200, 30, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
	modelStack.PushMatrix();
	modelStack.Translate(-4200, 30, -100);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(6, 6, 6);
	RenderMesh(meshList[GEO_MISSILE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
	//=============================================================================================
}

void SceneChangi::Exit()
{
	for (auto it = terrains.begin(); it != terrains.end(); ++it)
		delete (*it);
	for (auto it = items.begin(); it != items.end(); ++it)
		delete (*it);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
