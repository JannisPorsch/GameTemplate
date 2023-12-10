#include "input.h"
#include "options.h"
#include "core/window.h"




static bool keyStatusCurr[KEY_AMOUNT] = { false };
static bool keyStatusPrev[KEY_AMOUNT] = { false };




void inputUpdate()
{
    for(KEY key = 0; key < KEY_AMOUNT; key++)
    {
        keyStatusPrev[key] = keyStatusCurr[key];

        if(glfwGetKey(mainWindow.handle, OPTION_KEYMAP[key]) == GLFW_PRESS)
            keyStatusCurr[key] = 1;
        else if(glfwGetKey(mainWindow.handle, OPTION_KEYMAP[key]) == GLFW_RELEASE)
            keyStatusCurr[key] = 0;
        else
            DBG("glfwGetKey() returned unknown value");
    }
}


bool isKeyPressed(KEY key)
{
    return keyStatusCurr[key] && !keyStatusPrev[key];
}


bool isKeyDown(KEY key)
{
    return keyStatusCurr[key];
}
