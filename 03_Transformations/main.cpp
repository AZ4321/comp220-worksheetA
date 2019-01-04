
#include <SDL.h>
#include <SDL_image.h>
#include <GL\glew.h>
#include <SDL_opengl.h>

#include <string>
#include <vector>
#include <fstream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertex.h"
#include "shader.h"
#include "Texture.h"
#include "Model.h"

using namespace glm;

int main(int argc, char ** argsv)
{
	//Starting the SDL Library, using SDL_INIT_VIDEO to only run the video parts
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//If SDL failed to initialize, this error will show
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init failed", SDL_GetError(), NULL);
		return 1;
	}
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
	{
		//If SDL failed to initialize, this error will show
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Image failed", SDL_GetError(), NULL);
		return 1;
	}



	//Creating the window, have to remember to quit the window at the end to return the pointer by destroying it(the best way)
	SDL_Window* window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 640, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		//Show error if SDL didnt create the window
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow failed", SDL_GetError(), NULL);

		SDL_Quit(); //quit SDL
		return 1;
	}

	//Requesting 3.3 Core OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext gl_Context = SDL_GL_CreateContext(window);
	if (gl_Context == nullptr) //if the glew context has a null value, show error and then destroy window
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Create_Context failed, check to make sure the context is assigned correctly!", SDL_GetError(), NULL);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return 1;

	}

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) //if Glew is not okay, show error and close window
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Glew start-up failed, check to make sure Glew is started properly!", (char*)glewGetErrorString(err), NULL);
		SDL_DestroyWindow(window);
		SDL_Quit();

		return 1;
	}


	//Setting up VertexArrayID
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	

	// This will identify our vertex buffer and element buffer
	GLuint vertexbuffer;
	GLuint elementBuffer;

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &elementBuffer);

	// The following commands will talk about our 'vertexbuffer' buffer/element
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	unsigned int numberOfVerts = 0;
	unsigned int numberOfIndices = 0;
	loadModelFromFile("Tank1.FBX", vertexbuffer, elementBuffer, numberOfVerts, numberOfIndices);

	//Loading the image
	//GLuint textureID = loadTextureFromFile("brick_D.png");

	//Triangle scale/position
	vec3 trianglePosition = vec3(0.0f, 0.0f, 0.0f);
	vec3 triangleScale = vec3(0.0f, 0.0f, 0.0f);
	vec3 triangleRotation = vec3(0.0f, 0.0f, 0.0f);

	//Correspodning matrices
	mat4 translationMatrix = translate(trianglePosition);
	mat4 scaleMatrix = scale(triangleScale);
	mat4 rotationMatrix = rotate(triangleRotation.x, vec3(1.0f, 0.0f, 0.0f))*rotate(triangleRotation.y, vec3(0.0f, 1.0f, 0.0f))*rotate(triangleRotation.z, vec3(0.0f, 0.0f, 1.0f));

	//Cumulating the above transformations into one 
	mat4 modelMatrix = translationMatrix*rotationMatrix*scaleMatrix;

	//Camera components
	vec3 cameraPosition = vec3(0.0f, 0.0f, -2.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	//Viewmatrix for  the camera
	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

	//Projection matrix
	mat4 projectionMatrix = perspective(radians(90.0f), float(800 / 600), 0.1f, 100.0f);


	GLint simpleProgramID = LoadShaders("textureVert.glsl", "textureFrag.glsl");
	if (simpleProgramID < 0)
	{
		printf("Textures have not loaded");
	}

	GLint modelMatrixLocation=glGetUniformLocation(simpleProgramID, "modelMatrix");
	GLint viewMatrixLocation = glGetUniformLocation(simpleProgramID, "viewMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(simpleProgramID, "projectionMatrix");
	GLint textureLocation = glGetUniformLocation(simpleProgramID, "baseTexture");


	//Running is always true as long as Escape is not pressed 
	bool running = true;

	//SDL Event structure initiation
	SDL_Event ev;
	while (running)
	{
		//Poll for the events
		while (SDL_PollEvent(&ev))
		{
			//Switch case for event type
			switch (ev.type)
			{
				//If the case is SDL_QUIT then running will be false
			case SDL_QUIT:
				running = false;
				break;
				//Checks the keydown inputs
			case SDL_KEYDOWN:
				//Checks which button has been pressed
				switch (ev.key.keysym.sym)
				{
					//In case of ESC being pressed, the program will close
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_f:
					SDL_SetWindowFullscreen(window,1);
				}
			}
		}

		//Rendering goes here, noice
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		//bind textures
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureID);

		glUseProgram(simpleProgramID);

		//Declare uniforms below
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
		glUniform1i(textureLocation, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		// 1st attribute buffer : vertices
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

		// Draw the triangle !
		glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, (void*)0); 
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(window);
		SDL_SetWindowResizable(window, SDL_TRUE);
	}
	glDeleteBuffers(1, &elementBuffer);
	glDeleteProgram(simpleProgramID);
	glDeleteBuffers(1, &vertexbuffer);
	//glDeleteTextures(1, &textureID);
	glDeleteVertexArrays(1, &VertexArrayID);
	//Deleting the context
	SDL_GL_DeleteContext(gl_Context);
	//Clean up, deactivating the library and the window
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;

}

