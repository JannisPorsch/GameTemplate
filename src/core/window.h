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
} WINDOW, *PWINDOW;




bool createWindow(const char* title, int width, int height);


void windowCleanup();


bool isWindowOpen();


void toggleFullscreen();


void updateVSync();




extern WINDOW mainWindow;




#endif // !__WINDOW_H__
