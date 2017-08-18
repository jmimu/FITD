#ifndef _FONT_
#define _FONT_

#include "types.h"

void initFont(u8 *fontData, int color);
void initFont2(int var1, int var2);
int computeStringWidth(char* string);
void renderText(int x, int y, u8 *surface, char* string);
void drawSlectedText(int x, int y, int index, int color1, int color2);
void drawText(int x, int y, int index, int color);

#endif
