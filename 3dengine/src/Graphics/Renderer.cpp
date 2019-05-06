#include "Renderer.h"

uint VBO;
uint VBO2;
uint VAO;
uint VAO2;
uint shaderProgram;

void
REN_Init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void
REN_Test(float const vertexData[], float const vertexData2[])
{
	int success;
	const GLubyte* c = glGetString(GL_VERSION);

	// INITING
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float*) * 3 * 3, vertexData, GL_STATIC_DRAW);

	// More data from the buffer - how the vertex should be read
	// TODO: see if more than one object can have the same index - like 0
	int index = 0;
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Enable openGL draw to access this array
	glEnableVertexAttribArray(index);

	// Assign VAO for context
	glBindVertexArray(VAO2);

	// Bind buffer to VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	// Set buffer data of the current context (VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(float*) * 3 * 3, vertexData2, GL_STATIC_DRAW);

	// More data from the buffer - how the vertex should be read
	// TODO: see if more than one object can have the same index - like 0
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Enable openGL draw to access this array
	glEnableVertexAttribArray(index);

	// Unbind everything (we will rebind it on draw)
	glBindVertexArray(NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

#include <math.h>
#include "../Memory.h"

#define i4(x,y) (x*4 + y)

void
REN_Draw()
{
	// Clear
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Select shader
	glUseProgram(shaderProgram);

	persist float t = 0.0;
	t += 0.01;
	float v = sinf(t);

	float n = 2;
	float right = 800.f /** fabsf(v)*/;
	float left = 0;
	float top = 600.f /** fabsf(v)*/;
	float bottom = 0;
	float zFar = 1.f;
	float zNear = -1.f;
	float* orthomatrix = (float*)MEM_Alloc(sizeof(float) * 16);
	orthomatrix[i4(0, 0)] = n / (right - left);
	orthomatrix[i4(1, 1)] = n / (top - bottom);
	orthomatrix[i4(2, 2)] = -n / (zFar - zNear);
	orthomatrix[i4(3, 3)] = 1;

	GLint loc = glGetUniformLocation(shaderProgram, "projMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, orthomatrix);

	loc = glGetUniformLocation(shaderProgram, "color");
	float c[] = { 1.0f, 0.5f, 0.2f, 1.0f };
	glUniform4fv(loc, 1, c);

	// Bind
	glBindVertexArray(VAO);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 3);

	loc = glGetUniformLocation(shaderProgram, "color");
	float c2[] = { .0f, 0.5f, 0.2f, 1.0f };
	glUniform4fv(loc, 1, c2);

	// Bind
	glBindVertexArray(VAO2);

	// Draw
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind
	glBindVertexArray(NULL);

	// Swap back buffer
	glfwSwapBuffers(Window);
}