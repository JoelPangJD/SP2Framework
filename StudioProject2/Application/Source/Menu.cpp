#include "Menu.h"
#include "GL\glew.h"
#include "LoadTGA.h"
#include "Utility.h"


Menu::Menu()
{
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

void Menu::renderMenuScreen()
{
	meshList[GEO_MENU] = MeshBuilder::GenerateQuad("Menu", Color(1, 1, 1), 1.0f);
	//meshList[GEO_MENU]->textureID = LoadTGA("Image//inventory.tga");
	RenderMeshOnScreen(meshList[GEO_MENU], posX, posY, width, height, modelStack, viewStack, projectionStack, m_parameters);


}

void Menu::renderPause()
{
	meshList[GEO_PAUSE] = MeshBuilder::GenerateQuad("Pause", Color(1, 1, 1), 1.0f);
	//meshList[GEO_PAUSE]->textureID = LoadTGA("Image//menu.tga");

	Application::enableMouse = true;
	PauseCam = camera;
	camera.Init(Vector3(camera.position.x, camera.position.y, camera.position.z), Vector3(camera.target.x, camera.target.y, camera.target.z), Vector3(0, 1, 0));

	RenderMeshOnScreen(meshList[GEO_PAUSE], posX, posY, width, height, modelStack, viewStack, projectionStack, m_parameters);

	
}

void Menu::renderHelp()
{
	meshList[GEO_HELP] = MeshBuilder::GenerateQuad("Help", Color(1, 1, 1), 1.0f);
	//meshList[GEO_HELP]->textureID = LoadTGA("Image//inventory.tga");

	RenderMeshOnScreen(meshList[GEO_HELP], posX, posY, width, height, modelStack, viewStack, projectionStack, m_parameters);
	

}

