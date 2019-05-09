#include "Renderer.h"

uint* VAOs;
uint* VBOs;
uint EBO;
uint shaderProgram;

void
REN_Init()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //TODO more reliable array/list system
    VAOs = (uint*)MEM_Alloc(sizeof(uint) * 10);
    VBOs = (uint*)MEM_Alloc(sizeof(uint) * 10);

    //
    // SHADER STUFF
    //
    int success;
    glGenVertexArrays(10, &VAOs[0]); // Vertex array objects, contains buffers and how to read them
    glGenBuffers(10, &VBOs[0]); // Vertex buffers, contains vertex data (pos,color)
    glGenBuffers(1, &EBO); // Element buffer, contains element data (indices)

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

    //

    //
    // BASIC RECTANGLE STUFF
    //
    float const vertices[] = {
        10.f, 10.f, 0.f,
        10.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 10.f, 0.f,
    };

    uint const indices[] = {
        0,1,3,1,2,3
    };

    // Assign the VAO for context
    glBindVertexArray(VAOs[0]);

    // Bind buffer to VAO and assign vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, vertices, GL_STATIC_DRAW);

    // Bind buffer to EBO and assign indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, indices, GL_STATIC_DRAW);

    // More data from the VAO - how the vertex should be read
    // The index is the index os the attrib pointer inside the VAO
    // You can have for example one attrib pointer for positions and one for colors
    int index = 0;
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Tell OpenGL to use this array as vertex attrib
    glEnableVertexAttribArray(index);

    // Unbind everything (we will rebind it on draw)
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindVertexArray(NULL);

    // Use already created VBO and EBO to bind (it works!)
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(index);

    // Unbind everything (we will rebind it on draw)
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindVertexArray(NULL);
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
	t += 0.01f;
	float v = sinf(t);
    
	float n = 2;
	float right = 800.f * fabsf(v);
	float left = 0;
	float top = 600.f * fabsf(v);
	float bottom = 0;
	float* orthomatrix = (float*)MEM_Alloc(sizeof(float) * 16);
	orthomatrix[i4(0, 0)] = n / (right - left);
	orthomatrix[i4(1, 1)] = n / (top - bottom);
    orthomatrix[i4(3, 3)] = 1;

    float* std = (float*)MEM_Alloc(sizeof(float) * 16);
    std[i4(0, 0)] = n / 0.5;
    std[i4(1, 1)] = n / 0.5;
    std[i4(3, 3)] = 1;

    GLint loc;

    // RECT 1
    // Uniforms
    loc = glGetUniformLocation(shaderProgram, "projMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, std);

    loc = glGetUniformLocation(shaderProgram, "color");
    float c1[] = { 88.0f, 0.44f, 0.2f, 1.0f };
    glUniform4fv(loc, 1, c1);

	// Bind
	glBindVertexArray(VAOs[0]);

	// Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    // RECT 2
    // Uniforms
    loc = glGetUniformLocation(shaderProgram, "projMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, orthomatrix);

    loc = glGetUniformLocation(shaderProgram, "color");
    float c2[] = { .0f, 0.5f, 0.2f, 1.0f };
    glUniform4fv(loc, 1, c2);

    // Bind
    glBindVertexArray(VAOs[1]);

    // Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //

	// Unbind
	glBindVertexArray(NULL);

	// Swap back buffer
	glfwSwapBuffers(Window);
}