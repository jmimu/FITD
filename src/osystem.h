#ifndef _OSYSTEM_H_
#define _OSYSTEM_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifdef INTERNAL_DEBUGGER
#include "debugFont.h"
#endif

#define byte char

#define bool char
#define true 1
#define false 0

/*
class OSystem
{
  public:
  */
    void osystem_init();

    extern int osystem_mouseRight;
    extern int osystem_mouseLeft;

    void osystem_delay(int time);
    void osystem_crossFade(char *buffer, char *palette);
    void osystem_fadeBlackToWhite();
	void osystem_updateImage();
    void osystem_initBuffer(char *buffer, int width, int height);
	void osystem_initVideoBuffer(char *buffer, int width, int height);
    void osystem_putpixel(int x, int y, int pixel);
    void osystem_setColor(byte i, byte R, byte G, byte B);
    void osystem_setPalette(byte * palette);
	void osystem_setPalette320x200(byte * palette);
    void osystem_flip(unsigned char *videoBuffer);
	void osystem_draw320x200BufferToScreen(unsigned char *videoBuffer);
    void osystem_CopyBlockPhys(unsigned char *videoBuffer, int left, int top, int right, int bottom);
    void osystem_drawText(int X, int Y, char *text);
    void osystem_drawTextColor(int X, int Y, char *string, unsigned char R, unsigned char G, unsigned char B);
    void osystem_drawLine(int X1,int X2,int Y1,int Y2,unsigned char color, unsigned char* palette);
	void osystem_getPalette(char* palette);
	void osystem_playSampleFromName(char* sampleName);
  void osystem_playSample(char* samplePtr,int size);
//    void getMouseStatus(mouseStatusStruct * mouseData);

	void osystem_set320x200Mode(bool mode);

	void osystem_startFrame();
	void osystem_stopFrame();
	void osystem_startModelRender();
	void osystem_stopModelRender();
#ifdef USE_GL
	void osystem_fillPoly(float* buffer, int numPoint, unsigned char color,u8 polyType);
	void osystem_draw3dLine(float x1, float y1, float z1, float x2, float y2, float z2, unsigned char color);
	void osystem_draw3dQuad(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, unsigned char color, int transparency);
	void osystem_cleanScreenKeepZBuffer();
  void osystem_drawSphere(float X, float Y, float Z, u8 color, float size);

	void osystem_startBgPoly();
	void osystem_endBgPoly();
	void osystem_addBgPolyPoint(int x, int y);
#else
	void osystem_fillPoly(short int* buffer, int numPoint, unsigned char color);
	void osystem_draw3dLine(int x1, int y1, int z1, int x2, int y2, int z2, unsigned char color);
	void osystem_draw3dQuad(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4, unsigned char color);
#endif

#ifdef INTERNAL_DEBUGGER
	void osystem_drawDebugText(const u32 X, const u32 Y, const u8* string);
#endif
/*
  private:
};*/

#endif
