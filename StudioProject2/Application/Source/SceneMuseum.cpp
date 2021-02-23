#include "SceneMuseum.h"
#include "GL\glew.h"
#include "Application.h"

#include "LoadTGA.h"
#include "Utility.h"
#include "shader.hpp"
#include <Mtx44.h>
#include"MeshBuilder.h"


SceneMuseum::SceneMuseum()
{
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
	camera.Init(Vector3(-220.713, 10, 242.881), Vector3(-220.717, 10, 241.881), Vector3(0, 1, 0));
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
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", Color(0.5, 0.5, 0.5), 10, 10, 10);
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


	meshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("Testing", Color(1, 1, 1), 1.0f);
	meshList[GEO_INVENTORY]->textureID = LoadTGA("Image//inventory.tga");


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

	//OBJ FOR GAME1
	meshList[GEO_MINIPIC1] = MeshBuilder::GenerateQuad("Minigame pic1", Color(1, 1, 1), 1.0f);
	meshList[GEO_MINIPIC1]->textureID = LoadTGA("Image//Museum//photomain.tga");
	meshList[GEO_MINIPIC1]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_MINIPIC1]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MINIPIC1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MINIPIC1]->material.kShininess = 1.f;
	meshList[GEO_SELECTION] = MeshBuilder::GenerateQuad("Answer for color blind photo", Color(1, 1, 1), 1.0f);
	meshList[GEO_SELECTION]->textureID = LoadTGA("Image//Museum//selection.tga");
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

	//VECTORS FOR WALLS TO CHECK HITBOX
	terrains.push_back(new Terrain(Vector3(45, 0, -119.707), 0,0,0, 3, 220.66, "Wall"));
	terrains.push_back(new Terrain(Vector3(93.77, 0, -243.091),0,0,0, 70.626, 3, "Wall"));
	terrains.push_back(new Terrain(Vector3(152.5, 0, -120.507),0,0,0, 3, 220.430, "Wall"));
	terrains.push_back(new Terrain(Vector3(206.5, 0, -12), 0, 0, 0, 110.721, 3, "Wall"));
	terrains.push_back(new Terrain(Vector3(-107.9315, 0, 72), 0, 22, 10, 300.9754, 4.5, "Wall"));
	terrains.push_back(new Terrain(Vector3(42, 0, 130), 0, 22, 10, 3, 115.732, "Wall"));
	terrains.push_back(new Terrain(Vector3(97.606, 0, 206.3716), 0, 22, 10, 100, 3, "Wall"));
	terrains.push_back(new Terrain(Vector3(260, 0, 71.5), 0, 22, 10, 221.105, 3, "Wall"));
	terrains.push_back(new Terrain(Vector3(153.5, 0, 130), 0, 22, 10, 3, 125, "Wall"));
	terrains.push_back(new Terrain(Vector3(-101.273, 0, -10.2017), 0, 22, 10, 295.839, 3, "Wall"));
	terrains.push_back(new Terrain(Vector3(-246.153, 0, -20.2017), 0, 22, 10, 3, 21.1293, "Wall"));
	terrains.push_back(new Terrain(Vector3(-246.153, 0, -80.2017), 0, 22, 10, 3, 19.1293, "Wall"));
	terrains.push_back(new Terrain(Vector3(-265.3887, 0, -89.354), 0, 22, 10, 39.725, 3, "Wall"));
	terrains.push_back(new Terrain(Vector3(-284.3725, 0, 84.472), 0, 22, 10, 3, 351.252, "Wall"));
	terrains.push_back(new Terrain(Vector3(-236.308, 0, 260.323), 0, 22, 10, 98.564, 3, "Wall"));
	terrains.push_back(new Terrain(Vector3(-186.628, 0, 168.187), 0, 22, 10, 3, 190.936, "Wall"));
	terrains.push_back(new Terrain(Vector3(371.538, 0, -100.3729), 0, 22, 10, 3, 223.8275, "Wall"));
	terrains.push_back(new Terrain(Vector3(371.538, 0, 62.02704), 0, 22, 10, 3, 20, "Wall"));
	terrains.push_back(new Terrain(Vector3(261.7488, 0, -205.3159), 0, 22, 10, 218.204, 3, "Wall"));
	//Door hitbox 
	terrains.push_back(new Terrain(Vector3(-272.215, 0, 73.66698), 0, 22, 10, 218.204, 3, "Door")); //+19

	//Items vector
	items.push_back(new InteractableObject(Vector3(-210.785, 16.0715, 75.3848), 0, 0, 50, "preview"));
	items.push_back(new InteractableObject(Vector3(-283.869, 16.0715, 95.1478), 0, 0, 50, "answer"));
	items.push_back(new InteractableObject(Vector3(-104.012, 0, 5.04312), 0, 5, 5, "box"));


	//Ground mesh
	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1), 1.0f);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//Museum//wood.tga");

}



