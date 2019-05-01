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
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float*) * 3, vertexData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void
REN_Draw()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(Window);
}