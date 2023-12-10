#ifndef __TEXTURE_LOADER_H__
#define __TEXTURE_LOADER_H__




#include "main.h"
#include <glad/glad.h>




bool loadTexture2D(GLuint* texture, const char* imagePath);


bool loadTextureCube(GLuint* texture, const char* folderPath);




#endif // !__TEXTURE_LOADER_H__
