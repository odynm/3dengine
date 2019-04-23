#include "Renderer.h"

uint Renderer::VBO;
uint Renderer::VAO;
uint Renderer::shaderProgram;

void 
Renderer::Init()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

//
//TODO CLEANUP
//
//
//
void 
Renderer::Test(float const vertexData[])
{
    const GLubyte* c = glGetString(GL_VERSION);

    // INITING
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // SHADER CRAP
    String* vShaderSource = FileSystem::ReadContent
        ("src/Graphics/Shaders/simple.vertex");

    String* fShaderSource = FileSystem::ReadContent
        ("src/Graphics/Shaders/simple.frag");

    int success;
    char errorLog[512];

    uint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSource->text, NULL);
    glCompileShader(vShader);

    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vShader, 512, NULL, errorLog);
        printf("VERTEX SHADER COMPILATION ERROR: "); printf(errorLog);
    }

    uint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSource->text, NULL);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fShader, 512, NULL, errorLog);
        printf("FRAGMENT SHADER COMPILATION ERROR: "); printf(errorLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, errorLog);
        printf("PROGRAM SHADER LINKING ERROR: "); printf(errorLog);
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);


    // DRAWING
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //
    //
    //
    //TODO fix size here "sizeof(vertexData) * 3"
    //
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData) * 3, vertexData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void 
Renderer::Draw()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(Window::window);
}
