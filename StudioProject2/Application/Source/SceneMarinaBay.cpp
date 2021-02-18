#include "SceneMarinaBay.h"
#include "GL\glew.h"
#include "Application.h"

#include "LoadTGA.h"
#include "Utility.h"
#include "shader.hpp"
#include <Mtx44.h>
#include"MeshBuilder.h"

SceneMarinaBay::SceneMarinaBay()
{
}

SceneMarinaBay::~SceneMarinaBay()
{
}

void SceneMarinaBay::Init()
{
	//======Initialising variables========
	pointerX = 2;
	pointerY = 11;
	enemyHealthPos = 20.f;
	playerHealthPos = 60.f;
	enemyHealth = playerHealth = 0;

	//======Matrix stack========
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
	//==========================
	//camera.Init(Vector3(0, 8, 0), Vector3(0, 8, 5), Vector3(0, 1, 0));
	camera.Init(Vector3(90, 40, 240), Vector3(0, 8, 240), Vector3(0, 1, 0));
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
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.0f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.0f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.0f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.0f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.0f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");
	//main boat
	meshList[GEO_BOAT] = MeshBuilder::GenerateOBJMTL("boat", "OBJ//Marina//boat2.obj", "OBJ//Marina//boat.mtl");

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
	meshList[GEO_MC] = MeshBuilder::GenerateOBJMTL("MC", "OBJ//Marina//advancedCharacter.obj", "OBJ//Marina//advancedCharacter.obj.mtl");
	meshList[GEO_MC]->textureID = LoadTGA("Image//Marina//skin_adventurer.tga");
}



void SceneMarinaBay::Update(double dt)
{
	fps = 1.f / dt;
	if (!fight)
		camera.Update(dt);

	if (Application::IsKeyPressed('5'))
	{
		light[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
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
	if (Application::IsKeyPressed('I'))
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
	if (Application::IsKeyPressed('H'))
	{
		attackSelected = true;
		playerAction = A_ATTACK1;
	}
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = x / 10; //convert (0,800) to (0,80)
		float posY = 60 - (y / 10); //convert (600,0) to (0,60)
		int BUTTON_LEFT = 70, BUTTON_RIGHT = 80, BUTTON_BOTTOM = 57, BUTTON_TOP = 60;
		std::cout << "posX:" << posX << " , posY:" << posY << std::endl;
		if (posX > BUTTON_LEFT && posX < BUTTON_RIGHT && posY > BUTTON_BOTTOM && posY < BUTTON_TOP)
		{
			std::cout << "Hit!" << std::endl;
			//trigger user action or function
		}
		if (fight && !attackSelected && cooldownTimer <= 0)	//if in fight and attack is not playing
		{

			{
				//keyboard support might get rid of it entirely depending
				/*if (Application::IsKeyPressed('F'))
				{
					switch (playerAction)
					{
					case (A_ATTACK):
						fightSelected = true;
						playerAction = A_ATTACK1;
						pointerX += 21;
						pointerY -= 3;
					case (A_ITEMS):
						itemsSelected = true;
						playerAction = A_ITEM1;
					case (A_RUN):
						fightDia = true;
						fightDia = "He can probably shoot you before you try.";
						break;
					case (A_ATTACK1):
						attackSelected = true;
					}
					cooldownTimer = 0.3f;
				}*/
				//else if (Application::IsKeyPressed(VK_UP))		//if up arrow is pressed while in the fight
				//{
				//	if (!fightSelected && !itemsSelected)	//when on the first 3 options
				//	{
				//		playerAction = static_cast<ACTION_TYPE>((playerAction + 1) % (A_RUN + 1));	//updates playerAction
				//		pointerY = playerAction * 5 + 1;	//reads which action it is on and moves the pointer
				//	}
				//	else if (fightSelected)
				//	{
				//		playerAction = static_cast<ACTION_TYPE>((playerAction + 1) % (A_ATTACK4 + 1 - A_RUN));	//minuses the previous enum on the list
				//		pointerY = (playerAction - A_RUN + 1) * 6 + 2;
				//	}
				//	else if (itemsSelected)
				//	{
				//		playerAction = static_cast<ACTION_TYPE>((playerAction + 1) % (A_ITEM4 + 1 - A_ATTACK4));
				//		pointerY = (playerAction - A_ATTACK4 + 1) * 6 + 2;
				//	}
				//	cooldownTimer = 0.3f;
				//}
			}
		}
	}

	if (attackSelected)
	{
		switch (playerAction)
		{
		case (A_ATTACK1):			//MC goes big
			if (!attack1Hit)
			{
				if (attack1Scale < 5)		//go big
					attack1Scale += 10 * dt;
				else if (attack1Angle < 90)		//rotate onto enemy
					attack1Angle += 300 * dt;
				else
					attack1Hit = true;		//has hit enemy
			}
			else
			{
				if (attack1Angle > 0)
					attack1Angle -= 300 * dt;
				else if (attack1Scale > 0.7)
					attack1Scale -= 10 * dt;
				else			//resolution of attack
				{
					enemyHealthLost = 35.f;
					attack1Hit = false;
					attackSelected = false;
					playerTurn = false;
				}
			}
			//case (A_ATTACK2):
		}
		
	}


	if (playerHealthLost > 0)
	{	//health lost will be set to 100 so since the scaling is 20 the speed would be the same as health lost / 5
		int speedOfHealthLost = 30.f;
		playerHealthLost -= speedOfHealthLost * dt;
		playerHealth += speedOfHealthLost * 0.2 * dt;
		playerHealthPos -= speedOfHealthLost * 0.1 * dt;	//dependent on health's scaling
	}
	else if (enemyHealthLost > 0)
	{
		int speedOfHealthLost = 30.f;
		enemyHealthLost -= speedOfHealthLost * dt;
		enemyHealth += speedOfHealthLost * 0.2 * dt;
		enemyHealthPos -= speedOfHealthLost * 0.1 * dt;	//dependent on health's scaling
	}
	//==================Updating timers===========
	if (cooldownTimer > 0)
		cooldownTimer -= dt;
}

void SceneMarinaBay::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneMarinaBay::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y + 200, camera.position.z);

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
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 499, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
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

