#ifndef _TATOU_H_
#define _TATOU_H_

int make3dTatou(void);
//////////////// to mode
void paletteFill(void* palette, u8 r, u8 g, u8 b);
void fadeIn(void* palette);
void flip();
void startChrono(unsigned int* chrono);
int evalChrono(unsigned int* chrono);
void process_events( void );
void copyPalette(u8* source, u8* dest);
void copyToScreen(u8* source, u8* dest);
void makeRotationMtx(unsigned int x, unsigned int y, unsigned int z, int* xOut, int* yOut);
void make3dTatouUnk1(int var1,int var2);
void fadeOut(int var1, int var2);
void playSound(int num);
void rotateModel(int x,int y,int z,int alpha,int beta,int gamma,int time);
//////////////
#endif
