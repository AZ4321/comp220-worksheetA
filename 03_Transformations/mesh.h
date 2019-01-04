#pragma once
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <vector>
#include <G:\comp220-worksheetC\comp220-worksheetC-2018-19\03_Transformations\Vertex.h>

class mesh
{
public:
	mesh();
	~mesh();

	void copyMeshData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	void init();
	void destroy();

	void render();

private:
	GLuint m_VBO;
	GLuint m_EBO;

	unsigned int m_NumberOfVerts;
	unsigned int m_NumberOfIndices;

	GLuint m_VAO;

};

