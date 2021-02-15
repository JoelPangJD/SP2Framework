#include "MeshBuilder.h"
#include <GL\glew.h>
#include <Vector>
#include "Vertex.h"
#include "LoadOBJ.h"
/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/

Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;

	//x
	v.pos.Set(1000.0f, 0.0f, 0.0f); v.color.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-1000.0f, 0.0f, 0.0f); v.color.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	//y
	v.pos.Set(0.0f, 1000.0f, 0.0f); v.color.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.0f, -1000.0f, 0.0f); v.color.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	//z
	v.pos.Set(0.0f, 0.0f, 1000.0f); v.color.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.0f, 0.0f, -1000.0f); v.color.Set(0, 0, 1); vertex_buffer_data.push_back(v);

	for (int i = 0; i < 6; i++) {
		index_buffer_data.push_back(i);
	}
	Mesh *mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;
	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, float lengthX, float lengthY, Color color, float wrapno)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	v.pos.Set(0.5f, 0.5f, 0.f); v.color = color; v.normal.Set(0, 0, 1); v.texCoord.Set(wrapno, wrapno);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.f); v.color = color; v.normal.Set(0, 0, 1); v.texCoord.Set(0, wrapno);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.f); v.color = color; v.normal.Set(0, 0, 1); v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.f); v.color = color; v.normal.Set(0, 0, 1); v.texCoord.Set(wrapno, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.f); v.color = color; v.normal.Set(0, 0, 1); v.texCoord.Set(wrapno, wrapno);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.f); v.color = color; v.normal.Set(0, 0, 1); v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	for (int i = 0; i < 6; i++)
		index_buffer_data.push_back(i);
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, float lengthX, float lengthY, float lengthZ, Color color)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	std::vector<Vertex> normal;
	//front
	v.pos.Set(lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f,0.f,1.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, 1.f); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, 1.f); v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, 1.f); v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, 1.f); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, 1.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	//right
	v.pos.Set(lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(1.f, 0.f, 0.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(1.f, 0.f, 0.f); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(1.f, 0.f, 0.f); v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(1.f, 0.f, 0.f); v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(1.f, 0.f, 0.f); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(1.f, 0.f, 0.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	//down
	v.pos.Set(lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, -1.f, 0.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, -1.f, 0.f); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, -1.f, 0.f); v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, -1.f, 0.f); v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, -1.f, 0.f); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, -1.f, 0.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	//left
	v.pos.Set(-lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(-1.f, 0.f, 0.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(-1.f, 0.f, 0.f); v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(-1.f, 0.f, 0.f); v.texCoord.Set(1, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(-1.f, 0.f, 0.f); v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(-1.f, 0.f, 0.f); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(-1.f, 0.f, 0.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	//back
	v.pos.Set(-lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, -1.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, -1.f); v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, -1.f); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, -1.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, -1.f); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, -lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, 0.f, -1.f); v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	//top
	v.pos.Set(lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, 1.f, 0.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, 1.f, 0.f); v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, 1.f, 0.f); v.texCoord.Set(1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, 1.f, 0.f); v.texCoord.Set(0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-lengthX / 2, lengthY / 2, lengthZ / 2); v.color = color; v.normal.Set(0.f, 1.f, 0.f); v.texCoord.Set(0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX / 2, lengthY / 2, -lengthZ / 2); v.color = color; v.normal.Set(0.f, 1.f, 0.f); v.texCoord.Set(1, 1); vertex_buffer_data.push_back(v);

	for (int i = 0; i < 36; i++) {
		index_buffer_data.push_back(i);
	}
	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, mesh->colorBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = 36;
	return mesh;
}

float SphereX(float phi, float theta)
{
	return (cos(phi * 3.141592 / 180) * cos(theta * 3.141592 / 180));
}
float SphereY(float phi, float theta)
{
	return (sin(phi * 3.141592 / 180));
}
float SphereZ(float phi, float theta)
{
	return (cos(phi * 3.141592 / 180) * sin(theta * 3.141592 / 180));
}

