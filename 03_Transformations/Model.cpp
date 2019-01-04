#include "Model.h"

bool loadModelFromFile(const std::string filename, GLuint VBO, GLuint EBO, unsigned int& numberofVerts, unsigned int& numberIndices)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);
	
	if (scene == nullptr)
	{
		printf("Error loading model", importer.GetErrorString());
		return false;
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		const aiMesh* currentAiMesh = scene->mMeshes[m];
		for (unsigned int v = 0; currentAiMesh->mNumVertices; v++)
		{
			const aiVector3D currentAiPosition = currentAiMesh->mVertices[v];

			//Vertex positions 
			Vertex ourVertex;
			ourVertex.x = currentAiPosition.x;
			ourVertex.y = currentAiPosition.y;
			ourVertex.z = currentAiPosition.z;

			//Color
			ourVertex.r = 0.0f;
			ourVertex.g = 0.0f;  
			ourVertex.b = 0.0f; 
			ourVertex.a = 0.0f;

			//Textures
			ourVertex.tu = 0.0f; ourVertex.tv = 0.0f;

			if (currentAiMesh->HasTextureCoords(0))
			{
				const aiVector3D currentTextureCoords = currentAiMesh->mTextureCoords[0][v];
				ourVertex.tu = currentTextureCoords.x;
				ourVertex.tv = currentTextureCoords.y;

			}
			if (currentAiMesh->HasVertexColors(0))
			{
				const aiColor4D currentColour = currentAiMesh->mColors[0][v];
				ourVertex.r = currentColour.r;
				ourVertex.g = currentColour.g;
				ourVertex.b = currentColour.b;
				ourVertex.a = currentColour.a;
			}
			
			vertices.push_back(ourVertex);

		}

		for (unsigned int f = 0; f < currentAiMesh->mNumFaces; f++)
		{
			const aiFace currentFace = currentAiMesh->mFaces[f];
			indices.push_back(currentFace.mIndices[0]);
			indices.push_back(currentFace.mIndices[1]);
			indices.push_back(currentFace.mIndices[2]);
		}

	}
	
	numberofVerts = vertices.size();
	numberIndices = indices.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numberofVerts * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);






	return true;

}

bool loadMeshesFroMFile(const std::string & filename, std::vector<mesh*>& meshes)
{
	return false;
}
