#include "Inventory.h"
#include "SceneMain.h"
#include "GL\glew.h"
#include "Application.h"

#include "LoadTGA.h"
#include "Utility.h"
#include "shader.hpp"
#include <Mtx44.h>
#include"MeshBuilder.h"

Inventory::Inventory()
{
	y = 0;
	x = 0;
	key = 0;
	clue1 = 0;
}


int Inventory::get_key()
{
	return key;
}

int Inventory::get_clue()
{
	return clue1;
}

void Inventory::update_inventory()
{
}