Mesh* MeshBuilder::GenerateSphere(const std::string& meshName, float radius, unsigned numStack, unsigned numSlice, Color color)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float degreePerStack = 180.f/ numStack;
	float degreePerSlice = 360.f/numSlice;

	float textureStack = 0;
	float texturePerStack = 1 / numStack;
	float texturePerSlice = 1 / numSlice;
	int count = 0;
	for (float phi = -90; phi < 90; phi += degreePerStack)
	{
		float textureSlice = 0;
		for (float theta = 0; theta <= 360; theta += degreePerSlice)
		{
			v.pos.Set(radius * SphereX(phi, theta), radius * SphereY(phi, theta), radius * SphereZ(phi, theta));
			v.color = color;
			v.normal.Set(SphereX(phi, theta), SphereY(phi, theta), SphereZ(phi, theta));
			v.texCoord.Set(1-theta/360, (phi+90)/180);
			vertex_buffer_data.push_back(v);

			v.pos.Set(radius * SphereX(phi + degreePerStack, theta), radius * SphereY(phi + degreePerStack, theta), radius * SphereZ(phi + degreePerStack, theta));
			v.color = color;
			v.normal.Set(SphereX(phi + degreePerStack, theta), SphereY(phi + degreePerStack, theta), SphereZ(phi + degreePerStack, theta));
			v.texCoord.Set(1-theta / 360, (phi + degreePerStack + 90) / 180);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(count++);
			index_buffer_data.push_back(count++);
		}
	}
	//for (unsigned stack = 0; stack < numStack; ++stack)
	//{
	//	for (unsigned slice = 0; slice < numSlice + 1; ++slice)
	//	{
	//		index_buffer_data.push_back((numSlice + 1)* stack + slice + 0);
	//		index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
	//	}
	//}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateHemisphere(const std::string& meshName, float radius, unsigned numStack, unsigned numSlice, Color color)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float degreePerStack = 90.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	int count = 0;

	for (float i = 0.f; i <= 360.f; i += degreePerSlice) //Bottom circle
	{
		float theta = i * 3.141592 / 180;
		v.pos.Set(0, 0, 0);
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(radius * cos(theta), 0, radius * sin(theta));
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(count++);
		index_buffer_data.push_back(count++);
	}

	for (float phi = 0; phi < 90; phi += degreePerStack)
	{
		for (float theta = 0; theta <= 360; theta += degreePerSlice)
		{
			v.pos.Set(radius * SphereX(phi, theta), radius * SphereY(phi, theta), radius * SphereZ(phi, theta));
			v.color = color;
			v.normal.Set(SphereX(phi, theta), SphereY(phi, theta), SphereZ(phi, theta));
			vertex_buffer_data.push_back(v);

			v.pos.Set(radius * SphereX(phi + degreePerStack, theta), radius * SphereY(phi + degreePerStack, theta), radius * SphereZ(phi + degreePerStack, theta));
			v.color = color;
			v.normal.Set(SphereX(phi + degreePerStack, theta), SphereY(phi + degreePerStack, theta), SphereZ(phi + degreePerStack, theta));
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(count++);
			index_buffer_data.push_back(count++);
		}
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}


