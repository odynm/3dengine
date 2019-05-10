#include "Renderer.h"

RenderingObject** layers;

uint EBO;
uint shaderProgram;

void
REN_Init()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //TODO more reliable array/list system
    layers = (RenderingObject**)MEM_Alloc(sizeof(RenderingObject*) * NUMBER_OF_LAYERS);
    for (int i = 0; i < NUMBER_OF_LAYERS; i++)
    {
        layers[i] = (RenderingObject*)MEM_Alloc(sizeof(RenderingObject) * 10);
    }

    //
    // SHADER STUFF
    //
    int success;
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, indices, GL_STATIC_DRAW);
}

// TODO recieve a better list of arguments
void
REN_Add(int layer, int x, int y, int w, int h, int index)
{
#if DEBUG_MODE
    if (layer > NUMBER_OF_LAYERS)
    {
        LOG_Write("RENDERER ERROR: The layer is out of range");
        LOG_RawLog();
    }
#endif

    RenderingObject* rObj = &layers[layer][index];

    glGenVertexArrays(10, &rObj->VAO); // Vertex array objects, contains buffers and how to read them
    glGenBuffers(10, &rObj->VBO); // Vertex buffers, contains vertex data (pos,color)

    glBindVertexArray(rObj->VAO);

    float* vertices = (float*)MEM_Alloc(sizeof(float) * 12);

    vertices[i3(0, 0)] = w;
    vertices[i3(0, 1)] = h;
    vertices[i3(1, 0)] = w;
    vertices[i3(3, 1)] = h;

    // Bind buffer to VAO and assign vertices
    glBindBuffer(GL_ARRAY_BUFFER, rObj->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, vertices, GL_STATIC_DRAW);

    // Bind the EBO too to use the same global
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // More data from the VAO - how the vertex should be read
    // The index is the index os the attrib pointer inside the VAO
    // You can have for example one attrib pointer for positions and one for colors
    int indexAttrib = 0;
    glVertexAttribPointer(indexAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Tell OpenGL to use this array as vertex attrib
    glEnableVertexAttribArray(indexAttrib);

    // Unbind everything (we will rebind it on draw)
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindVertexArray(NULL);
}

#include <math.h>
#include "../Memory.h"

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
    float right = 800.f/* * fabsf(v)*/;
    float left = 0;
    float top = 600.f/* * fabsf(v)*/;
    float bottom = 0;
    float* orthomatrix = (float*)MEM_Alloc(sizeof(float) * 16);
    orthomatrix[i4(0, 0)] = n / (right - left);
    orthomatrix[i4(1, 1)] = n / (top - bottom);
    orthomatrix[i4(3, 3)] = 1;

    float* std = (float*)MEM_Alloc(sizeof(float) * 16);
    std[i4(0, 0)] = n / 0.5f;
    std[i4(1, 1)] = n / 0.5f;
    std[i4(3, 3)] = 1.f;

    GLint loc;

    // Uniforms
    loc = glGetUniformLocation(shaderProgram, "projMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, orthomatrix);

    loc = glGetUniformLocation(shaderProgram, "color");
    float c1[] = { 88.0f, 0.44f, 0.2f, 1.0f };
    glUniform4fv(loc, 1, c1);

    // Bind
    glBindVertexArray(layers[0][0].VAO);

    // Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(NULL);

    // Swap back buffer
    glfwSwapBuffers(Window);
}