#ifndef __OPTIONS_H__
#define __OPTIONS_H__




#include "main.h"




bool optionsInit();


void optionsCleanup();




extern int OPTION_WINDOW_WIDTH, OPTION_WINDOW_HEIGHT;
extern char OPTION_WINDOW_TITLE[256];
extern bool OPTION_FULLSCREEN, OPTION_VSYNC;




#endif // !__OPTIONS_H__
