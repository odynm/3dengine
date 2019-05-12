#include "Renderer.h"

/*
In the case of crop fields, I think is best o have two kinds of sprites: one for
top and one from bottom. Merging the together (overlaping a little bit), it would
not become obvious that they're sprites side by side. They will be animated.

This of course doesn't mean we could not have batches for huge fields.

In both cases, the're needs to be a way to control the crops almost individualy
*/

uint EBO;
uint shaderProgram;

void
REN_Init()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int layerData[NUMBER_OF_LAYERS] = { 10,10,10,10,10 };
    layers = REN_CreateRenderSetLayers(NUMBER_OF_LAYERS, layerData);

    //
    // SHADER STUFF
    //
    int success;
    //glGenBuffers(1, &EBO); // Element buffer, contains element data (indices)

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
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, indices, GL_STATIC_DRAW);
}

// TODO recieve a better list of arguments
void
REN_Add(int layer, float x, float y, int w, int h)
{
#if DEBUG_MODE
    if (layer > NUMBER_OF_LAYERS)
    {
        LOG_Write("RENDERER ERROR: The layer is out of range");
        LOG_RawLog();
    }
#endif

    RenderingObject* rObj = REN_GetAvailableRenderingObject(layers, layer);

    rObj->x = x;
    rObj->y = y;

    glGenVertexArrays(1, &rObj->VAO); // Vertex array objects, contains buffers and how to read them
    glGenBuffers(1, &rObj->VBO); // Vertex buffers, contains vertex data (pos,color)

    glBindVertexArray(rObj->VAO);

    float* vertices = (float*)MEM_Alloc(sizeof(float) * 8);

    vertices[i2(0, 0)] = -w / 2.f;
    vertices[i2(0, 1)] = -h / 2.f;

    vertices[i2(1, 0)] = -w / 2.f;
    vertices[i2(1, 1)] = h / 2.f;

    vertices[i2(2, 0)] = w / 2.f;
    vertices[i2(2, 1)] = h / 2.f;

    vertices[i2(3, 0)] = w / 2.f;
    vertices[i2(3, 1)] = -h / 2.f;

    // Bind buffer to VAO and assign vertices
    glBindBuffer(GL_ARRAY_BUFFER, rObj->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, vertices, GL_STATIC_DRAW);

    // Bind the EBO too to use the same global
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // More data from the VAO - how the vertex should be read
    // The index is the index os the attrib pointer inside the VAO
    // You can have for example one attrib pointer for positions and one for colors
    int indexAttrib = 0;
    glVertexAttribPointer(indexAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

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
    float right = 800.f * fabsf(v);
    float left = 0;
    float top = 800.f * fabsf(v);
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
    /*loc = glGetUniformLocation(shaderProgram, "projMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, orthomatrix);*/

    /*loc = glGetUniformLocation(shaderProgram, "color");
    float c1[] = { 88.0f, 0.44f, 0.2f, 1.0f };
    glUniform4fv(loc, 1, c1);*/

    for (int ilayer = 0; ilayer < NUMBER_OF_LAYERS; ilayer++)
    {
        for (int iobj = 0; iobj < layers[ilayer]->lastSlot + 1; iobj++)
        {
            float* model = (float*)MEM_Alloc(sizeof(float) * 16);
            model[i4(0, 0)] = cosf(t);
            model[i4(0, 1)] = -sinf(t);
            model[i4(1, 0)] = sinf(t);
            model[i4(1, 1)] = cosf(t);

            model[i4(3, 0)] = layers[ilayer]->rObjs[iobj].x * orthomatrix[i4(0, 0)];
            model[i4(3, 1)] = layers[ilayer]->rObjs[iobj].y * orthomatrix[i4(1, 1)];
            model[i4(3, 3)] = 1;

            loc = glGetUniformLocation(shaderProgram, "projMatrix");
            glUniformMatrix4fv(loc, 1, GL_FALSE, orthomatrix);

            loc = glGetUniformLocation(shaderProgram, "modelMatrix");
            glUniformMatrix4fv(loc, 1, GL_FALSE, model);

            glBindVertexArray(layers[ilayer]->rObjs[iobj].VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }

    // Swap back buffer
    glfwSwapBuffers(Window);
}