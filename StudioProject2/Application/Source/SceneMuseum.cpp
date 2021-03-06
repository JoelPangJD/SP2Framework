#include "SceneMuseum.h"
#include "GL\glew.h"
#include "Application.h"

#include "LoadTGA.h"
#include "Utility.h"
#include "shader.hpp"
#include <Mtx44.h>
#include"MeshBuilder.h"
#include "Entity.h"


SceneMuseum::SceneMuseum()
{
}

SceneMuseum::SceneMuseum(Inventory* inventory)
{
	this->inventory = inventory;
}

SceneMuseum::~SceneMuseum()
{
}

void SceneMuseum::Init()
{
	//======Matrix stack========
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//==========================
	camera.Init(Vector3(-220.713, 10, 242.881), Vector3(-220.717, 10, 541.881), Vector3(0, 1, 0));
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
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

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
	light[0].position.Set(0, 500, 0);
	light[0].color.Set(0.24725f, 0.1995f, 0.0745f);
	light[0].power = 500;
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
	light[1].power = 1.3;
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
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(0.5f, 0.2f, 0.0f), 1);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", Color(0.5, 0.5, 0.5), 10, 10, 0.2);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SPHERE]->material.kShininess = 1.f;
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Lightball", Color(1, 1, 1), 10, 10, 10);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Font.tga");

	meshList[GEO_TEXTINTEXTBOX] = MeshBuilder::GenerateText("Text in text box", 16, 16);
	meshList[GEO_TEXTINTEXTBOX]->textureID = LoadTGA("Image//Museum//ExportedFont.tga");
	meshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_TEXTBOX]->textureID = LoadTGA("Image//Marina//textbox.tga");
	meshList[GEO_HEADER] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_HEADER]->textureID = LoadTGA("Image//Marina//header.tga");

	//Skybox quads
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.0f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Museum//newleft.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.0f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Museum//newright.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.0f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Museum//newforward.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.0f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Museum//newbackward.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.0f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Museum//upward.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.0f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Museum//newdown.tga");

	//OBJs
	meshList[GEO_ELEPHANT] = MeshBuilder::GenerateOBJMTL("Elephant", "OBJ//Museum//elephant.obj", "OBJ//Museum//elephant.mtl");
	meshList[GEO_ELEPHANT]->textureID = LoadTGA("Image//Museum//elephantfull.tga");
	meshList[GEO_PAINTING] = MeshBuilder::GenerateOBJMTL("Painting", "OBJ//Museum//Painting.obj", "OBJ//Museum//Painting.mtl");
	meshList[GEO_PAINTING]->textureID = LoadTGA("Image//Museum//painting.tga");
	meshList[GEO_PAINTING2] = MeshBuilder::GenerateOBJMTL("2nd Painting", "OBJ//Museum//Painting.obj", "OBJ//Museum//Painting.mtl");
	meshList[GEO_PAINTING2]->textureID = LoadTGA("Image//Museum//BritishlosttoJapan.tga");
	meshList[GEO_GLASSTABLE] = MeshBuilder::GenerateOBJMTL("Glass Table", "OBJ//Museum//Wood_Table.obj", "OBJ//Museum//Wood_Table.mtl");
	meshList[GEO_GLASSTABLE]->textureID = LoadTGA("Image//Museum//Reflexion.tga");
	meshList[GEO_GLASSTABLE]->textureID = LoadTGA("Image//Museum//Wood_Table_C.tga");
	meshList[GEO_GLASSTABLE]->textureID = LoadTGA("Image//Museum//Wood_Table_C_2.tga");
	meshList[GEO_WALL] = MeshBuilder::GenerateOBJMTL("Wall", "OBJ//Museum//wallWoodHalf.obj", "OBJ//Museum//wallWoodHalf.mtl");
	meshList[GEO_WALLCURVED] = MeshBuilder::GenerateOBJMTL("Wall", "OBJ//Museum//wallWoodCurved.obj", "OBJ//Museum//wallWoodCurved.mtl");
	meshList[GEO_RICKSHAW] = MeshBuilder::GenerateOBJ("rickshaw", "OBJ//Museum//18569_Human-Pulled Rickshaw_v1.obj");
	meshList[GEO_WALLDOOR] = MeshBuilder::GenerateOBJMTL("Wall Door", "OBJ//Museum//wallDoor.obj", "OBJ//Museum//wallDoor.mtl");
	meshList[GEO_WALLCORNER] = MeshBuilder::GenerateOBJMTL("Wall Corner", "OBJ//Museum//wallWoodCorner.obj", "OBJ//Museum//wallWoodCorner.mtl");

	//NPC
	meshList[GEO_TEACHER] = MeshBuilder::GenerateOBJMTL("Wall Corner", "OBJ//Museum//untitled.obj", "OBJ//Museum//untitled.mtl");
	meshList[GEO_TEACHER]->textureID = LoadTGA("Image//Museum//criminalMaleA.tga");
	meshList[GEO_TEACHER]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_TEACHER]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_TEACHER]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_TEACHER]->material.kShininess = 1.f;
	meshList[GEO_ANDY] = MeshBuilder::GenerateOBJ("friend", "OBJ//Marina//defaultCharacter.obj");
	meshList[GEO_ANDY]->textureID = LoadTGA("Image//CityCenter//friend.tga");
	meshList[GEO_ANDY]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_ANDY]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_ANDY]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_ANDY]->material.kShininess = 1.f;
	//OBJ FOR GAME1
	meshList[GEO_MINIPIC1] = MeshBuilder::GenerateQuad("Minigame pic1", Color(1, 1, 1), 1.0f);
	meshList[GEO_MINIPIC1]->textureID = LoadTGA("Image//Museum//photomain.tga");
	meshList[GEO_MINIPIC1]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_MINIPIC1]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MINIPIC1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MINIPIC1]->material.kShininess = 1.f;
	meshList[GEO_DISPLAYMINIPIC1] = MeshBuilder::GenerateQuad("Display of Minigame pic1", Color(1, 1, 1), 1.0f);
	meshList[GEO_DISPLAYMINIPIC1]->textureID = LoadTGA("Image//Museum//DisplayMain.tga");
	meshList[GEO_DISPLAYMINIPIC1]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_DISPLAYMINIPIC1]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_DISPLAYMINIPIC1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_DISPLAYMINIPIC1]->material.kShininess = 1.f;
	meshList[GEO_SHOWANSWER] = MeshBuilder::GenerateQuad("show answer", Color(1, 1, 1), 1.0f);
	meshList[GEO_SHOWANSWER]->textureID = LoadTGA("Image//Museum//ShowAnswer.tga");
	meshList[GEO_SHOWANSWER]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SHOWANSWER]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SHOWANSWER]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SHOWANSWER]->material.kShininess = 1.f;
	meshList[GEO_SELECTION] = MeshBuilder::GenerateQuad("Answer", Color(1, 1, 1), 1.0f);
	meshList[GEO_SELECTION]->textureID = LoadTGA("Image//Museum//AnswerScreen.tga");
	meshList[GEO_SELECTION]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SELECTION]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SELECTION]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SELECTION]->material.kShininess = 1.f;

	//OBJ FOR GAME 2
	meshList[GEO_PIC] = MeshBuilder::GenerateQuad("Holding minigame pic1", Color(1, 1, 1), 1.0f);
	meshList[GEO_PIC]->textureID = LoadTGA("Image//Museum//slidemesh.tga");
	meshList[GEO_PIC]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_PIC]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_PIC]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PIC]->material.kShininess = 1.f;
	meshList[GEO_BOX] = MeshBuilder::GenerateQuad("Box for filling up", Color(1, 1, 1), 1.0f);
	meshList[GEO_BOX]->textureID = LoadTGA("Image//Museum//box.tga");
	meshList[GEO_BOX]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_BOX]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_BOX]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_BOX]->material.kShininess = 1.f;
	meshList[GEO_ITEM1] = MeshBuilder::GenerateOBJMTL("Box", "OBJ//Museum//wallBlock.obj", "OBJ//Museum//wallBlock.mtl");
	meshList[GEO_KEY] = MeshBuilder::GenerateOBJMTL("Box", "OBJ//Museum//Key_B_02.obj", "OBJ//Museum//Key_B_02.mtl");
	meshList[GEO_KEY]->textureID = LoadTGA("Image//Museum//keyB_tx.tga");
	meshList[GEO_FLAG] = MeshBuilder::GenerateQuad("FLAG", Color(1, 1, 1), 1.0f);
	meshList[GEO_FLAG]->textureID = LoadTGA("Image//Museum//sg_flag.tga");
	meshList[GEO_FLAG]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_FLAG]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_FLAG]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_FLAG]->material.kShininess = 1.f;
	meshList[GEO_PICFLAG] = MeshBuilder::GenerateQuad("Picture of flag on the floor", Color(1, 1, 1), 1.0f);
	meshList[GEO_PICFLAG]->textureID = LoadTGA("Image//Museum//Flag-Singapore.tga");
	meshList[GEO_PICFLAG]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_PICFLAG]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_PICFLAG]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PICFLAG]->material.kShininess = 1.f;
	meshList[GEO_PICKEY] = MeshBuilder::GenerateQuad("Picture of the key on the floor", Color(1, 1, 1), 1.0f);
	meshList[GEO_PICKEY]->textureID = LoadTGA("Image//Museum//download.tga");
	meshList[GEO_PICKEY]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_PICKEY]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_PICKEY]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PICKEY]->material.kShininess = 1.f;
	meshList[GEO_PICBOX] = MeshBuilder::GenerateQuad("Picture of the box on the floor", Color(1, 1, 1), 1.0f);
	meshList[GEO_PICBOX]->textureID = LoadTGA("Image//Museum//wallBlock_E.tga");
	meshList[GEO_PICBOX]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_PICBOX]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_PICBOX]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_PICBOX]->material.kShininess = 1.f;
	//VECTORS FOR WALLS TO CHECK HITBOX
	hitbox.push_back(new Terrain(Vector3(46, 0, -119.707), 0, 0, 0, 8, 220.66, "Wall"));
	hitbox.push_back(new Terrain(Vector3(93.77, 0, -243.091), 0, 0, 0, 70.626, 8, "Wall"));
	hitbox.push_back(new Terrain(Vector3(152.5, 0, -120.507), 0, 0, 0, 8, 220.430, "Wall"));
	hitbox.push_back(new Terrain(Vector3(206.5, 0, -10.8), 0, 0, 0, 110.721, 8, "Wall"));
	hitbox.push_back(new Terrain(Vector3(-107.9315, 0, 72), 0, 22, 10, 300.9754, 4.5, "Wall"));
	hitbox.push_back(new Terrain(Vector3(42, 0, 130), 0, 22, 10, 8, 115.732, "Wall"));
	hitbox.push_back(new Terrain(Vector3(97.606, 0, 206.3716), 0, 22, 10, 100, 8, "Wall"));
	hitbox.push_back(new Terrain(Vector3(260, 0, 70.479), 0, 22, 10, 221.105, 8, "Wall"));
	hitbox.push_back(new Terrain(Vector3(153.5, 0, 130), 0, 22, 10, 8, 125, "Wall"));
	hitbox.push_back(new Terrain(Vector3(-101.273, 0, -10.2017), 0, 22, 10, 295.839, 8, "Wall"));
	hitbox.push_back(new Terrain(Vector3(-246.153, 0, -20.2017), 0, 22, 10, 8, 21.1293, "Wall"));
	hitbox.push_back(new Terrain(Vector3(-246.153, 0, -55.374), 0, 22, 10, 8, 84, "Wall"));
	hitbox.push_back(new Terrain(Vector3(-265.3887, 0, -89.354), 0, 22, 10, 39.725, 8, "Wall"));
	hitbox.push_back(new Terrain(Vector3(-284.3725, 0, 84.472), 0, 22, 10, 8, 351.252, "Wall"));
	hitbox.push_back(new Terrain(Vector3(-236.308, 0, 260.323), 0, 22, 10, 98.564, 8, "Wall"));
	hitbox.push_back(new Terrain(Vector3(-186.628, 0, 168.187), 0, 22, 15, 8, 190.936, "Wall"));
	hitbox.push_back(new Terrain(Vector3(371.538, 0, -100.3729), 0, 22, 10, 8, 223.8275 , "Wall"));
	hitbox.push_back(new Terrain(Vector3(371.538, 0, -36.144), 0, 22, 10, 8, 223.8275, "Wall"));
	hitbox.push_back(new Terrain(Vector3(371.538, 0, 62.02704), 0, 22, 10, 8, 20, "Wall"));
	hitbox.push_back(new Terrain(Vector3(261.7488, 0, -205.3159), 0, 22, 10, 218.204, 8, "Wall"));
	hitbox.push_back(new Terrain(Vector3(261.7488, 0, -205.3159), 0, 22, 10, 218.204, 8, "Wall"));
	//Door hitbox 
	hitbox.push_back(new Terrain(Vector3(-272.215, 0, 73.66698), 0, 22, 10, 218.204, 8, "door"));
	//DECORATION HITBOX
	hitbox.push_back(new Terrain(Vector3(90.2891, 20, -196.527), 0, 22, 10, 228.204, 3, "Elephant hitbox"));
	hitbox.push_back(new Terrain(Vector3(97.15, 0, 151.471), 0, 22, 10, 218.204, 3, "Rickshaw hitbox"));
	hitbox.push_back(new Terrain(Vector3(12.7309, 0, 5.72924), 0, 22, 20, 20, 20, "Andy hitbox"));
	hitbox.push_back(new Terrain(Vector3(268.052, 0, -108.232), 0, 22, 20, 20, 20, "Mr Tang hitbox"));
	hitbox.push_back(new Terrain(Vector3(-104.012, 0, -2.04312), 0, 22, 20, 20, 20, "Table and box hitbox"));

	//Items vector
	items.push_back(new InteractableObject(Vector3(-210.785, 16.0715, 78.3848), 0, 0, 30, "preview", "preview for game", false));
	items.push_back(new InteractableObject(Vector3(-283.869, 16.0715, 95.1478), 0, 0, 30, "answer", "to answer", false));
	items.push_back(new InteractableObject(Vector3(-265.813, 23, -87.885), 0, 0, 30, "exit", "Exit", false));
	items.push_back(new InteractableObject(Vector3(-104.012, 6, -2.04312), 0, 5, 20, "box", "boxy", true));
	items.push_back(new InteractableObject(Vector3(12.7309, 0, 5.72924), 0, 0.75, 20, "andy", "Andy", false));
	items.push_back(new InteractableObject(Vector3(268.052, 0, -108.232), 0, 5, 20, "before gathering item", "Mr tang", false));
	items.push_back(new InteractableObject(Vector3(268.052, 0, -108.232), 0, 5, 20, "after gathering item", "Mr tang", false));
	items.push_back(new InteractableObject(Vector3(90.2891, 20, -210.542), 0, 5, 50, "elephant", "The elephant", false));
	items.push_back(new InteractableObject(Vector3(97.15, 0, 163.717), 0, 5, 50, "rickshaw", "The Rickshaw", false));
	items.push_back(new InteractableObject(Vector3(142.895, 2, 77.452), 90, 1, 20, "key", "Mr Tang's Key", true));
	items.push_back(new InteractableObject(Vector3(192.456, 10, -9.23282), 0, 10, 20, "flag", "Mr Tang's flag", true));

	items.push_back(new InteractableObject(Vector3(160.813, 10, -18.725), 0, 10, 20, "place flag", "Place flag", false));
	items.push_back(new InteractableObject(Vector3(361.669, 10, -198.308), 0, 10, 20, "place key", "Place key", false));
	items.push_back(new InteractableObject(Vector3(160.696, 10, -197.08), 0, 10, 20, "place box", "Place box", false));


	//Ground mesh
	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1), 1.0f);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//Museum//wood.tga");

	//Button for game 1
	button.setButton(26.3, 32.9, 28.9, 16);


	//Button for exit museum
	button2.setButton(58.7, 0.5, 16.4, 16.4);

}



