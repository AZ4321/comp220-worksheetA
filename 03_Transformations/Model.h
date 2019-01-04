#pragma once

//Asset import includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include <G:\comp220-worksheetC\comp220-worksheetC-2018-19\03_Transformations\Vertex.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "mesh.h"

bool loadModelFromFile(const std::string filename, GLuint VBO, GLuint EBO, unsigned int& numberofVerts, unsigned int& numberIndices);

bool loadMeshesFroMFile(const std::string& filename, std::vector<mesh*>& meshes);