Mesh* MeshBuilder::GenerateTruncatedHemisphere(const std::string& meshName, float radius, unsigned numStack, unsigned numSlice, unsigned truncated, Color color)
{

	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float degreePerStack = 90.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	int count = 0;

	for (float i = 0.f; i <= 360.f; i += degreePerSlice) //bottom circle
	{
		float theta = i * 3.141592 / 180;
		v.pos.Set(0, 0, 0);
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(radius * cos(theta), 0, radius * sin(theta));
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(count++);
		index_buffer_data.push_back(count++);
	}

	for (float phi = 0; phi < 90 - (degreePerStack * truncated); phi += degreePerStack)
	{
		for (float theta = 0; theta <= 360; theta += degreePerSlice)
		{
			v.pos.Set(radius * SphereX(phi, theta), radius * SphereY(phi, theta), radius * SphereZ(phi, theta));
			v.color = color;
			v.normal.Set(SphereX(phi, theta), SphereY(phi, theta), SphereZ(phi, theta));
			vertex_buffer_data.push_back(v);

			if ((phi + degreePerStack) >= (90 - (degreePerStack * truncated))) // top circle
			{
				v.pos.Set(0, radius * SphereY(phi, theta), 0);
				v.color = color;
				v.normal.Set(0, 1, 0);
				vertex_buffer_data.push_back(v);
			}
			else
			{
				v.pos.Set(radius * SphereX(phi + degreePerStack, theta), radius * SphereY(phi + degreePerStack, theta), radius * SphereZ(phi + degreePerStack, theta));
				v.color = color;
				v.normal.Set(SphereX(phi + degreePerStack, theta), SphereY(phi + degreePerStack, theta), SphereZ(phi + degreePerStack, theta));
				vertex_buffer_data.push_back(v);
			}
			index_buffer_data.push_back(count++);
			index_buffer_data.push_back(count++);
		}
	}


	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string& meshName, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	int count = 0;
	for (float i = 0; i <= 360; i += 10.f)
	{
		v.pos.Set(radius * cos(i * 3.141592 / 180), radius * sin(i * 3.141592 / 180), 0); v.color.Set(0, 1, 1); vertex_buffer_data.push_back(v);
		v.pos.Set((radius - 0.5) * cos(i  * 3.141592 / 180), (radius - 0.5) * sin(i * 3.141592 / 180), 0); v.color.Set(1, 1, 0); vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(count++);
		index_buffer_data.push_back(count++);
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

//Additional Cylinder code
float cylinderX(float degrees)
{
	return cos(degrees * 3.141592 / 180);
}
float cylinderZ(float degrees)
{
	return sin(degrees * 3.141592 / 180);
}

Mesh* MeshBuilder::GenerateCylinder(const std::string& meshName, float radius, float height, unsigned numSlice, unsigned numStack, Color color)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	int count = 0;
	float stackHeight = height / numStack;
	float degreeperslice = 360.f / numSlice;
	for (float theta = 0; theta <= 360; theta += degreeperslice) // bottom circle
	{
		v.pos.Set(0, 0, 0);
		v.color = color;
		v.normal.Set(0, -1, 0);
		v.texCoord.Set(0.5, 0.5);
		vertex_buffer_data.push_back(v);
		v.pos.Set(radius * cylinderX(theta), 0, radius * cylinderZ(theta));
		v.color = color;
		v.normal.Set(0, -1, 0);
		v.texCoord.Set((cylinderX(theta) / radius) / 2 + 0.5, -(cylinderZ(theta) / radius) / 2 + 0.5);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(count++);
		index_buffer_data.push_back(count++);
	}
	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (float theta = 0; theta <= 360; theta += degreeperslice)
		{
			v.pos.Set(radius * cylinderX(theta), stack * stackHeight, radius * cylinderZ(theta) );
			v.color = color;
			v.normal.Set(radius * cylinderX(theta), 0, radius * cylinderZ(theta));
			v.texCoord.Set((cylinderX(theta) / radius) / 2 + 0.5, -(cylinderZ(theta) / radius) / 2 + 0.5);
			vertex_buffer_data.push_back(v);
			v.pos.Set(radius * cylinderX(theta), (stack+1) * stackHeight, radius * cylinderZ(theta));
			v.color = color;
			v.normal.Set(radius * cylinderX(theta), 0, radius * cylinderZ(theta));
			v.texCoord.Set((cylinderX(theta) / radius) / 2 + 0.5, -(cylinderZ(theta) / radius) / 2 + 0.5);
			vertex_buffer_data.push_back(v);
			index_buffer_data.push_back(count++);
			index_buffer_data.push_back(count++);
		}
	}

	for (float theta = 0; theta <= 360; theta += degreeperslice) // top circle
	{
		v.pos.Set(radius * cylinderX(theta), height, radius * cylinderZ(theta));
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.texCoord.Set((cylinderX(theta) / radius)/2 + 0.5, -(cylinderZ(theta) / radius)/2 + 0.5 );
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, height, 0);
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.texCoord.Set(0.5, 0.5);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(count++);
		index_buffer_data.push_back(count++);
	}


	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string& meshName, float radius, float height, unsigned numSlice, Color color)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	int count = 0;
	float degreeperslice = 360.f / numSlice;

	for (float theta = 0; theta <= 360; theta += degreeperslice) //Bottom circle
	{
		v.pos.Set(0, 0, 0);
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(radius * cylinderX(theta), 0, radius * cylinderZ(theta));
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
		index_buffer_data.push_back(count++);
		index_buffer_data.push_back(count++);
	}

	for (float theta = 0; theta <= 360; theta += degreeperslice)
	{
		v.pos.Set(radius * cylinderX(theta), 0, radius * cylinderZ(theta));
		v.color = color;
		v.normal.Set(height * cylinderX(theta), radius, height * cylinderZ(theta));
		v.normal.Normalize();
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, height, 0);
		v.color = color;
		v.normal.Set(height * cylinderX(theta), radius, height * cylinderZ(theta));
		v.normal.Normalize();
		vertex_buffer_data.push_back(v);

		index_buffer_data.push_back(count++);
		index_buffer_data.push_back(count++);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateTorus(const std::string& meshName, unsigned numSlice, unsigned numStack, float outerR, float innerR, Color color)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	float degreePerStack = 360.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	float x1, z1; 
	float x2, y2, z2;
	for (unsigned stack = 0; stack < numStack + 1; stack++)
	{
		for (unsigned slice = 0; slice < numSlice + 1; slice++)
		{
			z1 = outerR * cos(stack * (degreePerStack * 3.141592 / 180));
			x1 = outerR * sin(stack * (degreePerStack * 3.141592 / 180));
			z2 = (outerR + innerR * cos(slice * (degreePerSlice * 3.141592 / 180))) * cos(stack * (degreePerStack * 3.141592 / 180));
			y2 = innerR * sin(slice * (degreePerSlice * 3.141592 / 180));
			x2 = (outerR + innerR * cos(slice * (degreePerSlice * 3.141592 / 180))) * sin(stack * (degreePerStack * 3.141592 / 180));
			v.pos.Set(x2, y2, z2);
			v.color = color;
			v.normal.Set(x2 - x1, y2, z2 - z1);
			v.normal.Normalize();
			vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; stack++) {
		for (unsigned slice = 0; slice < numSlice + 1; slice++) {
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string& meshName, const std::string& file_path) {
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	//Create the mesh and call glBindBuffer, glBufferData
	//Use triangle list and remember to set index size
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol)
{
	float width = 1.f / numCol;
	float height = 1.f / numRow;
	unsigned offset = 0;
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	for (unsigned row = numRow - 1; row > 0; --row)
	{
		for (unsigned col = 0; col < numCol; ++col)
		{
			v.pos.Set(0.5f, -0.5f, 0.f);  v.normal.Set(0, 0, 1); v.texCoord.Set(width * (col+1), height * row);
			vertex_buffer_data.push_back(v);
			v.pos.Set(0.5f, 0.5f, 0.f); v.normal.Set(0, 0, 1); v.texCoord.Set(width * (col+1), height * (row+1));
			vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, 0.5f, 0.f);  v.normal.Set(0, 0, 1); v.texCoord.Set(width * col, height * (row+1));
			vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, -0.5f, 0.f);  v.normal.Set(0, 0, 1); v.texCoord.Set(width * col, height * row);
			vertex_buffer_data.push_back(v);
			

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			index_buffer_data.push_back(offset + 0);

			offset += 4;
		}
	}
	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path)
{
	{
		//Read vertices, texcoords & normals from OBJ
		std::vector<Position> vertices;
		std::vector<TexCoord> uvs;
		std::vector<Vector3> normals;
		std::vector<Material> materials;
		bool success = LoadOBJMTL(file_path.c_str(), mtl_path.c_str(), vertices, uvs, normals, materials);
		if (!success)
			return NULL;
		//Index the vertices, texcoords & normals properly
		std::vector<Vertex> vertex_buffer_data;
		std::vector<GLuint> index_buffer_data;
		IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
		Mesh* mesh = new Mesh(meshName);
		for (Material& material : materials)
			mesh->materials.push_back(material);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
		mesh->indexSize = index_buffer_data.size();
		mesh->mode = Mesh::DRAW_TRIANGLES;
		return mesh;
	}
}
