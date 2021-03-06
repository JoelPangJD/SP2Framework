#include "SceneMain.h"
#include "GL\glew.h"
#include "Application.h"

#include "LoadTGA.h"
#include "Utility.h"
#include "shader.hpp"
#include <Mtx44.h>
#include"MeshBuilder.h"

#define PI 3.141592

SceneMain::SceneMain()
{
}

SceneMain::SceneMain(Inventory* inventory)
{
	this->inventory = inventory;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	//======Matrix stack========
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//==========================
	camera.Init(Vector3(-14, 3, 0), Vector3(0, 3, 5), Vector3(0, 1, 0));
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
	light[0].position.Set(0, 10, 0);
	light[0].color.Set(1,1,1);
	light[0].power = 1.f;
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
	light[1].position.Set(-20, 20, 0);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1.f;
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
	meshList[GEO_GROUNDMESH] = MeshBuilder::GenerateQuad("quad", 1, 1, Color(1, 1, 1), 100);
	meshList[GEO_GROUNDMESH]->textureID = LoadTGA("Image//CityCenter//brick.tga");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_PANEL] = MeshBuilder::GenerateQuad("securitypanel", Color(1, 1, 1), 10.f);
	meshList[GEO_PANEL]->textureID = LoadTGA("Image//CityCenter//SecurityPanel.tga");
	meshList[GEO_RED] = MeshBuilder::GenerateQuad("red", Color(1, 0, 0), 1.f);
	meshList[GEO_GREEN] = MeshBuilder::GenerateQuad("green", Color(0, 1, 0), 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(0.5f, 0.2f, 0.0f), 1);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", Color(0.5, 0.5, 0.5), 10, 10, 10);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SPHERE]->material.kShininess = 1.f;
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Lightball", Color(1, 1, 1), 10, 10, 10);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Marina//ExportedFont.tga");

	meshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("Testing", Color(1, 1, 1), 1.0f);
	meshList[GEO_INVENTORY]->textureID = LoadTGA("Image//inventory.tga");


	//Skybox quads
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.0f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//CityCenter//leftCityCenter.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.0f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//CityCenter//rightCityCenter.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.0f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//CityCenter//frontCityCenter.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.0f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//CityCenter//backCityCenter.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.0f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//CityCenter//topCityCenter.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.0f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//CityCenter//bottomCityCenter.tga");

	meshList[GEO_TREE] = MeshBuilder::GenerateOBJMTL("Tree", "OBJ//CityCenter//tree.obj", "OBJ//CityCenter//tree.mtl");
	meshList[GEO_ROADSTRAIGHT] = MeshBuilder::GenerateOBJMTL("straight", "OBJ//CityCenter//road_straight.obj", "OBJ//CityCenter//road_straight.mtl");
	meshList[GEO_ROADCROSS] = MeshBuilder::GenerateOBJMTL("roadcross", "OBJ//CityCenter//road_roundabout.obj", "OBJ//CityCenter//road_roundabout.mtl");
	meshList[GEO_LAMP] = MeshBuilder::GenerateOBJMTL("lamp", "OBJ//CityCenter//lamp.obj", "OBJ//CityCenter//lamp.mtl");
	meshList[GEO_MUSEUM] = MeshBuilder::GenerateOBJMTL("museum", "OBJ//CityCenter//museum.obj", "OBJ//CityCenter//museum.mtl");
	meshList[GEO_TEACHER] = MeshBuilder::GenerateOBJ("teacher", "OBJ//Marina//defaultCharacter.obj");
	meshList[GEO_TEACHER]->textureID = LoadTGA("Image//CityCenter//teacher.tga");
	meshList[GEO_FRIEND] = MeshBuilder::GenerateOBJ("friend", "OBJ//Marina//defaultCharacter.obj");
	meshList[GEO_FRIEND]->textureID = LoadTGA("Image//CityCenter//friend.tga");
	meshList[GEO_HEADER] = MeshBuilder::GenerateQuad("header", Color(1, 1, 1), 1.f);
	meshList[GEO_HEADER]->textureID = LoadTGA("Image//Marina//header.tga");
	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("textbox", Color(1, 1, 1), 1.f);
	meshList[GEO_TEXTBOX]->textureID = LoadTGA("Image//Marina//textbox.tga");
	meshList[GEO_MBS] = MeshBuilder::GenerateOBJMTL("mbs", "OBJ//CityCenter//mbs.obj", "OBJ//CityCenter//mbs.mtl");
	meshList[GEO_CHANGI] = MeshBuilder::GenerateOBJMTL("changi", "OBJ//Changi//ChangiTower.obj", "OBJ//Changi//ChangiTower.mtl");
	meshList[GEO_BUILDING] = MeshBuilder::GenerateOBJMTL("building", "OBJ//CityCenter//building.obj", "OBJ//CityCenter//building.mtl");
	meshList[GEO_BUILDING1] = MeshBuilder::GenerateOBJMTL("building1", "OBJ//CityCenter//building1.obj", "OBJ//CityCenter//building1.mtl");
	meshList[GEO_BUILDING2] = MeshBuilder::GenerateOBJMTL("building2", "OBJ//CityCenter//building2.obj", "OBJ//CityCenter//building2.mtl");
	meshList[GEO_BUILDING3] = MeshBuilder::GenerateOBJMTL("building3", "OBJ//CityCenter//building3.obj", "OBJ//CityCenter//building3.mtl");
	meshList[GEO_ROADCURVESPLITRIGHT] = MeshBuilder::GenerateOBJMTL("road_curvesplitright", "OBJ//CityCenter//road_curvesplitright.obj", "OBJ//CityCenter//road_curvesplitright.mtl");
	meshList[GEO_ROAD4WAY] = MeshBuilder::GenerateOBJMTL("road4way", "OBJ//CityCenter//road_4way.obj", "OBJ//CityCenter//road_4way.mtl");
	meshList[GEO_ROADJUNCTION] = MeshBuilder::GenerateOBJMTL("roadjunction", "OBJ//CityCenter//road_junction.obj", "OBJ//CityCenter//road_junction.mtl");
	meshList[GEO_ROADCURVED] = MeshBuilder::GenerateOBJMTL("roadcurved", "OBJ//CityCenter//road_curved.obj", "OBJ//CityCenter//road_curved.mtl");
	meshList[GEO_ROADINTERSECT] = MeshBuilder::GenerateOBJMTL("roadintersect", "OBJ//CityCenter//road_intersection.obj", "OBJ//CityCenter//road_intersection.mtl");
	meshList[GEO_CAR] = MeshBuilder::GenerateOBJMTL("roadintersect", "OBJ//CityCenter//car.obj", "OBJ//CityCenter//car.mtl");

	minigameMuseum = false;
	firstEnter = firstRender = walletNotGone = true;
	rotateCarY = 180;
	translateCarX = 0;
	translateCarZ = 100;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			grids[(i * 3) + j] = new Vector3(33.5 + (16.5 * j), 13.5 + (16.5 * i), 0);
		}
	}
	int height, width;
	height = width = 15;
	for (int i = 0; i < 9; i++) {
		if ((i == 3) || (i == 4) || (i == 5)) {
			height = 16;
		}
		else {
			height = 15;
		}
		if ((i == 1) || (i == 4) || (i == 7)) {
			width = 16;
		}
		else {
			width = 15;
		}
		gridButton[i].setButton(grids[i]->x - width / 2, grids[i]->y - height / 2, width, height);
		gridButton[i].active = true;
		colorGrid[i] = "Red";
	}
	pass = false;
	items.push_back(new InteractableObject(Vector3(-2, 2, 0), 0, 2, 4, "Mr.Sazz", "Mr.Sazz", false));
	items.push_back(new InteractableObject(Vector3(6, 1, 5), 0, 2, 3, "Andy", "Andy", false));
	items.push_back(new InteractableObject(Vector3(23, 3, 0), 0, 1, 10, "citytomuseum", "To the museum", false)); 
	items.push_back(new InteractableObject(Vector3(0, 3, 58), 0, 1, 10, "citytochangi", "To Changi Airport", false));
	items.push_back(new InteractableObject(Vector3(-48, 3, 0), 0, 1, 10, "citytomarina", "To Marina Bay Sands", false));
	items.push_back(new InteractableObject(Vector3(0, 3, -58), 0, 1, 10, "citytogarden", "To Botanic Garden", false));

	//wall.push_back(new Terrain(Vector3(26, 0, 0), 0, 1, 50, 1, "wall"));
	wall.push_back(new Terrain(Vector3(70, 0, 0), 0, 0, 0, 20, 150.f, "Wall"));
	wall.push_back(new Terrain(Vector3(-60, 0, 0), 0, 0, 0, 20, 150.f, "Wall"));
	wall.push_back(new Terrain(Vector3(0, 0, 70), 0, 0, 0, 150.f, 20, "Wall"));
	wall.push_back(new Terrain(Vector3(0, 0, -70), 0, 0, 0, 150.f, 20, "Wall"));
	wall.push_back(new Terrain(Vector3(7, 0, 7), 0, 10, 0, 4.f, 4.f, "tree"));
	wall.push_back(new Terrain(Vector3(-7, 0, 7), 0, 10, 0, 4.f, 4.f, "tree"));
	wall.push_back(new Terrain(Vector3(7, 0, -7), 0, 10, 0, 4.f, 4.f, "tree"));
	wall.push_back(new Terrain(Vector3(-7, 0, -7), 0, 10, 0, 4.f, 4.f, "tree"));
	wall.push_back(new Terrain(Vector3(0, 0, 0), 0, 7, 0, 2.f, 2.f, "lamp"));
	wall.push_back(new Terrain(Vector3(6, 0, 5), -90, 0.26, 0, 2.f, 2.f, "Andy"));
	wall.push_back(new Terrain(Vector3(-2, 0, 0), -90, 0.3, 0, 2.f, 2.f, "teacher"));
	wall.push_back(new Terrain(Vector3(37.5, 0, 0), 90, 25, 0, 23.f, 32.f, "museum"));
	wall.push_back(new Terrain(Vector3(28, 0, -35), 90, 25, 0, 23.f, 24.f, "building"));
	wall.push_back(new Terrain(Vector3(-28, 0, 15), 90, 25, 0, 23.f, 24.f, "building"));
	wall.push_back(new Terrain(Vector3(-30, 0, -30), -90, 35, 0, 31.f, 46.f, "building1"));
	wall.push_back(new Terrain(Vector3(30, 0, 55), 90, 25, 0, 23.f, 44.f, "building2"));
	wall.push_back(new Terrain(Vector3(-28, 0, 45), 90, 25, 0, 34.f, 34.f, "building3"));
}


