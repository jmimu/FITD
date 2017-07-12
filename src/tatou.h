#ifndef _TATOU_H_
#define _TATOU_H_

int make3dTatou(void);
//////////////// to mode
void paletteFill(void* palette, unsigned char r, unsigned char g, unsigned b);void fadeIn(void* palette);void flip();void startChrono(unsigned int* chrono);int evalChrono(unsigned int* chrono);void process_events( void );void copyPalette(char* source, char* dest);void copyToScreen(char* source, char* dest);void makeRotationMtx(unsigned int x, unsigned int y, unsigned int z, int* xOut, int* yOut);void make3dTatouUnk1(int var1,int var2);void fadeOut(int var1, int var2);void playSound(int num);void rotateModel(int x,int y,int z,int alpha,int beta,int gamma,int time);
//////////////
#endif