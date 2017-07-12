#ifndef _RENDERER_H_
#define _RENDERER_H_

extern int BBox3D1;
extern int BBox3D2;
extern int BBox3D3;
extern int BBox3D4;

#define NUM_MAX_POINT_IN_POINT_BUFFER 400
#define NUM_MAX_BONES 40

extern short int pointBuffer[NUM_MAX_POINT_IN_POINT_BUFFER*3];
extern int numOfPoints;

#ifdef USE_GL
void transformPoint(float* ax, float* bx, float* cx);
#else
void transformPoint(int* ax, int* bx, int* cx);
#endif

int renderModel(int var0,int var1,int var2,int var3,int var4,int var5,void* modelPtr);

void computeScreenBox(int x, int y, int z, int alpha, int beta, int gamma, char* bodyPtr);

#endif
