#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string& meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string& meshName, Color color, float size = 1.0f);
	static Mesh* GenerateQuad(const std::string& meshName, float lengthX, float lengthY, Color color, float wrapno); //for quad meshes with repeating textures
	static Mesh* Generate2dTriangle(const std::string& meshName, Color color, float lengthX, float lengthY, bool backwards = false);
	static Mesh* GenerateTriangle(const std::string& meshName, Color color, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateCube(const std::string& meshName, Color color, float length);
	static Mesh* GenerateCircle(const std::string& meshName, Color color, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateSphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateHemisphere(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f);
	static Mesh* GenerateTorus(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float outerR = 1.f, float innerR = 0.2f);
	static Mesh* GenerateQuarterTorus(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float outerR = 1.f, float innerR = 0.2f);
	static Mesh* GenerateCylinder(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f, float height = 1.f);
	static Mesh* GenerateBody(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f, float height = 1.f);
	static Mesh* GenerateCone(const std::string& meshName, Color color, unsigned numStack, unsigned numSlice, float radius = 1.f, float height = 1.f);
	static Mesh* GenerateCone(const std::string& meshName, float radius, float heigth, unsigned numSlice, Color color);
	static Mesh* GenerateFrustum(const std::string& meshName, Color color, unsigned numStack, float radius, float radiusUpper, float radiusLower, float height);
	static Mesh* GenerateCrescent(const std::string& meshName, Color color, float length);
	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol);
	static Mesh* GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path);
};

#endif