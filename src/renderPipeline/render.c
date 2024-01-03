#include "render.h"
#include "io/shaderLoader.h"
#include "io/textureLoader.h"
#include "renderPipeline/camera.h"




#ifdef _DEBUG
static GLuint shaderDebug;
#endif // _DEBUG
static GLuint location = 0;
static GLuint shaderRenderTexture, texture, vaoRenderTexture, vboRenderTexture;
static GLuint shaderTriangle, vaoTriangle, vboTriangle;
static GLfloat triangleVerts[] =
{
    // positions
    -0.5f, -0.5f, 0.f,
    0.5f, -0.5f, 0.f,
    0.f, 0.5f, 0.f
};
static GLfloat screenQuadVertices[] =
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
    
    if (!shaderProgram(&shaderRenderTexture, "/home/jannis/Projects/C/GameTemplate/src/shader/renderTexture.vs", NULL, "/home/jannis/Projects/C/GameTemplate/src/shader/renderTexture.fs"))
    {
        glDeleteTextures(1, &texture);
        return false;
    }

    if (!shaderProgram(&shaderTriangle, "/home/jannis/Projects/C/GameTemplate/src/shader/triangle.vs", NULL, "/home/jannis/Projects/C/GameTemplate/src/shader/triangle.fs"))
    {
        glDeleteTextures(1, &texture);
        glDeleteProgram(shaderRenderTexture);
        return false;
    }

#ifdef _DEBUG
    if (!shaderProgram(&shaderDebug, "/home/jannis/Projects/C/GameTemplate/src/shader/debug.vs", NULL, "/home/jannis/Projects/C/GameTemplate/src/shader/debug.fs"))
    {
        glDeleteTextures(1, &texture);
        glDeleteProgram(shaderRenderTexture);
        glDeleteProgram(shaderTriangle);
        return false;
    }
#endif // _DEBUG

    glGenVertexArrays(1, &vaoRenderTexture);
    glGenBuffers(1, &vboRenderTexture);

    glBindVertexArray(vaoRenderTexture);
    glBindBuffer(GL_ARRAY_BUFFER, vboRenderTexture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), screenQuadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &vaoTriangle);
    glGenBuffers(1, &vboTriangle);

    glBindVertexArray(vaoTriangle);
    glBindBuffer(GL_ARRAY_BUFFER, vboTriangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerts), triangleVerts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}


void renderPipelineCleanup()
{
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderRenderTexture);
    glDeleteVertexArrays(1, &vaoRenderTexture);
    glDeleteBuffers(1, &vboRenderTexture);

    glDeleteProgram(shaderTriangle);
    glDeleteVertexArrays(1, &vaoTriangle);
    glDeleteBuffers(1, &vboTriangle);

#ifdef _DEBUG
    glDeleteProgram(shaderDebug);
#endif // _DEBUG
}


void render(double alpha)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUseProgram(shaderRenderTexture);
    glBindVertexArray(vaoRenderTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(shaderTriangle);
    location = glGetUniformLocation(shaderTriangle, "projView");
    glUniformMatrix4fv(location, 1, GL_FALSE, (const float*)(currentCamera->projView));
    glBindVertexArray(vaoTriangle);
    glDrawArrays(GL_TRIANGLES, 0, 3);

#ifdef _DEBUG
    if (debugCameraInUse())
    {
        glUseProgram(shaderDebug);
        glBindVertexArray(vaoRenderTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
#endif // _DEBUG
}
