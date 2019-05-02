#include "Renderer.h"

uint VBO;
uint VAO;
uint shaderProgram;

void
REN_Init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

//
//TODO CLEANUP
//Read Khronos documentation and fill this with comments
//
//
//
void
REN_Test(float const vertexData[])
{
	int success;
	const GLubyte* c = glGetString(GL_VERSION);

	// INITING
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// SHADER CRAP
	String* vShaderSource = FS_ReadContent("src/Graphics/Shaders/simple.vertex");
	String* fShaderSource = FS_ReadContent("src/Graphics/Shaders/simple.frag");

	uint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vShaderSource->text, NULL);
	glCompileShader(vShader);

	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vShader, 512, NULL, LogBuffer);
		LOG_Log("VERTEX SHADER COMPILATION ERROR");
	}

	uint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderSource->text, NULL);
	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fShader, 512, NULL, LogBuffer);
		LOG_Log("FRAGMENT SHADER COMPILATION ERROR");
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, LogBuffer);
		LOG_Log("PROGRAM SHADER LINKING ERROR");
	}

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	// DRAWING
	// Assign VAO for context
	glBindVertexArray(VAO);

	// Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Set buffer data of the current context (VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(float*) * 3, vertexData, GL_STATIC_DRAW);

	// More data from the buffer - how the vertex should be read
	// TODO: see if more than one object can have the same index - like 0
	int index = 0;
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Enable openGL draw to access this array
	glEnableVertexAttribArray(index);

	// Unbind everything (we will rebind it on draw)
	glBindVertexArray(NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void
REN_Draw()
{
	// Clear
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Select shader
	glUseProgram(shaderProgram);

	// Bind
	glBindVertexArray(VAO);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind
	glBindVertexArray(NULL);

	// Swap back buffer
	glfwSwapBuffers(Window);
}