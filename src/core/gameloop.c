#include "gameloop.h"
#include "main.h"
#include "window.h"




static const char* TITLE = "GameTemplate";
static const int WIDTH = 800, HEIGHT = 600;




static bool gameInit();
static void gameCleanup();


static void globalUpdate();
static void globalRender(double alpha);




int gameLoop()
{
    if(!gameInit())
        return -1;
    
    double t = 0.0;
    const double dt = 1.0 / 60.0;

    double currentTime = glfwGetTime();
    double accumulator = 0.0;
    double oneSecTimer = 0.0;

    int ups = 0;
    int fps = 0;

    while (isWindowOpen())
    {
        const double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        oneSecTimer += frameTime;

        if (frameTime > 0.25)
            frameTime = 0.25;
        
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= dt)
        {
            globalUpdate();
            ups++;
            t += dt;
            accumulator -= dt;
        }

        globalRender(accumulator / dt);
        fps++;

        if(oneSecTimer >= 1.0)
        {
            oneSecTimer -= 1.0;
            fps = 0;
            ups = 0;
        }
    }

    gameCleanup();
    return 0;
}




static bool gameInit()
{
    if(!createWindow(TITLE, WIDTH, HEIGHT))
        return 0;
    
    return 1;
}


static void gameCleanup()
{
    windowCleanup();
}


static void globalUpdate()
{
}


static void globalRender(double alpha)
{
    glfwSwapBuffers(mainWindow.handle);
    glfwPollEvents();
}
