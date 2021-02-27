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
	baseMeshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("inventory", Color(1, 1, 1), 1.0f);
	baseMeshList[GEO_INVENTORY]->textureID = LoadTGA("Image//inventory.tga");
	baseMeshList[GEO_ACTIONS] = MeshBuilder::GenerateQuad("actions", Color(1, 1, 1), 1.0f);
	baseMeshList[GEO_ACTIONS]->textureID = LoadTGA("Image//actions.tga");
	baseMeshList[GEO_PRESSE] = MeshBuilder::GenerateQuad("pressE", Color(1, 1, 1), 1.0f);
	baseMeshList[GEO_PRESSE]->textureID = LoadTGA("Image//pressE.tga");

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
			currentname = "Steve";
		else if (dialoguetext[0] == '2')
			currentname = name;
		dialoguetext = dialoguetext.substr(1);
		RenderNPCDialogue(dialoguetext, currentname, modelStack, viewStack, projectionStack, m_parameters);
		if (cooldown <= 0 && Application::IsKeyPressed('E') || FoundAnswer) //Cooldown added to prevent spamming to pass the dialogues too fast
		{
			Preview = false;
			ShowAnswer = false;
			cooldown = 0.4f;
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
		RenderMeshOnScreen(baseMeshList[GEO_INVENTORY], 10, 37, 43, 45, modelStack, viewStack, projectionStack, m_parameters);
		int ypos = 52;
		vector<InteractableObject*> inventorycontent = inventory->getstorage();
		for (std::vector<InteractableObject*>::iterator it = inventorycontent.begin(); it != inventorycontent.end(); it++)
		{
			if ((*it) == inventory->getcurrentitem())
			{
				RenderMeshOnScreen(baseMeshList[GEO_TEXTBOX], 8, ypos + 1, 14, 2, modelStack, viewStack, projectionStack, m_parameters);
			}
			RenderTextOnScreen(baseMeshList[GEO_TEXT], (*it)->getname(), Color(0, 0, 0), 2, 2, ypos, modelStack, viewStack, projectionStack, m_parameters);
			ypos -= 2;
		}
		if (cooldown <= 0) //Cooldown added to prevent spamming to though inventory too fast
		{
			if (Application::IsKeyPressed(VK_UP))
			{
				inventory->navigateinventory(1);
				cooldown = 0.2;
			}
			else if (Application::IsKeyPressed(VK_DOWN))
			{
				inventory->navigateinventory(2);
				cooldown = 0.2;
			}
		}
		std::ostringstream ss;
		ss << "FPS: " << fps;
		RenderTextOnScreen(baseMeshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 58, 68, modelStack, viewStack, projectionStack, m_parameters);
		if (interacttext.str() != "")
		{
			RenderMeshOnScreen(baseMeshList[GEO_ACTIONS], 12, 10, 30, 30, modelStack, viewStack, projectionStack, m_parameters);
		}
		RenderTextOnScreen(baseMeshList[GEO_TEXT], interacttext.str(), Color(0.5, 0.5, 0.5), 5, 40 - (interacttext.str().length()), 30, modelStack, viewStack, projectionStack, m_parameters);
		interacttext.str("");
	}
}