void SceneMuseum::Update(double dt)
{
	fps = 1.f / dt;
	if (cooldown > 0)
		cooldown -= dt;

	TranslateBoxY += dt * TranslateBoxDirectional * 10; //speed of translation of Y
	if (TranslateBoxY > 5|| TranslateBoxY < -5)
	{
		TranslateBoxDirectional *= -1; // changes direction
	}

	TranslateKeyY += dt * TranslateKeyDirectional * 10; //speed of translation of Y
	if (TranslateKeyY > 5 || TranslateKeyY < -5)
	{
		TranslateKeyDirectional *= -1; // changes direction
	}

	TranslateFlagY -= dt * TranslateFlagDirectional * 10; //speed of translation of Y
	if (TranslateFlagY > 5 || TranslateFlagY < -5)
	{
		TranslateFlagDirectional *= -1; // changes direction
	}

	if (Preview == false || FoundAnswer == true)
	{
		Application::enableMouse = false;
		movement(camera, hitbox, dt);
	}

	if (Application::IsKeyPressed('E')) // To summon introduction
	{
		if (RenderIntruction1)
		{
			RenderIntruction1 = false;
		}
		else if (RenderIntruction2 && EndGame1)
		{
			RenderIntruction2 = false;
		}
		else if (RenderIntruction3 && place1 && place2 && place3)
		{
			RenderIntruction3 = false;
		}
		else if (RenderIntruction4 && ToExit)
		{
			RenderIntruction4 = false;
		}
	}

	if (Application::IsKeyPressed('E')) //during preview and show answer to look out
	{
		if (Preview)
		{
			Preview = false;
		}
		if (ShowAnswer)
		{
			MousePreview = false;
			ShowAnswer = false;
		}
	}

	if (MousePreview == true)
	{
		if (EndGame1 == false) //Button for the First Game
		{
			button.updateButton();
			Application::enableMouse = true;
			if (button.clickedOn)
			{
				Application::enableMouse = false;
				FoundAnswer = true;
				for (std::vector<Terrain*>::iterator it = hitbox.begin(); it != hitbox.end(); it++)
				{
					if ((*it)->gettype() == "door")
					{
						hitbox.erase(it);
						break;
					}
				}
				Preview = false;
				ShowAnswer = false;
				ShowFirstGame = false;
			}
		}
		else if(ToExit) // button for exit door
		{
			button2.updateButton();
			Application::enableMouse = true;
			if (button2.hold)
			{
				AddSize += 10 * dt;
				MoveX += (10 * dt) / 2;
				if (MoveX > 36.9 && AddSize > 73)
				{
					AddSize = 0;
					MoveX = 0;
					Application::enableMouse = false;
					MousePreview = false;
					RenderIntruction4 = true;
					ToExit = false;
					Application::SwitchScene = 0;
				}
			}
		}
	}
}