void SceneMain::Update(double dt)
{
	if (Scene::start) {
		UpdateStartMenu();
	}
	else{

		fps = 1.f / dt;
		movement(camera, wall, dt);
		string trigger = interact(camera, items);
		if (cooldown > 0) {
			cooldown -= dt;
		}

		if (Application::IsKeyPressed('U'))
		{
			light[0].power = 0;
			light[1].power = 0;
			glUniform1i(m_parameters[U_LIGHT0_POWER], light[0].power);
			glUniform1i(m_parameters[U_LIGHT1_POWER], light[1].power);
		}
		else if (Application::IsKeyPressed('Y'))
		{
			light[0].power = 1;
			light[1].power = 1;
			glUniform1i(m_parameters[U_LIGHT0_POWER], light[0].power);
			glUniform1i(m_parameters[U_LIGHT1_POWER], light[1].power);
		}
		else if (Application::IsKeyPressed('7'))
		{
			//to do: switch light type to SPOT and pass the information to shader
			light[0].type = Light::LIGHT_SPOT;
			glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		}

		if (Application::IsKeyPressed('E')) {
			if (firstEnter) {
				firstEnter = false;
				cooldown = 0.5;
			}
			if ((firstRender) && (minigameMuseum) && (cooldown <= 0)) {
				firstRender = false;
			}

			if ((walletNotGone) && (!firstEnter) && (cooldown <= 0)) {
				walletNotGone = false;
			}

		}

		if (trigger == "frontofmuseum")
		{
			minigameMuseum = true;
			cooldown = 0.5;
		}
		if (trigger == "locked") {
			dialogue.push_back("1It seems that I do not have the item required to open this door.");
			currentline = dialogue.begin();
			name = "";
			indialogue = true;
		}

		//minigame for entering museum
		if (minigameMuseum == true) {
			Application::enableMouse = true;
			if (!pass) {
				updateMinigame(dt);
			}
			if (pass == true) {
				minigameMuseum = false;
				Application::enableMouse = false;
				Application::SwitchScene = 1;
			}
		}
		updateCar(dt);
	}

}



