#include "render.h"
#include "io/shaderLoader.h"
#include "io/textureLoader.h"




static GLuint shader, texture, vao, vbo;
static GLfloat vertices[] =
{
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};




bool renderPipelineInit()
{
    if (!loadTexture2D(&texture, "/home/jannis/Downloads/soos.jpg"))
        return false;
    
    if (!shaderProgram(&shader, "/home/jannis/Projects/C/GameTemplate/src/shader/renderTexture.vs", NULL, "/home/jannis/Projects/C/GameTemplate/src/shader/renderTexture.fs"))
    {
        glDeleteTextures(1, &texture);
        return false;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}


void renderPipelineCleanup()
{
    glDeleteTextures(1, &texture);
    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}


void render(double alpha)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUseProgram(shader);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
