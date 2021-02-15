#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, float lengthX, float lengthY, Color color, float wrapno);
	static Mesh* GenerateCube(const std::string &meshName, float lengthX, float lengthY, float lengthZ, Color color);
	static Mesh* GenerateHemisphere(const std::string& meshName, float radius, unsigned numStack, unsigned numSlice, Color color);
	static Mesh* GenerateTruncatedHemisphere(const std::string& meshName, float radius, unsigned numStack, unsigned numSlice, unsigned truncated, Color color);
	static Mesh* GenerateSphere(const std::string& meshName, float radius, unsigned numStack, unsigned numSlice, Color color);
	static Mesh* GenerateRing(const std::string& meshName, float radius);
	static Mesh* GenerateCylinder(const std::string& meshName, float radius, float heigth, unsigned numSlice,unsigned numStack, Color color);
	static Mesh* GenerateCone(const std::string& meshName, float radius, float heigth, unsigned numSlice, Color color);
	static Mesh* GenerateTorus(const std::string& meshName, unsigned numSlice, unsigned numStack, float outerR, float innerR, Color color);
	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol);
	static Mesh* GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path);
};

#endif