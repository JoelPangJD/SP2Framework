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
	int x;
	int y;
public:
	vector<string> lookat;
	vector<string> dialogue;
	InteractableObject();
	InteractableObject(Vector3 pos, float angle, float scale, float radius, string type);
	InteractableObject(Vector3 pos, float angle, float scale, float height, float xwidth, float zwidth, string type);
	~InteractableObject();
	int interact();
};