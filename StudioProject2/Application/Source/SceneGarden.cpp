#include "SceneGarden.h"
#include "GL\glew.h"
#include "Application.h"
#include "LoadTGA.h"
#include "Utility.h"
#include "shader.hpp"
#include <Mtx44.h>
#include"MeshBuilder.h"

SceneGarden::SceneGarden()
{
}

SceneGarden::~SceneGarden()
{
}

void SceneGarden::Init()
{
	//======Matrix stack========
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//==========================
	camera.Init(Vector3(0, 5, 0), Vector3(0, 5, 5), Vector3(0, 1, 0));
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
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(0.5f, 0.5f, 0.5f), 1);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sphere", Color(0.5, 0.5, 0.5), 10, 10, 1);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SPHERE]->material.kShininess = 1.f;
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Lightball", Color(1, 1, 1), 10, 10, 10);

	meshList[GEO_TORUS1] = MeshBuilder::GenerateTorus("torus1", Color(0.5, 0.5, 0.5), 30, 30, 5,0.1);
	meshList[GEO_TORUS2] = MeshBuilder::GenerateTorus("torus2", Color(0.5, 1, 0.5), 30, 30, 5, 0.1);

	meshList[GEO_GRASSFLOOR] = MeshBuilder::GenerateQuad("grassfloor",1,1,Color(1,1,1), 30);
	meshList[GEO_GRASSFLOOR]->textureID = LoadTGA("Image//garden//grassfloor.tga");
	meshList[GEO_GRASSFLOOR]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_GRASSFLOOR]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_GRASSFLOOR]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_GRASSFLOOR]->material.kShininess = 1.f;

	meshList[GEO_POND] = MeshBuilder::GenerateQuad("pondwater", 1, 1, Color(1, 1, 1), 10);
	meshList[GEO_POND]->textureID = LoadTGA("Image//watertexture.tga");
	meshList[GEO_POND]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_POND]->material.kDiffuse.Set(0.4f, 0.4f, 0.4f);
	meshList[GEO_POND]->material.kSpecular.Set(0.2f, 0.2f, 0.2f);

	meshList[GEO_PONDBED] = MeshBuilder::GenerateQuad("pondwater", 1, 1, Color(1, 1, 1), 10);
	meshList[GEO_PONDBED]->textureID = LoadTGA("Image//garden//pondbed.tga");
	meshList[GEO_PONDBED]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_PONDBED]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_PONDBED]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_PONDBED]->material.kShininess = 1.f;

	meshList[GEO_GAZEBO] = MeshBuilder::GenerateOBJMTL("gazebo", "OBJ//garden//gazebo.obj", "OBJ//garden//gazebo.mtl");
	meshList[GEO_GAZEBO]->textureID = LoadTGA("Image//garden//gazebo.tga");

	meshList[GEO_PATH] = MeshBuilder::GenerateOBJMTL("path", "OBJ//garden//path.obj", "OBJ//garden//path.mtl");
	meshList[GEO_PATH]->textureID = LoadTGA("Image//garden//pathtexture.tga");

	meshList[GEO_PATHT] = MeshBuilder::GenerateOBJMTL("path", "OBJ//garden//patht.obj", "OBJ//garden//path.mtl");
	meshList[GEO_PATHT]->textureID = LoadTGA("Image//garden//pathtexture.tga");

	meshList[GEO_STICK] = MeshBuilder::GenerateOBJMTL("path", "OBJ//garden//stick.obj", "OBJ//garden//stick.mtl");
	meshList[GEO_STICK]->textureID = LoadTGA("Image//garden//stick.tga");

	meshList[GEO_TREE1] = MeshBuilder::GenerateOBJMTL("tree1", "OBJ//garden//tree.obj", "OBJ//garden//tree.mtl");
	meshList[GEO_TREE2] = MeshBuilder::GenerateOBJMTL("tree2", "OBJ//garden//tree_fat.obj", "OBJ//garden//tree.mtl");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");

	meshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("Testing", Color(1, 1, 1), 1.0f);
	meshList[GEO_INVENTORY]->textureID = LoadTGA("Image//inventory.tga");

	//Skybox quads
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.0f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//garden//gardenleft.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.0f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//garden//gardenright.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.0f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//garden//gardenfront.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.0f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//garden//gardenback.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.0f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//garden//gardentop.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.0f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//garden//gardenbottom.tga");

	//Creating entities in the Entity list
	terrains.push_back(new Terrain(Vector3(70, 0, 64), 0, 22, 10, 3, 3, "tree1"));
	terrains.push_back(new Terrain(Vector3(24, 0, 47), 0, 18, 10, 3, 3, "tree1"));
	terrains.push_back(new Terrain(Vector3(-25, 0, 80), 0, 18, 10, 3, 3, "tree1"));
	terrains.push_back(new Terrain(Vector3(-25, 0, 16), 0, 11, 10, 3, 3, "tree1"));
	terrains.push_back(new Terrain(Vector3(-43, 0, 47), 0, 18, 10, 3, 3, "tree1"));
	terrains.push_back(new Terrain(Vector3(-43, 0, -24), 0, 17, 10, 3, 3, "tree1"));
	terrains.push_back(new Terrain(Vector3(88, 0, -17), 0, 13, 10, 3, 3, "tree1"));
	terrains.push_back(new Terrain(Vector3(112, 0, -75), 0, 13, 10, 3, 3, "tree1"));

	terrains.push_back(new Terrain(Vector3(-47, 0, 76), 0, 13, 10, 3, 3, "tree2"));
	terrains.push_back(new Terrain(Vector3(1, 0, 54), 0, 17, 13, 3, 3, "tree2"));
	terrains.push_back(new Terrain(Vector3(130, 0, -60), 0, 17, 10, 3, 3, "tree2"));
	terrains.push_back(new Terrain(Vector3(80, 0, 27), 0, 15, 10, 3, 3, "tree2"));
	terrains.push_back(new Terrain(Vector3(-124, 0, -129), 0, 19, 10, 3, 3, "tree2"));
	terrains.push_back(new Terrain(Vector3(-120, 0, 32), 0, 14, 10, 3, 3, "tree2"));
	terrains.push_back(new Terrain(Vector3(-123, 0, -42), 0, 21, 10, 3, 3, "tree2"));

	terrains.push_back(new Terrain(Vector3(0, 0, -150), 0, 1, 1, 200, 200, "pond"));

	terrains.push_back(new Terrain(Vector3(59, 0, 44), 0, 1, 5, 5, 2, "gazebo"));
	terrains.push_back(new Terrain(Vector3(52, 0, 44), 0, 1, 5, 5, 2, "gazebo"));

	terrains.push_back(new Terrain(Vector3(59, 0, 58), 0, 1, 5, 5, 2, "gazebo"));
	terrains.push_back(new Terrain(Vector3(52, 0, 58), 0, 1, 5, 5, 2, "gazebo"));

	terrains.push_back(new Terrain(Vector3(62, 0, 51), 0, 1, 5, 3.5, 14, "gazebo"));
	terrains.push_back(new Terrain(Vector3(49, 0, 51), 0, 1, 5, 3.5, 14, "gazebo"));

	items.push_back(new InteractableObject(Vector3(0, 2, 0), 0, 1, 2, "stick"));
}

