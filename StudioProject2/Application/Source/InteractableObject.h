#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Entity.h"
using namespace std;

class InteractableObject : public Entity
{

private:
	void readfromfile(string filename, vector<string>& type);//private function for reading information from text files
	bool pickupable;
public:
	vector<string> lookat; //store descriptions
	vector<string> dialogue; //store dialogues
	
	bool getpickupable();
	void setpickupable(bool pickable);
	InteractableObject();
	InteractableObject(Vector3 pos, float angle, float scale, float radius, string type, string name, bool pickupable); //Create an interactable object with an interact radius around it
	~InteractableObject();
	void updatedialogue(string type);// to update the dialogue by changing the type
	void updatedescription(string type);
	int interact();
};