void SceneMuseum::Update(double dt)
{
	fps = 1.f / dt;

	if (ShowPreview == false || CorrectAnswer == true)
	{
		Application::enableMouse = false;
		camera.Update(dt);
		//check for wall detection
		for (std::vector<Terrain*>::iterator it = terrains.begin(); it != terrains.end(); it++)
		{
			(*it)->solidCollisionBox(camera.position);
		}
	
	}

	if (MousePreview == true)
	{
		//Mouse Inputs
		static bool bLButtonState = false;
		int BUTTON_TOP = 52.9;
		int BUTTON_BOTTOM = 34.4;
		int BUTTON_LEFT = 40.7;
		int BUTTON_RIGHT = 51.5;

		int HOLDBUTTON_TOP = 18;
		int HOLDBUTTON_BOTTOM = 0.6;
		int HOLDBUTTON_LEFT = 58.8;
		int HOLDBUTTON_RIGHT = 76.5;
		if (Application::IsMousePressed(0))
		{
			bLButtonState = true;
			std::cout << "LBUTTON DOWN" << std::endl;
			//Converting Viewport space to UI space
			double x, y;
			Application::GetCursorPos(&x, &y);
			unsigned w = Application::GetWindowWidth();
			unsigned h = Application::GetWindowHeight();
			float posX = x / 10; //convert (0,800) to (0,80)
			float posY = 60 - y / 10;//convert (600,0) to (0,60)
			std::cout << "posX:" << posX << " , posY:" << posY << std::endl;
			if (EndGame1 == false)
			{
				if (posX > BUTTON_LEFT && posX < BUTTON_RIGHT && posY > BUTTON_BOTTOM && posY < BUTTON_TOP)
				{
					Application::enableMouse = false;
					CorrectAnswer = true;
					terrains.erase(terrains.begin() + 19);
					ShowFirstGame = false;
					Continue = true;
					indialogue = false;
					std::cout << "Hit!" << std::endl;
					//trigger user action or function
				}
			}
			else if (posX > HOLDBUTTON_LEFT && posX < HOLDBUTTON_RIGHT && posY > HOLDBUTTON_BOTTOM && posY < HOLDBUTTON_TOP)
			{
				if (Application::IsMousePressed(0))
				{
					AddSize += 10 * dt;
					MoveX += (10 * dt) / 2;
					std::cout << "AddSize = " << AddSize << std::endl;
					std::cout << "MoveX = " << MoveX << std::endl;

					if (MoveX > 36.9 && AddSize > 73)
					{
						ShowHoldingGame = false;
						MousePreview = false;
						Application::SwitchScene = 0;
						////Adding Box to inventory (IGNORE FIRST)
						//for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
						//{
						//	if ((*it)->gettype() == "box")
						//	{
						//		inventory.additem((*it));
						//		items.erase(items.begin() + 2);
						//		break;
						//	}
						//}
					}

				}
				std::cout << "Hit!" << std::endl;
			}
			else
			{
				std::cout << "Miss!" << std::endl;
			}
		}
		else if (bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;
			std::cout << "LBUTTON UP" << std::endl;
		}
		static bool bRButtonState = false;
		if (!bRButtonState && Application::IsMousePressed(1))
		{
			bRButtonState = true;
			std::cout << "RBUTTON DOWN" << std::endl;
		}
		else if (bRButtonState && !Application::IsMousePressed(1))
		{
			bRButtonState = false;
			std::cout << "RBUTTON UP" << std::endl;
		}
	}

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
	{
		std::cout << "Character position X: " << camera.position.x << std::endl;
		std::cout << "Character position Y: " << camera.position.y << std::endl;
		std::cout << "Character position Z: " << camera.position.z << std::endl;
	}

	if (Application::IsKeyPressed('X'))
	{
		std::cout << "Target position X: " << camera.target.x << std::endl;
		std::cout << "Target position Y: " << camera.target.y << std::endl;
		std::cout << "Target position Z: " << camera.target.z << std::endl;
	}

	if (Application::IsKeyPressed('K'))
	{
		movez += 10 * dt;
		std::cout << "Moved z: " << movez << std::endl;
	}
	if (Application::IsKeyPressed('I'))
	{
		movez -= 10 * dt;
		std::cout << "Moved z: " << movez << std::endl;
	}

	if (Application::IsKeyPressed('J'))
	{
		movex += 10 * dt;
		std::cout << "Moved x: " << movex << std::endl;
	}
	if (Application::IsKeyPressed('L'))
	{
		movex -= 10 * dt;
		std::cout << "Moved x: " << movex << std::endl;
	}

	if (Application::IsKeyPressed('O'))
	{
		scale += 10 * dt;
		std::cout << "End scale : " << scale << std::endl;
	}
	if (Application::IsKeyPressed('P'))
	{
		scale -= 10 * dt;
		std::cout << "End scale : " << scale << std::endl;

	}
	if (Application::IsKeyPressed('Y'))
	{
		rotate += 10 * dt;
		std::cout << "Rotated : " << rotate << std::endl;

	}
	if (Application::IsKeyPressed('U'))
	{
		rotate -= 10 * dt;
		std::cout << "Rotated : " << rotate << std::endl;

	}

	if (Application::IsKeyPressed('T') && EndGame1 == true)
	{
		ShowHoldingGame = true;
	}
	

}



