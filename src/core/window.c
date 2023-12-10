#include "window.h"
#include "io/options.h"




WINDOW mainWindow = { 0 };


static bool glfwInitialised = false;
static bool videoModeSet = false;




static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xPos, double yPos);




bool createMainWindow()
{
    if (OPTION_WINDOW_WIDTH <= 0 || OPTION_WINDOW_HEIGHT <= 0)
    {
        ERROR("options not initialized when calling createMainWindow()");
        return false;
    }

    if (!glfwInitialised)
    {
        if (glfwInit() != GLFW_TRUE)
        {
            ERROR("glfwInit()");
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        glfwInitialised = true;
    }

    int viewportWidth = OPTION_WINDOW_WIDTH, viewportHeight = OPTION_WINDOW_HEIGHT;

    if (OPTION_FULLSCREEN)
    {
        mainWindow.width = OPTION_WINDOW_WIDTH;
        mainWindow.height = OPTION_WINDOW_HEIGHT;

        GLFWmonitor* mon = glfwGetPrimaryMonitor();
        if (!mon)
        {
            ERROR("glfwGetPrimaryMonitor()");
            windowCleanup();
            return 0;
        }

        const GLFWvidmode* mode = glfwGetVideoMode(mon);
        if (!mode)
        {
            ERROR("glfwGetVideoMode()");
            windowCleanup();
            return 0;
        }

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        videoModeSet = true;

        mainWindow.handle = glfwCreateWindow(mode->width, mode->height, OPTION_WINDOW_TITLE, mon, NULL);
        viewportWidth = mode->width;
        viewportHeight = mode->height;
    }
    else
    {
        mainWindow.handle = glfwCreateWindow(OPTION_WINDOW_WIDTH, OPTION_WINDOW_HEIGHT, OPTION_WINDOW_TITLE, NULL, NULL);
    }

    if (!mainWindow.handle)
    {
        ERROR("glfwCreateWindow()");
        windowCleanup();
        return 0;
    }

    glfwMakeContextCurrent(mainWindow.handle);

    glfwSetInputMode(mainWindow.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mainWindow.handle, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        ERROR("gladLoadGLLoader()");
        windowCleanup();
        return 0;
    }

    setVSync(OPTION_VSYNC);
    mainWindow.vsync = OPTION_VSYNC;
    mainWindow.fullscreen = OPTION_FULLSCREEN;

    glViewport(0, 0, viewportWidth, viewportHeight);
    glfwSetFramebufferSizeCallback(mainWindow.handle, framebuffer_size_callback);

    return true;
}


void windowCleanup()
{
    if (mainWindow.handle)
        glfwDestroyWindow(mainWindow.handle);
    
    if (glfwInitialised)
        glfwTerminate();

    glfwInitialised = false;
    videoModeSet = false;
}


bool isWindowOpen()
{
    return glfwWindowShouldClose(mainWindow.handle) ? false : true;
}


void setFullscreen(bool fullscreen)
{
    if (!fullscreen && mainWindow.fullscreen)
    {
        glfwSetWindowMonitor(mainWindow.handle, NULL, mainWindow.x, mainWindow.y, mainWindow.width, mainWindow.height, 0);
        mainWindow.fullscreen = false;
    }
    else if (fullscreen && !mainWindow.fullscreen)
    {
        GLFWmonitor* mon = glfwGetPrimaryMonitor();
        if (mon)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(mon);
            if (mode)
            {
                if (!videoModeSet)
                {
                    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
                    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
                    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
                    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
                }

                glfwGetWindowPos(mainWindow.handle, &mainWindow.x, &mainWindow.y);
                glfwGetWindowSize(mainWindow.handle, &mainWindow.width, &mainWindow.height);
                glfwSetWindowMonitor(mainWindow.handle, mon, 0, 0, mode->width, mode->height, mode->refreshRate);
                mainWindow.fullscreen = true;
            }
            else ERROR("glfwGetVideoMode()");

        }
        else ERROR("glfwGetPrimaryMonitor()");

    }
}


void setVSync(bool vsync)
{
    glfwSwapInterval(vsync ? 1 : 0);
    mainWindow.vsync = vsync;
}




static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    // later update camera & gbuffer
}


static void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
}