void SceneMarinaBay::RenderUI()
{
	modelStack.PushMatrix();
	std::ostringstream ss;
	ss << "x: " << x;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 35, 29);
	ss.str("");
	ss << "z: " << z;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 35, 27);
	ss.str("");
	ss << "scale: " << scale;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 35, 25);
	modelStack.PopMatrix();
}

void SceneMarinaBay::RenderText(Mesh* mesh, std::string text, Color color)
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
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//glEnable(GL_DEPTH_TEST);
}

void SceneMarinaBay::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneMarinaBay::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
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
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
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

	//RenderMeshOnScreen(meshList[GEO_INVENTORY], 40, 20, 30, 30);

	//Skybox
	RenderSkybox();

	//========================================================
	//modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_AXES], false);
	
	//infinity pool (to be changed to fix the need for face culling, maybe)
	modelStack.PushMatrix();
	modelStack.Translate(47, 0.0001, z + 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(30, x + 317, 1);
	glDisable(GL_CULL_FACE);
	RenderMesh(meshList[GEO_WATER], true);
	glEnable(GL_CULL_FACE);
	modelStack.PopMatrix();

	//boat
	modelStack.PushMatrix();
	modelStack.Translate(0, -27.7, 0);
	modelStack.Scale(40, 25, 50);
	RenderMesh(meshList[GEO_BOAT], true);
	modelStack.PopMatrix();

	

	//infinity poolside
	for (int z = 0; z > -170; z -= 30)
	{
		//trees
		modelStack.PushMatrix();
		modelStack.Translate(23, 0, z);
		modelStack.Scale(10, 20, 10);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();

		//all the chairs

		//chair right after tree
		modelStack.PushMatrix();
		modelStack.Translate(23, 0, z + 5);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[GEO_CHAIR], true);
		modelStack.PopMatrix();

		//chair in the middle of trees
		modelStack.PushMatrix();
		modelStack.Translate(23, 0, z + 15);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[GEO_CHAIR], true);
		modelStack.PopMatrix();

		//chair directly before the tree
		modelStack.PushMatrix();
		modelStack.Translate(23, 0, z - 5);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderMesh(meshList[GEO_CHAIR], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(x, 0, z);
	modelStack.Scale(scale, 2*scale, scale);
	RenderMesh(meshList[GEO_TALLTREE], true);
	modelStack.PopMatrix();
	
	if (fight)
	{
		//mc
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, 200);
		if (playerAction!=A_ATTACK1)
			modelStack.Scale(0.7f, 0.7f, 0.7f);
		else
		{
			modelStack.Rotate(attack1Angle, 1, 0, 0);
			modelStack.Scale(attack1Scale, attack1Scale, attack1Scale);
		}
		RenderMesh(meshList[GEO_MC], true);
		modelStack.PopMatrix();
	}

	if (fight && !attackSelected)
	{
		//dragon
		RenderTextOnScreen(meshList[GEO_TEXT], "Evil Guy", Color(0, 1, 0), 5, 0, 55);
		RenderMeshOnScreen(meshList[GEO_HEALTH], 10, 53, 20, 2);
		RenderMeshOnScreen(meshList[GEO_LOSTHEALTH], enemyHealthPos, 53, enemyHealth, 2);
		//MC
		RenderTextOnScreen(meshList[GEO_TEXT], "You", Color(0, 1, 0), 5, 72, 55);
		RenderMeshOnScreen(meshList[GEO_HEALTH], 70, 53, 20, 2);
		RenderMeshOnScreen(meshList[GEO_LOSTHEALTH], playerHealthPos, 53, playerHealth, 2);
		//fighting layout
		RenderMeshOnScreen(meshList[GEO_LAYOUT], 40, 15, 80, 30);
		RenderTextOnScreen(meshList[GEO_TEXT], "Attack", Color(0, 0, 0), 4, 4, 11);
		RenderTextOnScreen(meshList[GEO_TEXT], "Items", Color(0, 0, 0), 4, 4, 6);
		RenderTextOnScreen(meshList[GEO_TEXT], "Run", Color(0, 0, 0), 4, 4, 1);
		RenderTextOnScreen(meshList[GEO_TEXT], ">", Color(0, 0, 0), 4, pointerX, pointerY);
		if (fightSelected)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Placeholder", Color(0, 0, 0), 5, 25, 8);	//attack1
			RenderTextOnScreen(meshList[GEO_TEXT], "Placeholder2", Color(0, 0, 0), 5, 25, 2);	//attack2
		}
		else if (itemsSelected)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "Placeholder3", Color(0, 0, 0), 5, 25, 8);
			RenderTextOnScreen(meshList[GEO_TEXT], "Placeholder4", Color(0, 0, 0), 5, 25, 8);
		}
	}
	else if (!fight)
	{
		RenderMeshOnScreen(meshList[GEO_INVENTORY], 8, 37, 33, 45);
		RenderUI();
	}
}

void SceneMarinaBay::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