void SceneGarden::Update(double dt)
{
	fps = 1.f / dt;
	if (cooldown > 0)
		cooldown -= dt;
	if (minigame == 0)
	{
		camera.Update(dt);
		//Check collisions
		{
			for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
			{
				if ((*it)->gettype() == "stick")
				{
					if ((*it)->spherecollider(camera.target)) // Checks if the target is within a radius of the stick
					{
						std::cout << "touching stick" << std::endl;
						RenderTextOnScreen(meshList[GEO_TEXT], "Stick", Color(1, 1, 0.6), 5, 0, 0);
						if (Application::IsKeyPressed('E'))
						{
							minigame = 1;
						}
					}
				}
			}
			for (std::vector<Terrain*>::iterator it = terrains.begin(); it != terrains.end(); it++)
			{
				(*it)->solidCollisionBox(camera.position);
			}
		}
	}
	else if (minigame == 1)
	{
		circlescale1 -= circlespeed * dt;
		if (Application::IsKeyPressed('E') && cooldown <= 0)
		{
			if (circlescale1 >= circlescale2 - 0.05 && circlescale1 <= circlescale2 + 0.05)
			{
				cout << "Good" << endl;
				circlespeed += 0.2f;
			}
			else
				cout << "Bad" << endl;
			cooldown = 0.5;
			circlescale1 = 3;
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

	if (Application::IsKeyPressed('C') && minigame != 0) //exit minigame
	{
		camera = prevcamera;
		minigame = 0;
	}
	else if (Application::IsKeyPressed('V') && minigame != 1) //Enter minigame
	{
		prevcamera = camera;
		camera.Init(Vector3(0, 50, -150), Vector3(0, 0, -150), Vector3(0, 0, 1));
		circlescale2 = 1;
		circlescale1 = 3;
		circlespeed = 0.5f;
		minigame = 1;
	}

	if (Application::IsKeyPressed('I'))
		movez += 10 * dt;
	if (Application::IsKeyPressed('K'))
		movez -= 10 * dt;
	if (Application::IsKeyPressed('J'))
		movex += 10 * dt;
	if (Application::IsKeyPressed('L'))
		movex -= 10 * dt;
	if (Application::IsKeyPressed('U'))
		scale += 10 * dt;
	if (Application::IsKeyPressed('O'))
		scale -= 10 * dt;

}

void SceneGarden::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneGarden::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);

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

void SceneGarden::RenderUI()
{
	modelStack.PushMatrix();
	RenderMeshOnScreen(meshList[GEO_INVENTORY], 8, 37, 33, 45);
	std::ostringstream ss;
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 58, 68);
	modelStack.PopMatrix();
}

