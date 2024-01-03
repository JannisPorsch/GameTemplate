#ifndef __WINDOW_H__
#define __WINDOW_H__




#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "main.h"




typedef struct _WINDOW
{
    GLFWwindow* handle;
    int x;
    int y;
    int width;
    int height;
    bool fullscreen;
    bool vsync;
} WINDOW, *PWINDOW;




bool createMainWindow();


void windowCleanup();


bool isWindowOpen();


void setFullscreen(bool fullscreen);


void setVSync(bool vsync);


void getWindowDimensions(int* width, int* height);




extern WINDOW mainWindow;




#endif // !__WINDOW_H__
