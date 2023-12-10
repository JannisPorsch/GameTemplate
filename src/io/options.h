#ifndef __OPTIONS_H__
#define __OPTIONS_H__




#include "main.h"
#include "input.h"




bool optionsInit();


void optionsCleanup();




extern int OPTION_WINDOW_WIDTH, OPTION_WINDOW_HEIGHT;
extern char OPTION_WINDOW_TITLE[256];
extern bool OPTION_FULLSCREEN, OPTION_VSYNC;
extern int OPTION_KEYMAP[KEY_AMOUNT];




#endif // !__OPTIONS_H__
