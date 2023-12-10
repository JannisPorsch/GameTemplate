#include "options.h"
#include <GLFW/glfw3.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>




static bool initialized = false;
static const char* optionsFilePath = "gamedata";




int OPTION_WINDOW_WIDTH = -1, OPTION_WINDOW_HEIGHT = -1;
char OPTION_WINDOW_TITLE[256] = { 0 };
bool OPTION_FULLSCREEN = false, OPTION_VSYNC = false;
int OPTION_KEYMAP[KEY_AMOUNT] = { 0 };




static bool readOptions(FILE* f);
static bool writeOptions();
static void setOptionsDefault();




bool optionsInit()
{
    if (!initialized)
    {
        errno = 0;
        FILE* f = fopen(optionsFilePath, "r");
        if (!f)
        {
            if (errno == ENOENT)
            {
                setOptionsDefault();
                return writeOptions();
            }
            else return false;
        }

        bool retval = readOptions(f);
        fclose(f);
        return retval;
    }

    return true;
}


void optionsCleanup()
{
    if (initialized)
    {
        writeOptions();
        initialized = false;
    }
}




static bool readOptions(FILE* f)
{
    u32 buffer;

    if (fread(&buffer, 1, sizeof(u32), f) != sizeof(u32)) return false;
    OPTION_WINDOW_WIDTH = (int)ntohl(buffer);

    if (fread(&buffer, 1, sizeof(u32), f) != sizeof(u32)) return false;
    OPTION_WINDOW_HEIGHT = (int)ntohl(buffer);

    char c;
    int i = 0;
    do
    {
        c = fgetc(f);
        OPTION_WINDOW_TITLE[i] = c;
        i++;
    } while (c);

    if (fread(&buffer, 1, sizeof(u32), f) != sizeof(u32)) return false;
    OPTION_FULLSCREEN = (ntohl(buffer) & 0xFF) ? true : false;

    if (fread(&buffer, 1, sizeof(u32), f) != sizeof(u32)) return false;
    OPTION_VSYNC = (ntohl(buffer) & 0xFF) ? true : false;

    for (u8 i = 0; i < KEY_AMOUNT; i++)
    {
        if (fread(&buffer, 1, sizeof(u32), f) != sizeof(u32)) return false;
        OPTION_KEYMAP[i] = (int)ntohl(buffer);
    }
    
    return true;
}


static bool writeOptions()
{
    FILE* f = fopen(optionsFilePath, "w");
    if (!f) return false;

    int titleLength = 0;
    while (OPTION_WINDOW_TITLE[titleLength]) titleLength++;

    bool retval = true;

    u32 buffer = htonl((u32)OPTION_WINDOW_WIDTH);
    if (fwrite(&buffer, 1, sizeof(u32), f) != sizeof(u32)) retval = false;

    buffer = htonl((u32)OPTION_WINDOW_HEIGHT);
    if (fwrite(&buffer, 1, sizeof(u32), f) != sizeof(u32)) retval = false;

    fwrite(OPTION_WINDOW_TITLE, sizeof(char), titleLength + 1, f);

    buffer = htonl((u32)OPTION_FULLSCREEN);
    if (fwrite(&buffer, 1, sizeof(u32), f) != sizeof(u32)) retval = false;

    buffer = htonl((u32)OPTION_VSYNC);
    if (fwrite(&buffer, 1, sizeof(u32), f) != sizeof(u32)) retval = false;

    for (u8 i = 0; i < KEY_AMOUNT; i++)
    {
        buffer = htonl((u32)OPTION_KEYMAP[i]);
        if (fwrite(&buffer, 1, sizeof(u32), f) != sizeof(u32)) retval = false;
    }

    fclose(f);
    return retval;
}


static void setOptionsDefault()
{
    OPTION_WINDOW_WIDTH = 800;
    OPTION_WINDOW_HEIGHT = 600;

    const char* tmpTitle = "GameTemplate";
    for (u8 i = 0; i < 13; i++) OPTION_WINDOW_TITLE[i] = tmpTitle[i];

    OPTION_FULLSCREEN = false;
    OPTION_VSYNC = true;

    int tmpKeyMap[KEY_AMOUNT] =
    {
        GLFW_KEY_ESCAPE,
        GLFW_KEY_ENTER,
        GLFW_KEY_SPACE,
        GLFW_KEY_LEFT_SHIFT,
        GLFW_KEY_W,
        GLFW_KEY_A,
        GLFW_KEY_S,
        GLFW_KEY_D,
        GLFW_KEY_F
    };
    for (u8 i = 0; i < KEY_AMOUNT; i++) OPTION_KEYMAP[i] = tmpKeyMap[i];
}
