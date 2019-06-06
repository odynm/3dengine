#include "Renderer.h"

/*
In the case of crop fields, I think is best o have two kinds of sprites: one for
top and one from bottom. Merging them together (overlaping a little bit), it would
not become obvious that they're sprites side by side. They will be animated.

This of course doesn't mean that we can't have batches for huge fields.

In both cases, there needs to be a way to control the crops almost individualy

===
About layers:
Each layer should have it's own atlas
At least for the first iterations, nothing in the game will overlap - collisions will not allow it
If this change, I will need to do some sorting
If everything is show from top ortho, this method will suffice
The most likely setup will be:
terrain
crops
people
machinery
*/

//TODO would it be possible to not use buffer data at all? or to reuse
//the same in all objects. because it will not change between objects, if 
//everything works as expected - everything will be done with shader parameters

uint shaderProgram;
uint VAO;
uint VBO;
// TODO transform this in a vector2
float* positions = (float*)MEM_Alloc(4 * sizeof(float));
uint texture;

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

    glGenVertexArrays(1, &VAO); // Vertex array objects, contains buffers and how to read them
    glGenBuffers(1, &VBO); // Vertex buffers, contains vertex data (pos,color)

    glBindVertexArray(VAO);

    float* vertexData = (float*)MEM_Alloc(16 * sizeof(float));
    // Create basic quad shape and basic coords

    // Note: order of texture coordinates depends directly on order of pixel placement
    // because a vertex coord sets the position of texture in that vertice
    vertexData[i2(0, 0)] = -1.f / 2.f;
    vertexData[i2(0, 1)] = -1.f / 2.f;

    vertexData[i2(1, 0)] = 0.f;
    vertexData[i2(1, 1)] = 1.f;

    vertexData[i2(2, 0)] = -1.f / 2.f;
    vertexData[i2(2, 1)] = 1.f / 2.f;

    vertexData[i2(3, 0)] = 0.f;
    vertexData[i2(3, 1)] = 0.f;

    vertexData[i2(4, 0)] = 1.f / 2.f;
    vertexData[i2(4, 1)] = -1.f / 2.f;

    vertexData[i2(5, 0)] = 1.f;
    vertexData[i2(5, 1)] = 1.f;

    vertexData[i2(6, 0)] = 1.f / 2.f;
    vertexData[i2(6, 1)] = 1.f / 2.f;

    vertexData[i2(7, 0)] = 1.f;
    vertexData[i2(7, 1)] = 0.f;

    /*vertexData[i2(0, 0)] = -1.f / 2.f;
    vertexData[i2(0, 1)] = -1.f / 2.f;

    vertexData[i2(1, 0)] = -1.f / 2.f;
    vertexData[i2(1, 1)] = 1.f / 2.f;

    vertexData[i2(2, 0)] = 1.f / 2.f;
    vertexData[i2(2, 1)] = -1.f / 2.f;

    vertexData[i2(3, 0)] = 1.f / 2.f;
    vertexData[i2(3, 1)] = 1.f / 2.f;*/

    // Bind buffer to VAO and assign vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vertexData, GL_STATIC_DRAW);

    // More data from the VAO - how the vertex should be read
    // The index is the index os the attrib pointer inside the VAO
    // You can have for example one attrib pointer for positions and one for colors
    int indexAttribPos = 0;
	int indexAttribTex = 1;
    //form
    glVertexAttribPointer(indexAttribPos, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    //tex coord
    glVertexAttribPointer(indexAttribTex, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // Tell OpenGL to use this array as vertex attrib
    glEnableVertexAttribArray(indexAttribPos);
	glEnableVertexAttribArray(indexAttribTex);
}

void
REN_AddTexture(byte* textureBuffer, int width, int height)
{
    glGenTextures(1, &texture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);
}

// TODO recieve a better list of arguments
void
REN_AddSprite(int layer, float x, float y, float w, float h, float tx, float ty)
{
    Assert(layer >= NUMBER_OF_LAYERS);

    RenderingObject* rObj = REN_GetAvailableRenderingObject(layers, layer);

    rObj->x = x;
    rObj->y = y;
    rObj->tx = tx;
    rObj->ty = ty;

    positions[i2(rObj->instanceID, 0)] = x;
    positions[i2(rObj->instanceID, 1)] = y;

    // Set size uniform
    glUseProgram(shaderProgram);

    GLint loc;
    char formatedLoc[16];

    sprintf(formatedLoc, "size[%d]", rObj->instanceID);
    loc = glGetUniformLocation(shaderProgram, formatedLoc);
    glUniform2f(loc, w, h);

    sprintf(formatedLoc, "_texture[%d]", rObj->instanceID);
    loc = glGetUniformLocation(shaderProgram, formatedLoc);
    glUniform2f(loc, tx, ty);

    //glBufferSubData(GL_ARRAY_BUFFER, rObj->instanceID * sizeof(float) * 8, sizeof(float) * 8, vertices);

    // Unbind everything (we will rebind it later on draw)
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindVertexArray(NULL);
}

//TODO should not be needed when done
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
    t = 1.0f;
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

    //TODO: it will be best if we had a list of only dirty objects that need update
    for (int ilayer = 0; ilayer < NUMBER_OF_LAYERS; ilayer++)
    {
        //for (int iobj = 0; iobj < layers[ilayer]->lastSlot + 1; iobj++)
        //{
        float* model = (float*)MEM_Alloc(sizeof(float) * 16);
        //TODO trigonometric lookup
        /*model[i4(0, 0)] = cosf(t);
        model[i4(0, 1)] = -sinf(t);
        model[i4(1, 0)] = sinf(t);
        model[i4(1, 1)] = cosf(t);

        model[i4(3, 0)] = layers[ilayer]->rObjs[iobj].x * orthomatrix[i4(0, 0)];
        model[i4(3, 1)] = layers[ilayer]->rObjs[iobj].y * orthomatrix[i4(1, 1)];
        model[i4(3, 3)] = 1;*/

        loc = glGetUniformLocation(shaderProgram, "projMatrix");
        glUniformMatrix4fv(loc, 1, GL_FALSE, orthomatrix);

        loc = glGetUniformLocation(shaderProgram, "modelMatrix");
        glUniformMatrix4fv(loc, 1, GL_FALSE, model);

        loc = glGetUniformLocation(shaderProgram, "texCoordinates");
        glUniformMatrix4fv(loc, 1, GL_FALSE, model);

		loc = glGetUniformLocation(shaderProgram, "_texture");
		glUniform1i(loc, texture);

        for (int iinstance = 0; iinstance < layers[ilayer]->lastSlot; iinstance++)
        {
            RenderingObject rObj = layers[ilayer]->rObjs[iinstance];

            char formatedLoc[32];
            sprintf(formatedLoc, "positions[%d]", rObj.instanceID);
            loc = glGetUniformLocation(shaderProgram, formatedLoc);
            glUniform2f(loc, rObj.x, rObj.y);

            sprintf(formatedLoc, "textureCoords[%d]", rObj.instanceID);
            loc = glGetUniformLocation(shaderProgram, formatedLoc);
            glUniform2f(loc, rObj.tx, rObj.ty);
        }

        // wireframe
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 2);
    }

    // Swap back buffer
    glfwSwapBuffers(Window);
}