void SceneMuseum::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight && lighton == true)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
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
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 499);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -499, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 499, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(499, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-499, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void SceneMuseum::RenderWalls()
{
	//for (std::vector<Terrain*>::iterator it = terrains.begin(); it != terrains.end(); it++)
	//{
	//	modelStack.PushMatrix();
	//	modelStack.Translate((*it)->getposition().x + movex, (*it)->getposition().y + (*it)->getheight() * 0.5, (*it)->getposition().z + movez);
	//	modelStack.Scale((*it)->getxwidth(), (*it)->getheight() + 100, (*it)->getzwidth());
	//	RenderMesh(meshList[GEO_CUBE], false);
	//	modelStack.PopMatrix();
	//}
	modelStack.PushMatrix();
	modelStack.Translate(54.5, 0, -113.507);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 205);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(93.77, 0, -235.091);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 60);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(161.5, 0, -120.507);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 217);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(206.5, 0, -2);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 110);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-107.9315, 0, 82);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 300);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50, 0, 130);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 115);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(97.606, 0, 216.3716);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 70);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(242, 0, 80);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 180);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(163, 0, 130);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 115);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-100.273, 0, -2.2017);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 293.69);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-237.153, 0, -21.76082);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 20);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-237.153, 0,-80);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 20);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-256.725, 0, -80);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 20);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-275.86 , 0, -80);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 40, 20);
	RenderMesh(meshList[GEO_WALLCORNER], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-275.77, 0, 85.866);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 312.079);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-275.86, 0, 251.47);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(20, 40, 20);
	RenderMesh(meshList[GEO_WALLCORNER], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-226.94, 0, 269.39);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 80);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-177.61, 0, 166.13);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 186.80);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-272.215, 0, 82.023);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 40.1209, 29.7171);
	RenderMesh(meshList[GEO_WALLDOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-255.54, 0, -51.028);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(20, 40.1209, 39);
	RenderMesh(meshList[GEO_WALLDOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(380.54, 0, 36.895);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 40.1209, 39);
	RenderMesh(meshList[GEO_WALLDOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(362.13, 0, 63.431);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(21, 40, 16.540);
	RenderMesh(meshList[GEO_WALLCORNER], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(260.982, 0, -199.02);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 219.59);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(350.982, 0, 80);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(20, 80, 40);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(380.54, 0, -93.718);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 80, 227.96);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(55, 0, 198);
	modelStack.Rotate(270, 0, 1, 0);
	modelStack.Scale(21, 40, 30);
	RenderMesh(meshList[GEO_WALLCURVED], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(143.4894, 0, 194.39);
	modelStack.Rotate(360, 0, 1, 0);
	modelStack.Scale(25, 40, 30);
	RenderMesh(meshList[GEO_WALLCURVED], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(57.0803, 0, -229.69);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(25, 40, 30);
	RenderMesh(meshList[GEO_WALLCURVED], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(138.77, 0, -233.091);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(25, 40, 30);
	RenderMesh(meshList[GEO_WALLCURVED], true);
	modelStack.PopMatrix();
}

void SceneMuseum::StartGame1()
{
	if (EndGame1 == false)
	{
		if (camera.position.x < -192.129 && camera.position.x > -225 && camera.position.z > 90 && camera.position.z < 100)
		{
			//std::cout << "YOURE HERE" << std::endl;
			RenderInteractableText();
			if (Application::IsKeyPressed('T'))
			{
				ShowPreview = true;
				MousePreview = true;
			}
			if (ShowPreview == true && Application::IsKeyPressed('R'))
			{
				ShowPreview = false;
				MousePreview = true;
				indialogue = false;
			}
			for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
			{
				if ((*it)->spherecollider(camera.target))
				{
					int interacttype = (*it)->interact();
					if (interacttext.str() == ""); //If there's nothing object the highlighted for interactions, add it in 
					{
						if (interacttype == 1 || ShowPreview == true)// 1 is look at
						{
							GameCam1 = camera;
							RenderingText = true;
							Application::enableMouse = true;
							//Goes to some orange background to view image
							camera.Init(Vector3(-270.713, 10, 100), Vector3(220.717, 40, 241.881), Vector3(0, 1, 0));
							RenderMeshOnScreen(meshList[GEO_MINIPIC1], 70, 25, 80, 70);

							dialogue = (*it)->lookat; //Set the dialogue vector to that of the current object
							currentline = dialogue.begin(); //Currentline is set at the look at description
							indialogue = true;//Set state to in dialogue
						}
						if ((*it)->gettype() == "preview")
						{
							interacttext << "Preview";
							break;
						}
					}
				}
			}
		}
		if (camera.position.x < -262 && camera.position.x > -280 && camera.position.z > 90 && camera.position.z < 116)
		{
			//std::cout << "YOURE HERE" << std::endl;
			RenderInteractableText();
			if (Application::IsKeyPressed('T') && ShowPreview == false)
			{
				ShowFirstGame = true;
				MousePreview = true;
				GameTisPressed = true;
			}
			for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
			{
				if ((*it)->spherecollider(camera.target))
				{
					int interacttype = (*it)->interact();
					if (interacttext.str() == ""); //If there's nothing object the highlighted for interactions, add it in 
					{
						if (interacttype == 1 || ShowFirstGame == true)// 1 is look at
						{
							GameCam1 = camera;
							RenderingText = true;
							Application::enableMouse = true;
							//Goes to some orange background to view image
							camera.Init(Vector3(-270.713, 10, 100), Vector3(220.717, 40, 241.881), Vector3(0, 1, 0));
							RenderMeshOnScreen(meshList[GEO_SELECTION], 70, 25, 80, 70);

							dialogue = (*it)->lookat; //Set the dialogue vector to that of the current object
							currentline = dialogue.begin(); //Currentline is set at the look at description
							indialogue = true;//Set state to in dialogue
						}
						if ((*it)->gettype() == "answer")
						{
							interacttext << "Answer";
							break;
						}
					}
				}
			}
		}

		if (ShowPreview == true)
		{
			GameCam1 = camera;
			RenderingText = true;
			Application::enableMouse = true;
			//Goes to some orange background to view image
			camera.Init(Vector3(-220.713, 10, 95), Vector3(220.717, 40, 241.881), Vector3(0, 1, 0));
			RenderMeshOnScreen(meshList[GEO_MINIPIC1], 40, 30, 20, 10);
		}

		if (Continue == true)
		{
			MousePreview = false;
			EndGame1 = true;
		}
		//if (ShowFirstGame == true)
		//{
		//	GameCam1 = camera;
		//	RenderingText = true;
		//	Application::enableMouse = true;
		//	//Goes to some orange background to view image
		//	camera.Init(Vector3(-270.713, 10, 100), Vector3(220.717, 40, 241.881), Vector3(0, 1, 0));
		//	RenderMeshOnScreen(meshList[GEO_SELECTION], 70, 25, 80, 70);
		//}
	}

}

void SceneMuseum::ExitMuseum()
{
	if (camera.position.x < -258 && camera.position.x > -267 && camera.position.z < -24.3 && camera.position.z > -83)
	{
		RenderInteractableText();
		StartInteraction();
	}

}

void SceneMuseum::StartInteraction()
{
	if (EndInteraction == false)
	{
		if (ShowHoldingGame == true)
		{
			MousePreview = true;
			Application::enableMouse = true;
			GameCam1 = camera;
			//Application::enableMouse = true;
			//Goes to some orange background to view image
			camera.Init(Vector3(-100, 10, 10), Vector3(220.717, 5, 241.881), Vector3(0, 1, 0));
			RenderMeshOnScreen(meshList[GEO_PIC], 40, 30, 80, 65);
			RenderMeshOnScreen(meshList[GEO_BOX], 4 + MoveX, 29, 3 + AddSize, 8.9);
		}

	}

}

void SceneMuseum::RenderUI()
{
	if (indialogue)
	{
		string dialoguetext = (*currentline);
		string currentname;
		if (dialoguetext[0] == '1')
			currentname = "Player name";
		else if (dialoguetext[0] == '2')
			currentname = name;
		dialoguetext = dialoguetext.substr(1);
		RenderNPCDialogue(dialoguetext, currentname);
		if (cooldown <= 0 && Application::IsKeyPressed('Z')) //Cooldown added to prevent spamming to pass the dialogues too fast
		{
			cooldown = 1;
			currentline++;
			if (currentline == dialogue.end())
			{
				indialogue = false;
				dialogue.clear();
			}
		}
	}
	else
	{
		modelStack.PushMatrix();
		RenderMeshOnScreen(meshList[GEO_INVENTORY], 8, 37, 33, 45);
		int ypos = 52;
		vector<InteractableObject*> inventorycontent = inventory.getstorage();
		for (std::vector<InteractableObject*>::iterator it = inventorycontent.begin(); it != inventorycontent.end(); it++)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], (*it)->gettype(), Color(0, 0, 0), 2, 2, ypos);
			ypos -= 2;

		}
		std::ostringstream ss;
		ss << "FPS: " << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 58, 68);
		RenderTextOnScreen(meshList[GEO_TEXT], interacttext.str(), Color(0.5, 0.5, 0.5), 5, 40 - (interacttext.str().length()), 30);
		interacttext.str("");
		modelStack.PopMatrix();
	}
}

void SceneMuseum::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	//glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		std::vector<std::pair<std::string, std::vector<int>>> result;
		//std::ifstream myFile("Fontdata.csv");
		//if (!myFile.is_open()) throw std::runtime_error("Could not open file");

		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//glEnable(GL_DEPTH_TEST);
}