void SceneGarden::Renderminigame1()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 10, -150);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_TORUS1], false);
	modelStack.PopMatrix();
	if (circlescale1 > 0.1)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 10, -150);
		modelStack.Scale(circlescale1, circlescale1, circlescale1);
		RenderMesh(meshList[GEO_TORUS2], false);
		modelStack.PopMatrix();
	}
	else
	{
		circlescale1 = 3;
	}
}

void SceneGarden::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneGarden::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneGarden::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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

void SceneGarden::Render()
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
	modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_AXES], false);
	//entire pond
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, -5, -150);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(200, 200, 200);
		RenderMesh(meshList[GEO_PONDBED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, -2.5, -247.50);
		modelStack.Rotate(-45, 1, 0, 0);
		modelStack.Scale(200, 7.1, 200);
		RenderMesh(meshList[GEO_PONDBED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, -2.5, -52.50);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Rotate(-45, 1, 0, 0);
		modelStack.Scale(200, 7.1, 200);
		RenderMesh(meshList[GEO_PONDBED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-97.5, -2.5, -150);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Rotate(-45, 1, 0, 0);
		modelStack.Scale(200, 7.1, 200);
		RenderMesh(meshList[GEO_PONDBED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(97.5, -2.5, -150);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Rotate(-45, 1, 0, 0);
		modelStack.Scale(200, 7.1, 200);
		RenderMesh(meshList[GEO_PONDBED], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 0.01, -150);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(200, 200, 200);
		glDisable(GL_CULL_FACE);
		RenderMesh(meshList[GEO_POND], true);
		glEnable(GL_CULL_FACE);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(500,500,500);
	RenderMesh(meshList[GEO_GRASSFLOOR], true);
	modelStack.PopMatrix();

	//Path locations
	{
		for (int i = -250; i <250; i += 18.0)
		{
			modelStack.PushMatrix();
			if (i < 60 && i > 50)
			{
				modelStack.Translate(i, -2.29, -0.18);
				modelStack.Rotate(180, 0, 1, 0);
				modelStack.Scale(2.3, 2.3, 2.3);
				RenderMesh(meshList[GEO_PATHT], true);
				modelStack.Rotate(90, 0, 1, 0);
				modelStack.Translate(7.9, 0, 0.1);
				RenderMesh(meshList[GEO_PATH], true);
				modelStack.Translate(7.9, 0, 00);
				RenderMesh(meshList[GEO_PATH], true);
				modelStack.Translate(6.5, 0.7, 0);
				modelStack.Rotate(90, 0, 1, 0);
				modelStack.Scale(0.07, 0.07, 0.07);
				RenderMesh(meshList[GEO_GAZEBO], true);
			}
			else
			{
				modelStack.Translate(i, -2.29, 0);
				modelStack.Scale(2.3, 2.3, 2.3);
				RenderMesh(meshList[GEO_PATH], true);
			}
			modelStack.PopMatrix();
		}
	}

	//Render interactable items
	{
		for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*it)->getposition().x, (*it)->getposition().y, (*it)->getposition().z);
			modelStack.Rotate((*it)->getangle(),0,1,0);
			modelStack.Scale((*it)->getscale(), (*it)->getscale(), (*it)->getscale());
			if ((*it)->gettype() == "stick")
			{
				RenderMesh(meshList[GEO_STICK], true);
				if (hitboxshow)
				{
					modelStack.PopMatrix();
					modelStack.PushMatrix();
					modelStack.Translate((*it)->getposition().x, (*it)->getposition().y, (*it)->getposition().z);
					modelStack.Scale((*it)->getradius(), (*it)->getradius(), (*it)->getradius());
					RenderMesh(meshList[GEO_SPHERE], FALSE);
				}
			}
			modelStack.PopMatrix();
		}
	}
	//Render terrain entities
	{
		for (std::vector<Terrain*>::iterator it = terrains.begin(); it != terrains.end(); it++)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*it)->getposition().x, (*it)->getposition().y, (*it)->getposition().z);
			modelStack.Rotate((*it)->getangle(), 0, 1, 0);
			modelStack.Scale((*it)->getscale(), (*it)->getscale(), (*it)->getscale());
			if ((*it)->gettype() == "tree1")
				RenderMesh(meshList[GEO_TREE1], true);
			else if ((*it)->gettype() == "tree2")
				RenderMesh(meshList[GEO_TREE2], true);
			if (hitboxshow)
			{
				modelStack.PopMatrix();
				modelStack.PushMatrix();
				modelStack.Translate((*it)->getposition().x, (*it)->getposition().y + (*it)->getheight()*0.5, (*it)->getposition().z);
				modelStack.Scale((*it)->getxwidth(), (*it)->getheight(), (*it)->getzwidth());
				RenderMesh(meshList[GEO_CUBE], false);
			}
			modelStack.PopMatrix();
		}
	}
	//modelStack.PushMatrix();
	//modelStack.Translate(movex, 2, movez);
	//modelStack.Scale(1,5,2);
	//RenderMesh(meshList[GEO_CUBE], FALSE);
	//modelStack.PopMatrix();

	if (minigame == 0)
		RenderUI();
	else if(minigame == 1)
		Renderminigame1();
}

void SceneGarden::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