void SceneMain::RenderSkybox()
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


void SceneMain::RenderMinigame()
{
	if (firstRender) {
		RenderMinigameIntro("Click on the squares to make them change colour. Convert all of them to green to enter the museum.", "Security", 3, modelStack, viewStack, projectionStack, m_parameters);
	}
	else {
		RenderMeshOnScreen(meshList[GEO_PANEL], 40, 30, 7, 5, modelStack, viewStack, projectionStack, m_parameters);
		int width, height;
		width = height = 15;
		RenderTextOnScreen(meshList[GEO_TEXT], "Turn all squares", Color(0, 0, 0), 2.2, 7, 50, modelStack, viewStack, projectionStack, m_parameters);
		RenderTextOnScreen(meshList[GEO_TEXT], "Green to enter", Color(0, 0, 0), 2.2, 7, 46, modelStack, viewStack, projectionStack, m_parameters);
		RenderTextOnScreen(meshList[GEO_TEXT], "the museum.", Color(0, 0, 0), 2.2, 7, 42, modelStack, viewStack, projectionStack, m_parameters);
		for (int i = 0; i < 9; i++) {

			if ((i == 3) || (i == 4) || (i == 5)) {
				height = 16;
			}
			else {
				height = 15;
			}
			if ((i == 1) || (i == 4) || (i == 7)) {
				width = 16;
			}
			else {
				width = 15;
			}
			if (colorGrid[i] == "Red") {
				RenderMeshOnScreen(meshList[GEO_RED], grids[i]->x, grids[i]->y, width, height, modelStack, viewStack, projectionStack, m_parameters);
			}
			else if (colorGrid[i] == "Green") {
				RenderMeshOnScreen(meshList[GEO_GREEN], grids[i]->x, grids[i]->y, width, height, modelStack, viewStack, projectionStack, m_parameters);
			}
		}
	}

}