void SceneMuseum::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y + 200, camera.position.z);

	/*Author
		======
		This is the work of Emil Persson, aka Humus.
		http://www.humus.name
	License
		====== =
		This work is licensed under a Creative Commons Attribution 3.0 Unported License.
		http://creativecommons.org/licenses/by/3.0/*/


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

void SceneMuseum::RenderWalls()
{
	modelStack.PushMatrix();
	modelStack.Translate(54.5, 0, -113.507);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 205);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(93.77, 0, -235.091);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 60);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(161.5, 0, -120.507);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 217);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(206.5, 0, -2);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 110);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-107.9315, 0, 82);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 300);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50, 0, 130);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 115);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(97.606, 0, 216.3716);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 70);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(242, 0, 80);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 180);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(163, 0, 130);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 115);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100.273, 0, -2.2017);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 293.69);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-237.153, 0, -21.76082);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 20);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-237.153, 0, -80);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 20);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-256.725, 0, -80);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 20);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-275.86, 0, -80);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 40, 20);
	RenderMesh(meshList[GEO_WALLCORNER], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-275.77, 0, 85.866);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 312.079);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-275.86, 0, 251.47);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(20, 40, 20);
	RenderMesh(meshList[GEO_WALLCORNER], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-226.94, 0, 269.39);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 80);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-177.61, 0, 166.13);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 186.80);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-272.215, 0, 82.023);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 40.1209, 29.7171);
	RenderMesh(meshList[GEO_WALLDOOR], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-255.54, 0, -51.028);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(20, 40.1209, 39);
	RenderMesh(meshList[GEO_WALLDOOR], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(380.54, 0, 36.895);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 40.1209, 39);
	RenderMesh(meshList[GEO_WALLDOOR], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(362.13, 0, 63.431);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(21, 40, 16.540);
	RenderMesh(meshList[GEO_WALLCORNER], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(260.982, 0, -199.02);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 219.59);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(350.982, 0, 80);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 40);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(380.54, 0, -93.718);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 227.96);
	RenderMesh(meshList[GEO_WALL], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(55, 0, 198);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(21, 40, 30);
	RenderMesh(meshList[GEO_WALLCURVED], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(143.4894, 0, 194.39);
	modelStack.Rotate(360, 0, 1, 0);
	modelStack.Scale(25, 40, 30);
	RenderMesh(meshList[GEO_WALLCURVED], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(57.0803, 0, -229.69);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(25, 40, 30);
	RenderMesh(meshList[GEO_WALLCURVED], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(138.77, 0, -233.091);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(25, 40, 30);
	RenderMesh(meshList[GEO_WALLCURVED], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();
}

void SceneMuseum::StartGame1()
{
	if (!EndGame1) //if game 1 is not ended yet
	{
		if (RenderIntruction1) //render intruction
		{
			RenderMinigameIntro("Look at the image at Preview and choose the correct image to unlock the door.", "Instruction", 3, modelStack, viewStack, projectionStack, m_parameters);
		}
		else //after rendering intruction render all of these
		{
			RenderUI(cooldown, fps, modelStack, viewStack, projectionStack, m_parameters);
			interact(camera, items);
			if (Preview && !inmenu) //prevent image to overlay menu image during pause. Also in preview mode render the image to show preview
			{
				RenderMeshOnScreen(meshList[GEO_DISPLAYMINIPIC1], 45, 20, 80, 60, modelStack, viewStack, projectionStack, m_parameters);
				camera.Init(Vector3(-210, 10, 125), Vector3(-200, 10, 125), Vector3(0, 1, 0));
			}
			else if (ShowAnswer && !inmenu) //prevent image to overlay menu image during pause.Also in answer mode render the images and enable mouse for players to choose
			{
				RenderMeshOnScreen(meshList[GEO_SELECTION], 40, 25, 70, 60, modelStack, viewStack, projectionStack, m_parameters);
				camera.Init(Vector3(-210, 10, 125), Vector3(-200, 10, 125), Vector3(0, 1, 0));
				MousePreview = true;
			}
			else if (FoundAnswer) //if found answer, get out of the answer mode and end the game 1
			{
				MousePreview = false;
				for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); ++it)
				{
					if ((*it)->gettype() == "preview")
					{
						items.erase(it);
						it = items.begin();
					}
					if ((*it)->gettype() == "answer")
					{
						items.erase(it);
						break;
					}
				}
				FoundAnswer = false;
				EndGame1 = true;

			}
		}
	}
}
		
void SceneMuseum::StartGame2()
{
	if (EndGame2 == false && EndGame1 == true) //to check if game 1 has ended
	{
		if (RenderIntruction2) //render introduction
		{
			RenderMinigameIntro("Good job! Now go to the end of the musem and talk to Mr Tang, he will tell you to gather 3 items. Gather 3 items and MAKE SURE TO PLACE THEM IN THE CORRECT ORDER around him. Use the arrow key to navigate through your inventory. He will grant you the Changi pass for the next scene afterwards.", "Instruction", 3, modelStack, viewStack, projectionStack, m_parameters);
		}
		else //after introduction render all these
		{
			RenderUI(cooldown, fps, modelStack, viewStack, projectionStack, m_parameters);
			interact(camera, items);
			if (ToExit && !inmenu) // If player want to go back city center
			{
				if (RenderIntruction4) // redner intruction
				{
					RenderMinigameIntro("Hold the button to exit the museum.", "Security", 3, modelStack, viewStack, projectionStack, m_parameters);
				} 
				else //show the holding button game to exit museum
				{
					MousePreview = true;
					GameCam1 = camera;
					camera.Init(Vector3(-260, 10, 10), Vector3(220.717, 0, 541.881), Vector3(0, 1, 0));
					RenderMeshOnScreen(meshList[GEO_PIC], 40, 30, 80, 65, modelStack, viewStack, projectionStack, m_parameters);
					RenderMeshOnScreen(meshList[GEO_BOX], 4 + MoveX, 29, 3 + AddSize, 8.9, modelStack, viewStack, projectionStack, m_parameters);
				}
			}
			if (place1 == true && place2 == true && place3 == true) //after placing all the item, end the game2
			{
				EndGame2 = true;
			}
		}
	}
}
	
void SceneMuseum::ExitMuseum()
{
	if (EndGame2 == true && EndGame1 == true && !RenderIntruction1 && !RenderIntruction2) //checks that game 1 and 2 is over and intruction 1 and 2 has been rendered
	{
		if (RenderIntruction3) //render final intruction
		{
			RenderMinigameIntro("Nicely Done! Now go to Mr Tang and get your Changi Pass.", "Instruction", 3, modelStack, viewStack, projectionStack, m_parameters);
		}
		else
		{
			RenderUI(cooldown, fps, modelStack, viewStack, projectionStack, m_parameters);
			interact(camera, items);
			if (ToExit && !inmenu)
			{
				if (RenderIntruction4)
				{
					RenderMinigameIntro("Hold the button to exit the museum.", "Security", 3, modelStack, viewStack, projectionStack, m_parameters);
				}
				else
				{
					MousePreview = true;
					GameCam1 = camera;
					camera.Init(Vector3(-260, 10, 10), Vector3(220.717, 0, 541.881), Vector3(0, 1, 0));
					RenderMeshOnScreen(meshList[GEO_PIC], 40, 30, 80, 65, modelStack, viewStack, projectionStack, m_parameters);
					RenderMeshOnScreen(meshList[GEO_BOX], 4 + MoveX, 29, 3 + AddSize, 8.9, modelStack, viewStack, projectionStack, m_parameters);
				}
			}
		}
	}
}




void SceneMuseum::Render()
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

	//all the walls
	RenderWalls();

	//GROUND MESH
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_GROUND], false, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	//OBJ
	modelStack.PushMatrix();
	modelStack.Translate(90.2891, 20, -210.542);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(7, 7, 7);
	RenderMesh(meshList[GEO_ELEPHANT], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(45.9141, -8.1529, -87.926);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(20, 25, 50);
	RenderMesh(meshList[GEO_PAINTING], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(151.9141, -8.1529, -87.926);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 25, 50);
	RenderMesh(meshList[GEO_PAINTING2], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(97.15, 0, 163.717);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-51.626, 0, 0, 1);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_RICKSHAW], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(-104.012, 0, -2.04312);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(13, 13, 13);
	RenderMesh(meshList[GEO_GLASSTABLE], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	//Game1 OBJ
	modelStack.PushMatrix();
	modelStack.Translate(-210.785, 16.0715, 75.3848);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_MINIPIC1], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(-283.869, 16.0715, 95.1478);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(35, 35, 45);
	RenderMesh(meshList[GEO_SHOWANSWER], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	//Game2 OBJ
	modelStack.PushMatrix();
	modelStack.Translate(361.669, 16.0715 + TranslateKeyY, -198.308);
	modelStack.Rotate(-45, 0, 1, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_PICKEY], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(160.696, 16.0715 + TranslateBoxY, -197.08);
	modelStack.Rotate(45, 0, 1, 0);
	modelStack.Scale(35, 35, 45);
	RenderMesh(meshList[GEO_PICBOX], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(160.813, 16.0715 + TranslateFlagY, -18.725);
	modelStack.Rotate(135, 0, 1, 0);
	modelStack.Scale(15, 15, 15);
	RenderMesh(meshList[GEO_PICFLAG], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	//Exit door
	modelStack.PushMatrix();
	modelStack.Translate(-265.813, 23, -87.885);
	modelStack.Scale(35, 35, 45);
	RenderMesh(meshList[GEO_PIC], true, modelStack, viewStack, projectionStack, m_parameters);
	modelStack.PopMatrix();

	//For interactable items Game 2
	for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
	{
		modelStack.PushMatrix();
		modelStack.Translate((*it)->getposition().x, (*it)->getposition().y, (*it)->getposition().z);
		modelStack.Rotate((*it)->getangle(), 0, 1, 0);
		modelStack.Scale((*it)->getscale(), (*it)->getscale(), (*it)->getscale());
		if ((*it)->gettype() == "box")
		{
			RenderMesh(meshList[GEO_ITEM1], true, modelStack, viewStack, projectionStack, m_parameters);
		}
		else if ((*it)->gettype() == "andy")
		{
			RenderMesh(meshList[GEO_ANDY], true, modelStack, viewStack, projectionStack, m_parameters);
		}
		else if ((*it)->gettype() == "before gathering item")
		{
			RenderMesh(meshList[GEO_TEACHER], true, modelStack, viewStack, projectionStack, m_parameters);
		}
		else if ((*it)->gettype() == "after gathering item")
		{
			RenderMesh(meshList[GEO_TEACHER], true, modelStack, viewStack, projectionStack, m_parameters);
		}
		else if ((*it)->gettype() == "key")
		{
			RenderMesh(meshList[GEO_KEY], true, modelStack, viewStack, projectionStack, m_parameters);
		}
		else if ((*it)->gettype() == "flag")
		{
			RenderMesh(meshList[GEO_FLAG], true, modelStack, viewStack, projectionStack, m_parameters);
		}
		modelStack.PopMatrix();
	}

	StartGame1();
	StartGame2();
	ExitMuseum();
}

void SceneMuseum::Exit()
{
	//prevent memory leaked also a good habit to remove :D
	for (auto it = hitbox.begin(); it != hitbox.end(); ++it)
		delete (*it);
	for (auto it = items.begin(); it != items.end(); ++it)
		delete (*it);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}