void Scene::RenderText(Mesh* mesh, std::string text, Color color, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	float zspace = 0.f;

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
		characterSpacing.SetToTranslation(i * 0.5f, 0, zspace); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		zspace += 0.01;
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

void Scene::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
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
	RenderMeshOnScreen(baseMeshList[GEO_PRESSE], 76, 3, 7, 7, modelStack, viewStack, projectionStack, m_parameters);
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

void Scene::RenderMinigameIntro(std::string MinigamedescriptionText, std::string MinigamenameText, float fontsize, MS modelStack, MS viewStack, MS projectionStack, unsigned m_parameters[])
{
	RenderMeshOnScreen(baseMeshList[GEO_TEXTBOX], 40, 30, 80, 50, modelStack, viewStack, projectionStack, m_parameters);
	RenderMeshOnScreen(baseMeshList[GEO_HEADER], 40, 57, 60, 8, modelStack, viewStack, projectionStack, m_parameters);
	RenderTextOnScreen(baseMeshList[GEO_TEXT], MinigamenameText, Color(0, 0, 0), 6, 38 - (MinigamenameText.size() * 1.5), 54, modelStack, viewStack, projectionStack, m_parameters);	//header text
	RenderMeshOnScreen(baseMeshList[GEO_PRESSE], 76, 8, 7, 7, modelStack, viewStack, projectionStack, m_parameters);
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

string Scene::interact(Camera3 &camera, vector<InteractableObject*>& items, bool MarinaBay)
{
	if (Application::IsKeyPressed('Q') && !(inventory->getstorage().empty())) //Use an item in inventory if inventory not empty
	{
		if (inventory->getcurrentitem()->gettype() == "yarn")//Using yarn activates garden minigame 2
			return "Gardenminigame2";
		else if((inventory->getcurrentitem()->gettype() == "stick" && inventory->checkinventory("fishing line")) 
			|| (inventory->getcurrentitem()->gettype() == "fishing line" && inventory->checkinventory("stick")))
		{
			inventory->removeitem("stick");
			inventory->removeitem("fishing line");
			inventory->additem(new InteractableObject(Vector3(0,0,0),0,1,0,"fishing rod", "Fishing rod", true));
			dialogue.push_back("1The stick and fishing line were combined into a fishing rod.");
			currentline = dialogue.begin();
			name = "";
			indialogue = true;
		}

	}
	for (std::vector<InteractableObject*>::iterator it = items.begin(); it != items.end(); it++)
	{
		if (MarinaBay)
		{
			if ((*it)->gettype() == "badguy3" && currentline + 1 == dialogue.end())	
				//just returns if its at the end of the dialogue but I still can't think of a better way to do this
				return "battleStart";
		}
		if ((*it)->spherecollider(camera.target) && !indialogue) // Checks if the target is within a radius of an item and not in a dialogue
		{
		    if (Application::IsKeyPressed('Q')) //Q is use
			{
				///For scenemusem///
				if ((*it)->gettype() == "exit")
				{
					ToExit = true;
				}
				///////////////

				if ((*it)->gettype() == "gardentocity")
				{
					camera.position = Vector3(-85, 5, 0);
					Application::SwitchScene = 0;
				}

				else if ((*it)->gettype() == "citytomuseum")
				{
					//The position here is in the scene before swtiching such that the player will not overlap when they return to the same scene
					camera.position = Vector3(0, 3, -10); //Change the camera position to somewhere that doesn't overlap to prevent constantly moving back and forth
					return "frontofmuseum"; //For the other scene you can follow the garden to city example as there is no minigame to trigger before hand
				}

				if (!(inventory->getstorage().empty())) //For uses that rely on inventory, make sure the inventory is 
				{
					if ((*it)->gettype() == "cat" && inventory->getcurrentitem()->gettype() == "fish")//using fish on cat
					{
						CantUse = false;
						inventory->removeitem(inventory->getcurrentitem());
						inventory->additem(new InteractableObject(Vector3(0, 0, 0), 0, 1, 0, "Marina Bay ticket", "MBS ticket", true));
						dialogue.push_back("1Oh it gave me a ticket to the Marina Bay Sands.");
						dialogue.push_back("1I should be access that place now to find who stole my wallet");
						currentline = dialogue.begin();
						name = "";
						indialogue = true;

					}
					if ((*it)->gettype() == "pond" && inventory->getcurrentitem()->gettype() == "fishing rod")//using fishing rod on the pond
					{
						CantUse = false;
						return "Gardenminigame1";
					}
					if ((*it)->gettype() == "place key" && inventory->getcurrentitem()->gettype() == "key")
					{
						CantUse = false;
						place1 = true;
						inventory->removeitem("key");
					}

					else if ((*it)->gettype() == "place flag" && inventory->getcurrentitem()->gettype() == "flag")
					{
						CantUse = false;
						place2 = true;
						inventory->removeitem("flag");
					}

					else if ((*it)->gettype() == "place box" && inventory->getcurrentitem()->gettype() == "box")
					{
						CantUse = false;
						place3 = true;
						inventory->removeitem("box");
					}
					else if (CantUse == true)
					{
						dialogue.push_back("1I'm not supposed to use it here.");
						currentline = dialogue.begin();
						name = "";
						indialogue = true;
					}
				}
			}
			if (Application::IsKeyPressed('F'))// F is look at
			{
				dialogue = (*it)->lookat; //Set the dialogue vector to that of the current object
				currentline = dialogue.begin(); //Currentline is set at the look at description
				indialogue = true;//Set state to in dialogue
			}
			else if (Application::IsKeyPressed('G'))// G is pick up
			{
				if ((*it)->getpickupable() == true)
				{
					inventory->additem((*it));
					items.erase(items.begin() + distance(items.begin(),it));
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
			else if (Application::IsKeyPressed('T')) //T is talk to
			{
				dialogue = (*it)->dialogue; //Set the dialogue vector to that of the current object
				currentline = dialogue.begin(); //Currentline iteratior as the first line of dialogue
				name = (*it)->getname(); //Set the name of the npc the player talks to
				indialogue = true;//Set state to in dialogue

				//////////////////////////////////////////FOR SCENE MUSEUM///////////////////////////////////////////////////////
				if (EndGame1 == false)
				{
					if ((*it)->gettype() == "preview")
					{
						Preview = true;
					}
					if ((*it)->gettype() == "answer")
					{
						ShowAnswer = true;
					}

				}
				if (place1 == true && place2 == true && place3 == true)
				{
					if ((*it)->gettype() == "before gathering item")
					{
						items.erase(it);
						inventory->additem(new InteractableObject(Vector3(0, 0, 0), 0, 1, 0, "changi pass", "Changi pass", true));
					}
				}
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				if (MarinaBay == true)
				{
					//shortening dialogue to not show the full length when talked to
					if ((*it)->gettype() == "girl")
					{
						(*it)->updatedialogue("girl2");
						(*it)->updatedescription("girl2");
					}
					else if ((*it)->gettype() == "robot")
						(*it)->updatedialogue("robot2");
					else if ((*it)->gettype() == "orc2")
						(*it)->updatedialogue("orc3");
					else if ((*it)->gettype() == "badguy2")
						(*it)->updatedialogue("badguy3");
					else if ((*it)->gettype() == "pool2")
					{
						(*it)->updatedialogue("pool");
						(*it)->updatedescription("pool");
					}
					//triggers start of riddle
					else if ((*it)->gettype() == "adventurer")	
					{
						(*it)->updatedialogue("adventurer2");
						riddleStarted = true;
					}
				}
			}
			else if(interacttext.str() == "") //if the text for highlighted object is empty 
				interacttext << (*it)->getname();
			if (MarinaBay == true)
			{
				if ((*it)->gettype() == "pool" && riddleStarted)
				{
					(*it)->updatedialogue("pool2");
					(*it)->updatedescription("pool2");
					riddleSolved = true;
				}
				else if ((*it)->gettype() == "adventurer2" && riddleSolved)
				{
					(*it)->updatedialogue("adventurer3");
					inventory->additem(new InteractableObject(Vector3(0, 0, 0), 0, 0, 0, "Sword", "Sword", true));
					inventory->additem(new InteractableObject(Vector3(0, 0, 0), 0, 0, 0, "Orb", "Orb", true));
				}
				else if ((*it)->gettype() == "orc" && riddleSolved)
					(*it)->updatedialogue("orc2");
			}
			break;
		}
	}
	return ""; //if no special actions occur, return an empty string
}


