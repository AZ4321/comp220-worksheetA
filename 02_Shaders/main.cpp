#include <iostream>
#include <SDL.h>
#include "main.h"

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	
	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

int main(int argc, char ** argsv)
{
	//Starting the SDL Library, using SDL_INIT_VIDEO to only run the video parts
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//If SDL failed to initialize, this error will show
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init failed", SDL_GetError(), NULL);
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

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
	   -1.0f, -1.0f, 0.0f,
	   1.0f, -1.0f, 0.0f,
	   0.0f,  1.0f, 0.0f,
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLint simpleProgramID = LoadShaders("vertexshader.glsl", "fragmentshader.glsl");
	if (simpleProgramID < 0)
	{
		printf("Shaders has not loaded");
	}

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
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);


		glUseProgram(simpleProgramID);
		SDL_GL_SwapWindow(window);
		SDL_SetWindowResizable(window, SDL_TRUE);
	}

	glDeleteProgram(simpleProgramID);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	//Deleting the context
	SDL_GL_DeleteContext(gl_Context);
	//Clean up, deactivating the library and the window
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;

}

