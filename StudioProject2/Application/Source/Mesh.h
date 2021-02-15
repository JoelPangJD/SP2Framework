#ifndef MESH_H
#define MESH_H
#include <fstream>
#include <string>
#include "Material.h"
#include <vector>
#include <utility> // std::pair
#include <sstream> // std::stringstream

/******************************************************************************/
/*!
		Class Mesh:
\brief	To store VBO (vertex & color buffer) and IBO (index buffer)
*/
/******************************************************************************/
class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	~Mesh();
	void Render();
	void Render(unsigned offset, unsigned count);
	static void SetMaterialLoc(unsigned kA, unsigned kD, unsigned kS, unsigned nS);
	std::vector<Material> materials;
	static unsigned locationKa;
	static unsigned locationKd;
	static unsigned locationKs;
	static unsigned locationNs;

	Material material;
	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	//unsigned colorBuffer;
	unsigned indexBuffer;
	unsigned indexSize;
	unsigned textureID;
	
};

#endif