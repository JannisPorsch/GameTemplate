#include "gameloop.h"
#include "main.h"
#include "window.h"
#include "io/options.h"
#include "io/input.h"
#include "renderPipeline/render.h"
#include "renderPipeline/camera.h"




static bool gameInit();
static void gameCleanup();
static void globalUpdate();
static void globalRender(double alpha);




int gameLoop()
{
    if (!gameInit())
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

        if (frameTime > 0.25) frameTime = 0.25;
        
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

        if (oneSecTimer >= 1.0)
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
    cameraInit((vec3){0.f, 0.f, 1.f}, 1, 1);
    
    if (!optionsInit())
        return false;

    if (!createMainWindow())
    {
        optionsCleanup();
        return false;
    }

    if (!renderPipelineInit())
    {
        optionsCleanup();
        windowCleanup();
        return false;
    }

    int windowWidth, windowHeight;
    getWindowDimensions(&windowWidth, &windowHeight);
    updateCameraProj(windowWidth, windowHeight);

    return true;
}


static void gameCleanup()
{
    renderPipelineCleanup();
    windowCleanup();
    optionsCleanup();
}


static void globalUpdate()
{
    inputUpdate();
    cameraUpdate();
}


static void globalRender(double alpha)
{
    render(alpha);
    
    glfwSwapBuffers(mainWindow.handle);
    glfwPollEvents();
}
