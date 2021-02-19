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
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");

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

	//OBJ FOR MINIGAME1
	meshList[GEO_MINIPIC1] = MeshBuilder::GenerateQuad("Minigame pic1", Color(1, 1, 1), 1.0f);
	meshList[GEO_MINIPIC1]->textureID = LoadTGA("Image//Museum//photomain.tga");
	meshList[GEO_MINIPIC1]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_MINIPIC1]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MINIPIC1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MINIPIC1]->material.kShininess = 1.f;
	meshList[GEO_MINIPIC2] = MeshBuilder::GenerateQuad("Answer for color blind photo", Color(1, 1, 1), 1.0f);
	meshList[GEO_MINIPIC2]->textureID = LoadTGA("Image//Museum//photomain.tga");
	meshList[GEO_MINIPIC2]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_MINIPIC2]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_MINIPIC2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_MINIPIC2]->material.kShininess = 1.f;
	meshList[GEO_COLORBLINDPIC1] = MeshBuilder::GenerateQuad("Answer for color blind photo", Color(1, 1, 1), 1.0f);
	meshList[GEO_COLORBLINDPIC1]->textureID = LoadTGA("Image//Museum//4colorblindmain.tga");
	meshList[GEO_COLORBLINDPIC1]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_COLORBLINDPIC1]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_COLORBLINDPIC1]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_COLORBLINDPIC1]->material.kShininess = 1.f;

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


	//Ground mesh
	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1), 1.0f);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//Museum//wood.tga");

}



void SceneMuseum::Update(double dt)
{
	fps = 1.f / dt;

	if (ActivateGame1 == false)
	{
		camera.Update(dt);
		//check for wall detection
		for (std::vector<Terrain*>::iterator it = terrains.begin(); it != terrains.end(); it++)
		{
			(*it)->solidCollisionBox(camera.position);
		}
	}

	else if (ActivateGame1 == true)
	{
		GameCam1 = camera;
		//Goes to some orange background to view image
		camera.Init(Vector3(-220.713, 10, 242.881), Vector3(220.717, 40, 241.881), Vector3(0, 1, 0));
		RenderingMesh = true;
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

	if (camera.position.z > 90 && camera.position.z < 120)
	{
		std::cout << "YOURE HERE" << std::endl;
		if (Application::IsKeyPressed('E'))
			ActivateGame1 = true;
		if (Application::IsKeyPressed('R'))
			ActivateGame1 = false;
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

void SceneMuseum::RenderUI()
{
	modelStack.PushMatrix();
	std::ostringstream ss;
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 35, 29);
	modelStack.PopMatrix();
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
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);
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

void SceneMuseum::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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

	//Minigame1 OBJ
	modelStack.PushMatrix();
	modelStack.Translate(-194.785, 16.0715, 75.3848);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_MINIPIC1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-217.385, 16.0715, 75.3848);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_COLORBLINDPIC1], true);
	modelStack.PopMatrix();


	if (RenderingMesh == true)
	{
		RenderMeshOnScreen(meshList[GEO_MINIPIC1], 10, 30, 20, 10);
		RenderMeshOnScreen(meshList[GEO_COLORBLINDPIC1], 50, 30, 20, 10);
	}

	if (RenderingText == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "PRESS E ", Color(0, 1, 0), 2, 0, 54);
	}
	RenderUI();
}

void SceneMuseum::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}