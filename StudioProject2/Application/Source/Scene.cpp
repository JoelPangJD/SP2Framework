#include "Scene.h"
#include "GL\glew.h"
#include "Application.h"
#include "LoadTGA.h"
#include "Utility.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include <Mtx44.h>

Scene::Scene()
{
	baseMeshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	baseMeshList[GEO_TEXT]->textureID = LoadTGA("Image//font.tga");
	baseMeshList[GEO_TEXTBOX] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	baseMeshList[GEO_TEXTBOX]->textureID = LoadTGA("Image//Marina//textbox.tga");
	baseMeshList[GEO_HEADER] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	baseMeshList[GEO_HEADER]->textureID = LoadTGA("Image//Marina//header.tga");
	baseMeshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("Testing", Color(1, 1, 1), 1.0f);
	baseMeshList[GEO_INVENTORY]->textureID = LoadTGA("Image//inventory.tga");

}

void Scene::RenderMesh(Mesh* mesh, bool enableLight, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
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

void Scene::RenderUI(float &cooldown, float fps, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
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
		RenderNPCDialogue(dialoguetext, currentname, modelStack, viewStack, projectionStack, m_parameters);
		if (cooldown <= 0 && Application::IsKeyPressed('E')) //Cooldown added to prevent spamming to pass the dialogues too fast
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
		RenderMeshOnScreen(baseMeshList[GEO_INVENTORY], 8, 37, 33, 45, modelStack, viewStack, projectionStack, m_parameters);
		int ypos = 52;
		vector<InteractableObject*> inventorycontent = inventory.getstorage();
		for (std::vector<InteractableObject*>::iterator it = inventorycontent.begin(); it != inventorycontent.end(); it++)
		{
			RenderTextOnScreen(baseMeshList[GEO_TEXT], (*it)->gettype(), Color(0, 0, 0), 2, 2, ypos, modelStack, viewStack, projectionStack, m_parameters);
			ypos -= 2;

		}
		std::ostringstream ss;
		ss << "FPS: " << fps;
		RenderTextOnScreen(baseMeshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 58, 68, modelStack, viewStack, projectionStack, m_parameters);
		RenderTextOnScreen(baseMeshList[GEO_TEXT], interacttext.str(), Color(0.5, 0.5, 0.5), 5, 40 - (interacttext.str().length()), 30, modelStack, viewStack, projectionStack, m_parameters);
		interacttext.str("");
	}
}

void Scene::RenderText(Mesh* mesh, std::string text, Color color, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
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
}

void Scene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
{
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
}

void Scene::RenderMeshOnScreen(Mesh* mesh, float x, float y, int sizex, int sizey, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
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
	RenderMesh(mesh, false, modelStack, viewStack, projectionStack, m_parameters); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Scene::RenderNPCDialogue(std::string NPCText, std::string headerText, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
{
	//float headerTextPos = 4.f;
	RenderMeshOnScreen(baseMeshList[GEO_HEADER], 14.75, 19.25, 30, 6, modelStack, viewStack, projectionStack, m_parameters);
	//headerText.size()
	RenderTextOnScreen(baseMeshList[GEO_TEXT], headerText, Color(0, 0, 0), 4, 14.5 - (headerText.size()), 17, modelStack, viewStack, projectionStack, m_parameters);	//header text
	RenderMeshOnScreen(baseMeshList[GEO_TEXTBOX], 40, 8.75, 80, 17.5, modelStack, viewStack, projectionStack, m_parameters);
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
		RenderTextOnScreen(baseMeshList[GEO_TEXT], word, Color(0, 0, 0), 3, xpos, ypos, modelStack, viewStack, projectionStack, m_parameters);
		if (wordpos > last)
			break;
		wordpos += word.length() + 1;
		xpos += 1.5 * word.length() + 1;
	}
}

void Scene::RenderMinigameIntro(std::string MinigamedescriptionText, std::string headerText, float fontsize, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
{
	RenderMeshOnScreen(baseMeshList[GEO_TEXTBOX], 40, 30, 80, 50, modelStack, viewStack, projectionStack, m_parameters);
	RenderMeshOnScreen(baseMeshList[GEO_HEADER], 40, 57, 60, 8, modelStack, viewStack, projectionStack, m_parameters);
	RenderTextOnScreen(baseMeshList[GEO_TEXT], headerText, Color(0, 0, 0), 6, 38 - (headerText.size() * 1.5), 54, modelStack, viewStack, projectionStack, m_parameters);	//header text
	string word;																	//automating text
	int wordpos = 0, ypos = 50 - fontsize, last = MinigamedescriptionText.find_last_of(" ");
	float xpos = 2.f;
	while (true)
	{
		word = MinigamedescriptionText.substr(wordpos, MinigamedescriptionText.find(" ", wordpos + 1) - wordpos);
		if (xpos + word.length() * (fontsize * 0.5) + (fontsize * 0.5) > 80)		//if new word will exceed screensize
		{
			ypos -= fontsize;
			xpos = 2;
		}
		RenderTextOnScreen(baseMeshList[GEO_TEXT], word, Color(0, 0, 0), fontsize, xpos, ypos, modelStack, viewStack, projectionStack, m_parameters);
		if (wordpos > last)
			break;
		wordpos += word.length() + 1;
		xpos += (fontsize * 0.5) * word.length() + (fontsize * 0.5);
	}
}

void Scene::movement(Camera3 &camera, vector<Terrain*> terrains, double dt)
{
	if (!indialogue)//Don't move while in a dialogue
	{
		camera.Updatepos(dt); //Updates to the position all happen before updates to the view
		for (std::vector<Terrain*>::iterator it = terrains.begin(); it != terrains.end(); it++)
			(*it)->solidCollisionBox(camera.position);
		camera.Updateview(dt); //Updates the view after the processing of all the collisions
	}
}

void Scene::interact(Camera3 camera, vector<InteractableObject*>& items)
{
	{
		int counter = 0;
		for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
		{
			if ((*it)->spherecollider(camera.target) && !indialogue) // Checks if the target is within a radius of an item and not in a dialogue
			{
				if (Application::IsKeyPressed('F'))// F is look at
				{
					dialogue = (*it)->lookat; //Set the dialogue vector to that of the current object
					currentline = dialogue.begin(); //Currentline is set at the look at description
					indialogue = true;//Set state to in dialogue
				}
				if (Application::IsKeyPressed('G'))// G is pick up
				{
					if ((*it)->getpickupable() == true)
					{
						inventory.additem((*it));
						items.erase(items.begin() + counter);
						break;
					}
					else //If cannot pick up item, a dialogue box show is that tells them that they can't do so
					{
						dialogue.push_back("1I can't do that.");
						currentline = dialogue.begin(); 
						name = "";
						indialogue = true;
					}
				}
				if (Application::IsKeyPressed('T')) //T is talk to
				{
					dialogue = (*it)->dialogue; //Set the dialogue vector to that of the current object
					currentline = dialogue.begin(); //Currentline iteratior as the first line of dialogue
					name = (*it)->getname(); //Set the name of the npc the player talks to
					indialogue = true;//Set state to in dialogue
				}
				interacttext << (*it)->getname();
				break;
			}
			counter++;
		}
	}
}