void SceneMuseum::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);

}

void SceneMuseum::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 0);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}
void SceneMuseum::RenderInteractableText()
{
	RenderTextOnScreen(meshList[GEO_TEXT], "Interable object! Press 'T'", Color(0, 1, 0), 2, 0, 58);
}

void SceneMuseum::RenderNPCDialogue(std::string NPCText, std::string headerText)
{
	//float headerTextPos = 4.f;
	RenderMeshOnScreen(meshList[GEO_HEADER], 14.75, 19.25, 30, 6);
	//headerText.size()
	RenderTextOnScreen(meshList[GEO_TEXTINTEXTBOX], headerText, Color(0, 0, 0), 4, 14.5 - (headerText.size()), 17.5);	//header text
	RenderMeshOnScreen(meshList[GEO_TEXTBOX], 40, 8.75, 80, 17.5);
	string word;																	//automating text
	int wordpos = 0, ypos = 13, last = NPCText.find_last_of(" ");
	float xpos = 2.f;
	while (true)
	{
		word = NPCText.substr(wordpos, NPCText.find(" ", wordpos + 1) - wordpos);
		if (xpos + word.length() * 1.5 + 1 > 80)		//if new word will exceed screensize
		{
			ypos -= 3;
			xpos = 2;
		}
		RenderTextOnScreen(meshList[GEO_TEXT], word, Color(0, 0, 0), 3, xpos, ypos);
		if (wordpos > last)
			break;
		wordpos += word.length() + 1;
		xpos += 1.5 * word.length() + 1;
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

	RenderMesh(meshList[GEO_AXES], false);

	RenderWalls();

	//GROUND MESH
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_GROUND], false);
	modelStack.PopMatrix();

	//OBJ
	modelStack.PushMatrix();
	modelStack.Translate(90.2891, 20, -210.542);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(7, 7, 7);
	RenderMesh(meshList[GEO_ELEPHANT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(45.9141, -8.1529, -87.926);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(20, 25, 50);
	RenderMesh(meshList[GEO_PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(145.9141, -8.1529, -87.926);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(20, 25, 50);
	RenderMesh(meshList[GEO_PAINTING2], true);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(97.15, 0, 163.717);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(-51.626, 0, 0, 1);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_RICKSHAW], true);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(167.2672, 0, -4.8068);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(15, 15, 15);
	RenderMesh(meshList[GEO_GLASSTABLE], true);
	modelStack.PopMatrix();

	//Game1 OBJ
	modelStack.PushMatrix();
	modelStack.Translate(-210.785, 16.0715, 75.3848);
	modelStack.Scale(20, 20, 20);
	RenderMesh(meshList[GEO_MINIPIC1], true);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(-283.869, 16.0715, 95.1478);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(35, 35, 45);
	RenderMesh(meshList[GEO_SELECTION], true);
	modelStack.PopMatrix();

	//Game2 OBJ
	modelStack.PushMatrix();
	modelStack.Translate(-76.3144, 23, -10.252);
	modelStack.Scale(35, 35, 45);
	RenderMesh(meshList[GEO_PIC], true);
	modelStack.PopMatrix();

	//For interactable items
	for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
	{
		modelStack.PushMatrix();
		modelStack.Translate((*it)->getposition().x, (*it)->getposition().y, (*it)->getposition().z);
		modelStack.Rotate((*it)->getangle(), 0, 1, 0);
		modelStack.Scale((*it)->getscale(), (*it)->getscale(), (*it)->getscale());
		if ((*it)->gettype() == "box")
		{
			RenderMesh(meshList[GEO_ITEM1], true);
		}
		modelStack.PopMatrix();
	}

	StartGame1();
	StartInteraction();
	ExitMuseum();
	RenderUI();
}

void SceneMuseum::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}