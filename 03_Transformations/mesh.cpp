#include "mesh.h"



mesh::mesh()
{
	m_VBO = 0;
	m_EBO = 0;
	m_VAO = 0;
	m_NumberOfIndices = 0;
	m_NumberOfVerts = 0;

}


mesh::~mesh()
{

	destroy();

}

void mesh::copyMeshData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	
	//binding array buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	//binding element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));

	m_NumberOfVerts = vertices.size();
	m_NumberOfIndices = indices.size();

}

void mesh::init()
{
	//Vertex Array
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	//Vertex Buffer
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//Element buffer
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_EBO);
}

void mesh::destroy()
{
	//deleting arrays/buffers
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);

}

void mesh::render()
{
	glDrawElements(GL_TRIANGLES, m_NumberOfIndices, GL_UNSIGNED_INT, (void*)0);
}
