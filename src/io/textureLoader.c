#include "textureLoader.h"
#include <stb_image.h>




static bool initialized = false;
static const int CHANNELS[4] =
{
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA
};




bool loadTexture2D(GLuint* texture, const char* imagePath)
{
    if (!initialized)
    {
        stbi_set_flip_vertically_on_load(1);
        initialized = true;
    }

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    int width, height, nrChannels;
    u8 *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, CHANNELS[nrChannels - 1], GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, texture);
        ERROR("loadTexture2D()");
        return false;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}


bool loadTextureCube(GLuint* texture, const char* folderPath)
{
    if (!initialized)
    {
        stbi_set_flip_vertically_on_load(1);
        initialized = true;
    }

    int folderPathLength = 0;
    while (folderPath[folderPathLength]) folderPathLength++;

    char* faces[6] = { NULL };
    for (u8 i = 0; i < 6; i++)
    {
        faces[i] = (char*)malloc((folderPathLength + 12) * sizeof(char));
        if (!faces[i])
        {
            for (u8 j = 0; j < i; j++) free(faces[j]);
            return false;
        }
    }

    const char* tmp[6] =
    {
        "/right.jpg",
        "/left.jpg",
        "/top.jpg",
        "/bottom.jpg",
        "/front.jpg",
        "/back.jpg"
    };

    for (u8 i = 0; i < 6; i++)
    {
        for (int j = 0; j < folderPathLength + 12; j++)
        {
            if (j < folderPathLength) faces[i][j] = folderPath[j];
            else
            {
                char c = tmp[i][j - folderPathLength];
                faces[i][j] = c;
                if (!c) break;
            }
        }
    }

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);

    u8 *data;
    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++)
    {
        data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, CHANNELS[nrChannels - 1], GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            glDeleteTextures(1, texture);
            ERROR("loadTextureCube()");
            return false;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return true;
}