void SceneMain::updateMinigame(double dt)
{
	for (int i = 0; i < 9; i++) {
		gridButton[i].updateButton();
		if (gridButton[i].isClickedOn()) {
			if (colorGrid[i] == "Red") {
				colorGrid[i] = "Green";
			}
			else {
				colorGrid[i] = "Red";
			}
		}
	}

	for (int i = 0; i < 9; i++) {
		if (colorGrid[i] == "Red") {
			break;
		}
		else if (i == 8) {
			pass = true;
			for (int i = 0; i < 9; i++) {
				delete grids[i];
			}
		}
	}

}

void SceneMain::updateCar(double dt)
{
	if (translateCarZ > 77.5) {
		translateCarZ -= 10 * float(dt);
		if (translateCarZ <= 77.5)
			translateCarZ = 77.5;
	}
	else if ((rotateCarY <270) && (translateCarX > -5)) {
		rotateCarY += 25 * float(dt);
		if (rotateCarY >= 270)
			rotateCarY = 270;
		translateCarX = -5 - 5.f * cos(rotateCarY * PI / 180);
		translateCarZ = 77.5 + 2.5f *  sin(rotateCarY * PI / 180);
	}
	else if (translateCarX > -50) {
		translateCarX -= 10 * float(dt);
		if (translateCarX <= -50)
			translateCarX = -50;
	}
	else if (rotateCarY > 180) {
		rotateCarY -= 25 * float(dt);
		if (rotateCarY <= 180)
			rotateCarY = 180;
		translateCarX = -50 + 10.f * cos(rotateCarY * PI / 180);
		translateCarZ = 67.5 - 7.5f * sin(rotateCarY * PI / 180);
	}
	else {
		translateCarZ -= 10 * float(dt);
		if (translateCarZ < -150) {
			translateCarZ = 100;
			translateCarX = 0;
		}
	}
}

