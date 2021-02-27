#include "Menu.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "Utility.h"


Menu::Menu()
{
	meshList[GEO_MENU] = MeshBuilder::GenerateQuad("Menu", Color(1, 1, 1), 1.0f);
	meshList[GEO_MENU]->textureID = LoadTGA("Image//menu.tga");
	//meshList[GEO_PAUSE] = MeshBuilder::GenerateQuad("Pause", Color(1, 1, 1), 1.0f);
	//meshList[GEO_PAUSE]->textureID = LoadTGA("Image//helpscreen.tga");
	meshList[GEO_HELP] = MeshBuilder::GenerateQuad("Help", Color(1, 1, 1), 1.0f);
	meshList[GEO_HELP]->textureID = LoadTGA("Image//helpscreen.tga");
	this->posX = 0;
	this->posY = 0;
	this->width = 0;
	this->height = 0;
}

Menu::Menu(float posX, float posY, float width, float height)
{
	this->posX = posX;
	this->posY = posY;
	this->width = width;
	this->height = height;

}

Menu::~Menu()
{
}

void Menu::RenderMesh(Mesh* mesh, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Menu::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
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
	RenderMesh(mesh,modelStack, viewStack, projectionStack, m_parameters); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}


void Menu::renderMenuScreen()
{
	RenderMeshOnScreen(meshList[GEO_MENU], posX, posY, width, height, modelStack, viewStack, projectionStack, m_parameters);
	std::cout << "YOU ARE RENDERING MENU" << std::endl;
}

void Menu::renderPause()
{
	if (Application::IsKeyPressed('P'))
	{
		Application::enableMouse = true;
		PauseCam = camera;
		camera.Init(Vector3(camera.position.x, camera.position.y, camera.position.z), Vector3(camera.target.x, camera.target.y, camera.target.z), Vector3(0, 1, 0));

		RenderMeshOnScreen(meshList[GEO_PAUSE], posX, posY, width, height, modelStack, viewStack, projectionStack, m_parameters);
	
	}
}

void Menu::renderHelp()
{
	if (Application::IsKeyPressed('P'))
	{
		RenderMeshOnScreen(meshList[GEO_HELP], posX, posY, width, height, modelStack, viewStack, projectionStack, m_parameters);
		std::cout << "DID YOU SAY HELP" << std::endl;
		Application::enableMouse = true;
		PauseCam = camera;
		camera.Init(Vector3(camera.position.x, camera.position.y, camera.position.z), Vector3(camera.target.x, camera.target.y, camera.target.z), Vector3(0, 1, 0));

	}
	

}