void SceneMain::Render()
{
	if (Scene::start) {
		RenderStartMenu(modelStack, viewStack, projectionStack, m_parameters);
	}
	else {
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
			glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		}

		//LIGHT1====================================================
		if (light[1].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
			glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
		}

		//RenderMeshOnScreen(meshList[GEO_INVENTORY], 40, 20, 30, 30);

		//Skybox
		RenderSkybox();

		//========================================================
		//modelStack.LoadIdentity();

		RenderMesh(meshList[GEO_AXES], false, modelStack, viewStack, projectionStack, m_parameters);

		modelStack.PushMatrix();
		modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
		modelStack.Scale(0.05, 0.05, 0.05);
		RenderMesh(meshList[GEO_LIGHTBALL], false, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(1000, 1000, 1000);
		modelStack.Rotate(-90, 1, 0, 0);
		RenderMesh(meshList[GEO_GROUNDMESH], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(17.5, 0, 0);
		modelStack.Scale(20, 5, 5);
		RenderMesh(meshList[GEO_ROADSTRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();



		modelStack.PushMatrix();
		modelStack.Translate(-75.5, 0, 0);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_MBS], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(10, 0, 250);
		modelStack.Scale(10, 10, 10);
		modelStack.Translate(-28, 0, 8);
		RenderMesh(meshList[GEO_CHANGI], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-25, 0, 0);
		modelStack.Scale(35, 5, 5);
		RenderMesh(meshList[GEO_ROADSTRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		for (std::vector<Terrain*>::iterator it = wall.begin(); it != wall.end(); it++)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*it)->getposition().x, (*it)->getposition().y, (*it)->getposition().z);
			modelStack.Rotate((*it)->getangle(), 0, 1, 0);
			modelStack.Scale((*it)->getscale(), (*it)->getscale(), (*it)->getscale());
			if ((*it)->gettype() == "tree")
				RenderMesh(meshList[GEO_TREE], true, modelStack, viewStack, projectionStack, m_parameters);
			else if ((*it)->gettype() == "lamp")
				RenderMesh(meshList[GEO_LAMP], true, modelStack, viewStack, projectionStack, m_parameters);
			else if ((*it)->gettype() == "Andy")
				RenderMesh(meshList[GEO_FRIEND], true, modelStack, viewStack, projectionStack, m_parameters);
			else if ((*it)->gettype() == "teacher")
				RenderMesh(meshList[GEO_TEACHER], true, modelStack, viewStack, projectionStack, m_parameters);
			else if ((*it)->gettype() == "museum")
				RenderMesh(meshList[GEO_MUSEUM], true, modelStack, viewStack, projectionStack, m_parameters);
			else if ((*it)->gettype() == "building")
				RenderMesh(meshList[GEO_BUILDING], true, modelStack, viewStack, projectionStack, m_parameters);
			else if ((*it)->gettype() == "building1")
				RenderMesh(meshList[GEO_BUILDING1], true, modelStack, viewStack, projectionStack, m_parameters);
			else if ((*it)->gettype() == "building2")
				RenderMesh(meshList[GEO_BUILDING2], true, modelStack, viewStack, projectionStack, m_parameters);
			else if ((*it)->gettype() == "building3")
				RenderMesh(meshList[GEO_BUILDING3], true, modelStack, viewStack, projectionStack, m_parameters);
			modelStack.PopMatrix();
		}


		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 40);
		modelStack.Scale(5, 5, 65);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_ROADSTRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 110);
		modelStack.Scale(5, 5, 65);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_ROADSTRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -32.5);
		modelStack.Scale(5, 5, 50);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_ROADSTRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_ROADCROSS], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-55, 0, 0);
		modelStack.Scale(5, 5, 5);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_ROADJUNCTION], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-57.5, 0, 72.5);
		modelStack.Scale(5, 5, 5);
		modelStack.Rotate(180, 0, 1, 0);
		RenderMesh(meshList[GEO_ROADCURVED], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-57.5, 0, -57.5);
		modelStack.Scale(5, 5, 5);
		modelStack.Rotate(90, 0, 1, 0);
		RenderMesh(meshList[GEO_ROADCURVESPLITRIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-60, 0, -120);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(115, 5, 5);
		RenderMesh(meshList[GEO_ROADSTRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-60, 0, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_ROAD4WAY], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-60, 0, 35);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(65, 5, 5);
		RenderMesh(meshList[GEO_ROADSTRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-27.5, 0, 75);
		modelStack.Scale(50, 5, 5);
		RenderMesh(meshList[GEO_ROADSTRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-27.5, 0, -60);
		modelStack.Scale(50, 5, 5);
		RenderMesh(meshList[GEO_ROADSTRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-60, 0, -35);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(35, 5, 5);
		RenderMesh(meshList[GEO_ROADSTRAIGHT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 75);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_ROADINTERSECT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -60);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_ROADINTERSECT], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(translateCarX, 0, translateCarZ);
		modelStack.Scale(3, 3, 3);
		modelStack.Rotate(rotateCarY, 0, 1, 0);
		RenderMesh(meshList[GEO_CAR], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-7, 0, -70);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_TREE], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(7, 0, -73);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_TREE], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(1, 0, -78);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_TREE], true, modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Translate(2.5, 9, 62.4);
		modelStack.Scale(2, 2, 2);
		modelStack.Rotate(180, 0, 1, 0);
		RenderText(meshList[GEO_TEXT], "Changi Airport", Color(0, 0, 0), modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-52.4, 9, 2.5);
		modelStack.Scale(2, 2, 2);
		modelStack.Rotate(90, 0, 1, 0);
		RenderText(meshList[GEO_TEXT], "MarinaBay", Color(0, 0, 0), modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-2.5, 9, -62.4);
		modelStack.Scale(2, 2, 2);
		RenderText(meshList[GEO_TEXT], "Botanic Garden", Color(0, 0, 0), modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(27.4, 9, -1.5);
		modelStack.Scale(2, 2, 2);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderText(meshList[GEO_TEXT], "Museum", Color(0, 0, 0), modelStack, viewStack, projectionStack, m_parameters);
		modelStack.PopMatrix();



		if (firstEnter == true) {
			{
				dialogue.push_back("2Welcome to the city tour, you can press T to talk to, F to observe, G to pick up items and E to continue conversations.");
				dialogue.push_back("2Finally, you can use the up and down arrow keys to navigate your inventory and Q to use items.");
				name = "Mr.Sazz";
				currentline = dialogue.begin();
				indialogue = true;
				firstEnter = false;
			}
		}
		if ((walletNotGone) && (!firstEnter) && (cooldown <= 0)) {
			dialogue.push_back("1Wait, my pocket is lighter now... My wallet is gone? I have to find my wallet.");
			currentline = dialogue.begin();
			name = "";
			indialogue = true;
			walletNotGone = false;
		}

		RenderUI(cooldown, fps, modelStack, viewStack, projectionStack, m_parameters);

		if (minigameMuseum == true) {
			RenderMinigame();
		}
		//stringstream ss;
		//ss.str("");
		//ss << "Pos: X: " << camera.position.x << " Z: " << camera.position.z;
		//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 4, 0, 50, modelStack, viewStack, projectionStack, m_parameters);
	}
}

void SceneMain::Exit()
{
	for (auto it = wall.begin(); it != wall.end(); ++it)
		delete (*it);
	for (auto it = items.begin(); it != items.end(); ++it)
		delete (*it);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
