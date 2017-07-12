#include "common.h"

struct rendererPointStruct
{
  float X;
  float Y;
  float Z;
};

typedef struct rendererPointStruct rendererPointStruct;

#define NUM_MAX_POINT 5000

rendererPointStruct primPointTable[NUM_MAX_POINT];

u32 positionInPointTable = 0;

enum primTypeEnum
{
  primTypeEnum_Line = 0,
  primTypeEnum_Poly = 1,
  primTypeEnum_Point = 2,
  primTypeEnum_Disc = 3,
};

typedef enum primTypeEnum primTypeEnum;

struct primEntryLineStruct
{
  u8 color;

  rendererPointStruct points[2];
};

typedef struct primEntryLineStruct primEntryLineStruct;

struct primEntryPolyStruct
{
  u16 numOfPoints;
  u8 color;
  u8 polyType;
  rendererPointStruct* firstPointPtr;
};

typedef struct primEntryPolyStruct primEntryPolyStruct;

struct primEntryPointStruct
{
  u8 color;
  float X;
  float Y;
  float Z;
};

typedef struct primEntryPointStruct primEntryPointStruct;

struct primEntryDiscStruct
{
  u8 color;
  u16 size;
  float X;
  float Y;
  float Z;
};

typedef struct primEntryDiscStruct primEntryDiscStruct;

struct primEntryStruct
{
  primTypeEnum type;

  union
  {
    primEntryLineStruct lineEntry;
    primEntryPolyStruct polyEntry;
    primEntryPointStruct pointEntry;
    primEntryDiscStruct discEntry;
  };
};

typedef struct primEntryStruct primEntryStruct;

#define NUM_MAX_PRIM_ENTRY 500

primEntryStruct primTable[NUM_MAX_PRIM_ENTRY];

u32 positionInPrimEntry = 0;

int BBox3D1=0;
int BBox3D2=0;
int BBox3D3=0;
int BBox3D4=0;

int renderVar1=0;

int numOfPolyToRender=0;

char renderBuffer[3261];

char* renderVar2=NULL;

int modelFlags = 0;

// DEBUG
int numPointInPoly;
//

int modelCosAlpha;
int modelSinAlpha;
int modelCosBeta;
int modelSinBeta;
int modelCosGamma;
int modelSinGamma;

bool noModelRotation;

int renderX;
int renderY;
int renderZ;

int numOfPoints;
int numOfBones;

short int pointBuffer[NUM_MAX_POINT_IN_POINT_BUFFER*3];
short int cameraSpaceBuffer[NUM_MAX_POINT_IN_POINT_BUFFER*3];
short int bonesBuffer[NUM_MAX_BONES];

bool boneRotateX;
bool boneRotateY;
bool boneRotateZ;

int boneRotateXCos;
int boneRotateXSin;
int boneRotateYCos;
int boneRotateYSin;
int boneRotateZCos;
int boneRotateZSin;

#ifdef USE_GL
char primBuffer[30000];
#else
char primBuffer[8000];
#endif
char* tempOutPtr;

int renderVar3;

void fillpoly(short int * datas, int n, char c);
#ifdef USE_GL
void transformPoint(float* ax, float* bx, float* cx)
{
  int X = (int)*ax;
  int Y = (int)*bx;
  int Z = (int)*cx;
  {
    int* ax = &X;
    int* bx = &Y;
    int* cx = &Z;

    {
      int x;
      int y;
      int z;

      if(transformUseY)
      {
        x = (((((*ax) * transformYSin) - ((*cx) * transformYCos)))>>16)<<1;
        z = (((((*ax) * transformYCos) + ((*cx) * transformYSin)))>>16)<<1;
      }
      else
      {
        x = (*ax);
        z = (*cx);
      }

      //si = x
      //ax = z

      if(transformUseX)
      {
        int tempY = (*bx);
        int tempZ = z;
        y = ((((tempY * transformXSin ) - (tempZ * transformXCos)))>>16)<<1;
        z = ((((tempY * transformXCos ) + (tempZ * transformXSin)))>>16)<<1;
      }
      else
      {
        y = (*bx);
      }

      // cx = y
      // bx = z

      if(transformUseZ)
      {
        int tempX = x;
        int tempY = y;
        x = ((((tempX * transformZSin) - ( tempY * transformZCos)))>>16)<<1;
        y = ((((tempX * transformZCos) + ( tempY * transformZSin)))>>16)<<1;
      }

      *ax = x;
      *bx = y;
      *cx = z;
    }
  }

  *ax = (float)X;
  *bx = (float)Y;
  *cx = (float)Z;
/*  float x = *ax;
  float y = *bx;
  float z = *cx;

  if(transformUseY)
  {
    float tempX = x;
    float tempZ = z;
    x = ((((tempX * transformYSin ) - (tempZ * transformYCos)))/65535.f)*2.f;
    z = ((((tempX * transformYCos ) + (tempZ * transformYSin)))/65535.f)*2.f;
  }

  if(transformUseX)
  {
    float tempY = y;
    float tempZ = z;
    y = ((((tempY * transformXSin) - ( tempZ * transformXCos)))/65535.f)*2.f;
    z = ((((tempY * transformXCos) + ( tempZ * transformXSin)))/65535.f)*2.f;
  }

  if(transformUseZ)
  {
    float tempX = x;
    float tempY = y;
    x = ((((tempX * transformZSin) - (tempY * transformZCos)))/65535.f)*2.f;
    z = ((((tempX * transformZCos) + (tempY * transformZSin)))/65535.f)*2.f;
  }


  *ax = x;
  *bx = y;
  *cx = z; */
}
#else
void transformPoint(int* ax, int* bx, int* cx)
{
  int x;
  int y;
  int z;

  if(transformUseY)
  {
    x = (((((*ax) * transformYSin) - ((*cx) * transformYCos)))>>16)<<1;
    z = (((((*ax) * transformYCos) + ((*cx) * transformYSin)))>>16)<<1;
  }
  else
  {
    x = (*ax);
    z = (*cx);
  }

  //si = x
  //ax = z

  if(transformUseX)
  {
    int tempY = (*bx);
    int tempZ = z;
    y = ((((tempY * transformXSin ) - (tempZ * transformXCos)))>>16)<<1;
    z = ((((tempY * transformXCos ) + (tempZ * transformXSin)))>>16)<<1;
  }
  else
  {
    y = (*bx);
  }

  // cx = y
  // bx = z

  if(transformUseZ)
  {
    int tempX = x;
    int tempY = y;
    x = ((((tempX * transformZSin) - ( tempY * transformZCos)))>>16)<<1;
    y = ((((tempX * transformZCos) + ( tempY * transformZSin)))>>16)<<1;
  }

  *ax = x;
  *bx = y;
  *cx = z;
}
#endif

void prepareRotationMatrix(int transX,int transY,int transZ)
{
  if(transX)
  {
    boneRotateXCos = cosTable[transX&0x3FF];
    boneRotateXSin = cosTable[(transX+0x100)&0x3FF];

    boneRotateX = true;
  }
  else
  {
    boneRotateX = false;
  }

  if(transY)
  {
    boneRotateYCos = cosTable[transY&0x3FF];
    boneRotateYSin = cosTable[(transY+0x100)&0x3FF];

    boneRotateY = true;
  }
  else
  {
    boneRotateY = false;
  }

  if(transZ)
  {
    boneRotateZCos = cosTable[transZ&0x3FF];
    boneRotateZSin = cosTable[(transZ+0x100)&0x3FF];

    boneRotateZ = true;
  }
  else
  {
    boneRotateZ = false;
  }
}

void computeBoneRotation(short int* pointPtr, int numOfPoint)
{
  int i;

  for(i=0;i<numOfPoint;i++)
  {
    int x = *(short int*)pointPtr;
    int y = *(short int*)(pointPtr+1);
    int z = *(short int*)(pointPtr+2);

    if(boneRotateZ)
    {
      int tempX = x;
      int tempY = y;
      x = ((((tempX * boneRotateZSin) - ( tempY * boneRotateZCos)))>>16)<<1;
      y = ((((tempX * boneRotateZCos) + ( tempY * boneRotateZSin)))>>16)<<1;
    }

    if(boneRotateY)
    {
      int tempX = x;
      int tempZ = z;

      x = ((((tempX * boneRotateYSin) - (tempZ * boneRotateYCos)))>>16)<<1;
      z = ((((tempX * boneRotateYCos) + (tempZ * boneRotateYSin)))>>16)<<1;
    }

    if(boneRotateX)
    {
      int tempY = y;
      int tempZ = z;
      y = ((((tempY * boneRotateXSin ) - (tempZ * boneRotateXCos)))>>16)<<1;
      z = ((((tempY * boneRotateXCos ) + (tempZ * boneRotateXSin)))>>16)<<1;
    }

    *(short int*)(pointPtr) = x;
    *(short int*)(pointPtr+1) = y;
    *(short int*)(pointPtr+2) = z;

    pointPtr+=3;
  }
}

void computeBoneRotation2(char* ptr)
{
  if(*(ptr+7))
  {
    int baseBone = *(short int*)(ptr);
    int numPoints = *(short int*)((ptr)+2);

    computeBoneRotation(pointBuffer+baseBone/2,numPoints);
  }
}

void computeRotationMatrix(char* ptr)
{
  int baseBone = *(short int*)(ptr);
  int numPoints = *(short int*)((ptr)+2);
  int temp;
  int temp2;

  computeBoneRotation(pointBuffer+baseBone/2,numPoints);

  temp = *((ptr)+7);

  temp2 = numOfBones - temp;

  do
  {
    if(*((ptr)+6) == temp) // is it a child of me ?
    {
      computeRotationMatrix(ptr); // yes, so apply the transformation to him
    }

    (ptr)+=0x10;
  }while(--temp2);
}

void computeTranslation1(int transX,int transY,int transZ,char* ptr)
{
  int i;
  short int* ptrSource = &pointBuffer[(*(short int*)ptr)/2];
  int number = *(short int*)(ptr+2);

  for(i=0;i<number;i++)
  {
    *(ptrSource++) += transX;
    *(ptrSource++) += transY;
    *(ptrSource++) += transZ;
  }
}

void computeTranslation2(int transX,int transY,int transZ,char* ptr)
{
  int i;
  short int* ptrSource = &pointBuffer[(*(short int*)ptr)/2];
  int number = *(short int*)(ptr+2);

  for(i=0;i<number;i++)
  {
    *(ptrSource++) = (*(ptrSource) * transX)>>8;
    *(ptrSource++) = (*(ptrSource) * transY)>>8;
    *(ptrSource++) = (*(ptrSource) * transZ)>>8;
  }
}

int computeModel(int x,int y,int z,int alpha,int beta,int gamma,void* modelPtr, char* ptr)
{
  char* tempPtr;
  int i;
  char* si;

  renderX = x - translateX;
  renderY = y;
  renderZ = z - translateZ;

  numOfPoints = *(short int*)ptr;
  ptr+=2;

  ASSERT(numOfPoints<NUM_MAX_POINT_IN_POINT_BUFFER); 

  memcpy(pointBuffer,ptr,numOfPoints*3*2);
  ptr+=numOfPoints*3*2;

  numOfBones = *(short int*)ptr;
  ptr+=2;

  ASSERT(numOfBones<NUM_MAX_BONES);
  memcpy(bonesBuffer,ptr,numOfBones*2);
  ptr+=numOfBones*2;

  tempPtr = ptr;

  if(modelFlags&8)
  {
    for(i=0;i<numOfBones;i++)
    {
      int boneDataOffset = bonesBuffer[i];
      char* boneDataPtr = tempPtr + boneDataOffset;

      int type = *(short int*)(boneDataPtr+0x8);

      switch(type)
      {
      case 1:
        {
          int transX;
          int transY;
          int transZ;

          transX = *(short int*)(boneDataPtr+0xA);
          transY = *(short int*)(boneDataPtr+0xC);
          transZ = *(short int*)(boneDataPtr+0xE);

          if(transX || transY || transZ)
          {
            computeTranslation1(transX,transY,transZ,boneDataPtr);
          }

          break;
        }
      case 2:
        {
          int transX;
          int transY;
          int transZ;

          transX = *(short int*)(boneDataPtr+0xA);
          transY = *(short int*)(boneDataPtr+0xC);
          transZ = *(short int*)(boneDataPtr+0xE);

          if(transX || transY || transZ)
          {
            computeTranslation2(transX,transY,transZ,boneDataPtr);
          }

          break;
        }
      }

      prepareRotationMatrix(*(short int*)(boneDataPtr+0x10),*(short int*)(boneDataPtr+0x12),*(short int*)(boneDataPtr+0x14));
      computeBoneRotation2(boneDataPtr);
    }
  }
  else
  {
    *(short int*)(ptr+0xA) = alpha;
    *(short int*)(ptr+0xC) = beta;
    *(short int*)(ptr+0xE) = gamma;

    for(i=0;i<numOfBones;i++)
    {
      int boneDataOffset = bonesBuffer[i];
      char* boneDataPtr = tempPtr + boneDataOffset;

      int transX;
      int transY;
      int transZ;

      transX = *(short int*)(boneDataPtr+0xA);
      transY = *(short int*)(boneDataPtr+0xC);
      transZ = *(short int*)(boneDataPtr+0xE);

      if(transX || transY || transZ)
      {
        int type = *(short int*)(boneDataPtr+0x8);

        switch(type)
        {
        case 0:
          { 
            prepareRotationMatrix(transX,transY,transZ);
            computeRotationMatrix(boneDataPtr);
            break;
          }
        case 1:
          {
            computeTranslation1(transX,transY,transZ,boneDataPtr);
            break;
          }
        case 2:
          {
            computeTranslation2(transX,transY,transZ,boneDataPtr);
            break;
          }
        }
      }
    }
  }

  si = tempPtr;

  for(i=0;i<numOfBones;i++)
  {
    int j;

    int point1;
    int point2;

    short int* ptr1;
    short int* ptr2;

    int number;

    int ax;
    int bx;
    int dx;

    point1 = *(short int*)(si+4);
    point2 = *(short int*)(si);

    ASSERT(point1%2 == 0);
    ASSERT(point2%2 == 0);

    point1/=2;
    point2/=2;

    ASSERT(point1/3<NUM_MAX_POINT_IN_POINT_BUFFER);
    ASSERT(point2/3<NUM_MAX_POINT_IN_POINT_BUFFER);

    ptr1 = (short int*)&pointBuffer[point1];
    ptr2 = (short int*)&pointBuffer[point2];

    number = *(short int*)(si+2);

    ax = ptr1[0];
    bx = ptr1[1];
    dx = ptr1[2];

    for(j=0;j<number;j++)
    {
      *(ptr2++) += ax;
      *(ptr2++) += bx;
      *(ptr2++) += dx;
    }

    if(modelFlags & 8)
    {
      si += 0x18;
    }
    else
    {
      si += 0x10;
    }
  }

  //(*ptr) = si;
  tempOutPtr = si;

  if(modelFlags & 8)
  {
    prepareRotationMatrix(alpha,beta,gamma);
    computeBoneRotation(pointBuffer,numOfPoints);
  }

  {
    char* ptr = (char*)pointBuffer;
    short int* outPtr = cameraSpaceBuffer;
    int k = numOfPoints;
    

#ifdef USE_GL
    float* outPtr2;
#else
    short int* outPtr2;
#endif

    numPointInPoly = numOfPoints;

    for(i=0;i<numOfPoints;i++)
    {
#ifdef USE_GL
      float X = *(short int*)ptr;
      float Y = *(short int*)(ptr+2);
      float Z = *(short int*)(ptr+4);
#else
      int X = *(short int*)ptr;
      int Y = *(short int*)(ptr+2);
      int Z = *(short int*)(ptr+4);
#endif
      ptr+=6;

      X += renderX;
      Y += renderY;

      if(Y>10000) // depth clamp
      {
        *(outPtr++) = -10000;
        *(outPtr++) = -10000;
        *(outPtr++) = -10000;
      }
      else
      {
        Y -= translateY;
        Z += renderZ;

        transformPoint(&X,&Y,&Z);

        *(outPtr++) = (short int)X;
        *(outPtr++) = (short int)Y;
        *(outPtr++) = (short int)Z;
      }
    }

    ptr = (char*)cameraSpaceBuffer;
    outPtr2 = renderPointList;
    
    do
    {
      float X;
      float Y;
      float Z;
      
      X = *(short int*)ptr;
      ptr+=2;
      Y = *(short int*)ptr;
      ptr+=2;
      Z = *(short int*)ptr;
      ptr+=2;

      Z += cameraX;

      if( Z<=50 ) // clipping
      {
        *(outPtr2++) = -10000;
        *(outPtr2++) = -10000;
        *(outPtr2++) = -10000;
      }
      else
      {
        float transformedX = ((X * cameraY) / Z) + cameraCenterX;
        float transformedY;

        *(outPtr2++) = transformedX;

        if(transformedX < BBox3D1)
          BBox3D1 = (int)transformedX;

        if(transformedX > BBox3D3)
          BBox3D3 = (int)transformedX;

        transformedY = ((Y * cameraZ) / Z) + cameraCenterY;

        *(outPtr2++) = transformedY;

        if(transformedY < BBox3D2)
          BBox3D2 = (int)transformedY;

        if(transformedY > BBox3D4)
          BBox3D4 = (int)transformedY;

        *(outPtr2++) = Z; 

     /*   *(outPtr2++) = X;
        *(outPtr2++) = Y;
        *(outPtr2++) = Z; */
      }

      k--;
      if(k==0)
      {
        return(1);
      }

    }while(renderVar1 == 0);
  }

  return(0);
}

int prerenderFlag0(int x,int y,int z,int alpha,int beta,int gamma,void* modelPtr, char* ptr)
{

  int var1;
#ifdef USE_GL
    float* outPtr;
#else
    short int* outPtr;
#endif

  renderX = x - translateX;
  renderY = y;
  renderZ = z - translateZ;

  if(!alpha && !beta && !gamma)
  {
    noModelRotation = true;
  }
  else
  {
    noModelRotation = false;

    modelCosAlpha = cosTable[alpha&0x3FF];
    modelSinAlpha = cosTable[(alpha+0x100)&0x3FF];

    modelCosBeta = cosTable[beta&0x3FF];
    modelSinBeta = cosTable[(beta+0x100)&0x3FF];

    modelCosGamma = cosTable[gamma&0x3FF];
    modelSinGamma = cosTable[(gamma+0x100)&0x3FF];
  }

  var1 = *(short int*)ptr;
  ptr+=2;

// DEBUG
  numPointInPoly = var1;
//

  outPtr = renderPointList;

  do
  {
#ifdef USE_GL
    float X;
    float Y;
    float Z;
    
    X = *(short int*)ptr;
    ptr+=2;
    Y = *(short int*)ptr;
    ptr+=2;
    Z = *(short int*)ptr;
    ptr+=2;
#else
    int X;
    int Y;
    int Z;
    
    X = *(short int*)ptr;
    ptr+=2;
    Y = *(short int*)ptr;
    ptr+=2;
    Z = *(short int*)ptr;
    ptr+=2;
#endif

    if(!noModelRotation)
    {
      {
        float tempX = X;
        float tempY = Y;

        X = (((modelSinGamma * tempX) - (modelCosGamma * tempY))/65536.f)*2.f;
        Y = (((modelCosGamma * tempX) + (modelSinGamma * tempY))/65536.f)*2.f;
      }

      {
        float tempX = X;
        float tempZ = Z;

        X = (((modelSinBeta * tempX) - (modelCosBeta * tempZ))/65536.f)*2.f;
        Z = (((modelCosBeta * tempX) + (modelSinBeta * tempZ))/65536.f)*2.f;
      }

      {
        float tempY = Y;
        float tempZ = Z;

        Y = (((modelSinAlpha * tempY) - (modelCosAlpha * tempZ))/65536.f)*2.f;
        Z = (((modelCosAlpha * tempY) + (modelSinAlpha * tempZ))/65536.f)*2.f;
      }
    }

    X += renderX;
    Y += renderY;

    if(Y>10000) // depth clamp
    {
      *(outPtr++) = -10000;
      *(outPtr++) = -10000;
      *(outPtr++) = -10000;
    }
    else
    {
      float transformedX;
      float transformedY;
      
      Y -= translateY;
      Z += renderZ;

      transformPoint(&X,&Y,&Z);

      Z += cameraX;

      transformedX = ((X * cameraY) / Z) + cameraCenterX;

      *(outPtr++) = transformedX;

      if(transformedX < BBox3D1)
        BBox3D1 = (int)transformedX;

      if(transformedX > BBox3D3)
        BBox3D3 = (int)transformedX;

      transformedY = ((Y * cameraZ) / Z) + cameraCenterY;

      *(outPtr++) = transformedY;

      if(transformedY < BBox3D2)
        BBox3D2 = (int)transformedY;

      if(transformedY > BBox3D4)
        BBox3D4 = (int)transformedY;

      *(outPtr++) = Z; 

    /*  *(outPtr++) = X;
      *(outPtr++) = Y;
      *(outPtr++) = Z; */

    }

    var1--;
    if(var1==0)
    {
      tempOutPtr = ptr;
      return(1);
    }

  }while(renderVar1 == 0);

  tempOutPtr = ptr;
  renderVar1 = 2;

  return(0);
}

char* primVar1;
char* primVar2;

void primFunctionDefault(int primType,char** ptr,char** out)
{
  printf("UnHandled primType %d\n",primType);
  exit(1);
}

void primType0(int primType, char** ptr, char** out) // line tested
{
  int i;
  u8 lineColor;
  float depth = 32000.f;
  primEntryStruct* pCurrentPrimEntry = &primTable[positionInPrimEntry];

  (*ptr)++;
  lineColor = **ptr;
  (*ptr)++;
  (*ptr)++;

  pCurrentPrimEntry->type = primTypeEnum_Line;
  pCurrentPrimEntry->lineEntry.color = lineColor;


  for(i=0;i<2;i++)
  {
    u16 pointIndex;

    pointIndex = *(u16*)(*ptr);
    (*ptr)+=2;

    ASSERT((pointIndex%2) == 0);

    pCurrentPrimEntry->lineEntry.points[i].X = renderPointList[pointIndex/2];
    pCurrentPrimEntry->lineEntry.points[i].Y = renderPointList[(pointIndex/2)+1];
    pCurrentPrimEntry->lineEntry.points[i].Z = renderPointList[(pointIndex/2)+2];

    if(depth>pCurrentPrimEntry->lineEntry.points[i].Z)
      depth = pCurrentPrimEntry->lineEntry.points[i].Z;
  }

  if(depth > 0)
  {
    positionInPrimEntry++;

    numOfPolyToRender++;
  }
}

void primType1(int primType, char** ptr, char** out) // poly
{
  int i;
  int numOfPointInPoly;
  u8 polyColor;
  u8 polyType;
  float depth = 32000.f;

  rendererPointStruct* pCurrentPoint = &primPointTable[positionInPointTable];
  primEntryStruct* pCurrentPrimEntry = &primTable[positionInPrimEntry];

  ASSERT(positionInPointTable < NUM_MAX_POINT);
  ASSERT(positionInPrimEntry < NUM_MAX_PRIM_ENTRY);

  numOfPointInPoly = **ptr;
  (*ptr)++;

  polyType = **ptr;
  (*ptr)++;

  polyColor = **ptr;
  (*ptr)++;

  pCurrentPrimEntry->type = primTypeEnum_Poly;
  pCurrentPrimEntry->polyEntry.numOfPoints = numOfPointInPoly;
  pCurrentPrimEntry->polyEntry.firstPointPtr = pCurrentPoint;
  pCurrentPrimEntry->polyEntry.color = polyColor;
  pCurrentPrimEntry->polyEntry.polyType = polyType;

  for(i=0;i<numOfPointInPoly;i++)
  {
    u16 pointIndex;

    pointIndex = *(u16*)(*ptr);
    (*ptr)+=2;

    ASSERT((pointIndex%2) == 0);

    pCurrentPoint->X = renderPointList[pointIndex/2];
    pCurrentPoint->Y = renderPointList[(pointIndex/2)+1];
    pCurrentPoint->Z = renderPointList[(pointIndex/2)+2];

    if(depth>pCurrentPoint->Z)
      depth = pCurrentPoint->Z;

    pCurrentPoint++;
  }

  if(depth > 0)
  {
    positionInPointTable += numOfPointInPoly;
    positionInPrimEntry++;

    numOfPolyToRender++;
  }
}

void primType2(int primType, char** ptr, char** out) // point
{
  u8 pointColor;
  u16 pointIndex;
  float depth = 32000.f;
  primEntryStruct* pCurrentPrimEntry = &primTable[positionInPrimEntry];

  (*ptr)++;
  pointColor = **ptr;
  (*ptr)++;
  (*ptr)++;

  pCurrentPrimEntry->type = primTypeEnum_Point;
  pCurrentPrimEntry->pointEntry.color = pointColor;

  pointIndex = *(u16*)(*ptr);
  (*ptr)+=2;

  ASSERT((pointIndex%2) == 0);

  pCurrentPrimEntry->pointEntry.X = renderPointList[pointIndex/2];
  pCurrentPrimEntry->pointEntry.Y = renderPointList[(pointIndex/2)+1];
  depth = pCurrentPrimEntry->pointEntry.Z = renderPointList[(pointIndex/2)+2];

  if(depth > 0)
  {
    positionInPrimEntry++;
    numOfPolyToRender++;
  }
}

void primType3(int primType, char** ptr, char** out) // sphere
{
  u8 discColor;
  u16 discSize;
  u16 pointIndex;
  float depth = 32000.f;
  primEntryStruct* pCurrentPrimEntry = &primTable[positionInPrimEntry];

  (*ptr)++;
  discColor = **ptr;
  (*ptr)++;
  (*ptr)++;
  discSize = *(u16*)(*ptr);
  (*ptr)+=2;

  pCurrentPrimEntry->type = primTypeEnum_Disc;
  pCurrentPrimEntry->discEntry.color = discColor;
  pCurrentPrimEntry->discEntry.size = discSize;

  pointIndex = *(u16*)(*ptr);
  (*ptr)+=2;

  ASSERT((pointIndex%2) == 0);

  pCurrentPrimEntry->discEntry.X = renderPointList[pointIndex/2];
  pCurrentPrimEntry->discEntry.Y = renderPointList[(pointIndex/2)+1];
  depth = pCurrentPrimEntry->discEntry.Z = renderPointList[(pointIndex/2)+2];

  if(depth > 0)
  {
    positionInPrimEntry++;
    numOfPolyToRender++;
  }
}

void primType5(int primType, char** ptr, char** out) // draw out of hardClip
{
  int pointNumber;
  short int ax2;

	primVar1 = *out;

	*(short int*)(*out) = *(short int*)(*ptr);
	*out+=2;
	*ptr+=3;

	pointNumber = *(short int*)(*ptr);
	*ptr+=2;

	// here, should check for clip on X Y Z

	*(float*)(*out) = renderPointList[pointNumber/2]; // X
	*out+=sizeof(float);
	*(float*)(*out) = renderPointList[pointNumber/2+1]; // Y
	*out+=sizeof(float);
	*(float*)(*out) = renderPointList[pointNumber/2+2]; // Z
  ax2 = (short int)(*(float*)(*out));
	*out+=sizeof(float);

	primVar2 = *out;

	{
		numOfPolyToRender++;

		*out = renderVar2;

		*(short int*)(*out) = ax2;
		*out+=2;
		*(short int*)(*out) = ax2;
		*out+=2;
		*(short int*)(*out) = primType;
		*out+=2;

		*(char**)(*out) = primVar1;
		*out+=4;

		renderVar2 = *out;
		*out = primVar2;
	}
}

void line(int x1, int y1, int x2, int y2, char c);

void renderStyle0(primEntryStruct* pEntry) // line
{
  osystem_draw3dLine( pEntry->lineEntry.points[0].X,pEntry->lineEntry.points[0].Y,pEntry->lineEntry.points[0].Z,
                      pEntry->lineEntry.points[1].X,pEntry->lineEntry.points[1].Y,pEntry->lineEntry.points[1].Z,
                      pEntry->lineEntry.color);
}

void renderStyle1(primEntryStruct* pEntry) // poly
{
  osystem_fillPoly((float*)pEntry->polyEntry.firstPointPtr,pEntry->polyEntry.numOfPoints, pEntry->polyEntry.color, pEntry->polyEntry.polyType);
}

void renderStyle2(primEntryStruct* pEntry) // point
{
  float transformedSize;
	
  transformedSize = ((5.f * (float)cameraY) / (float)(pEntry->pointEntry.Z+cameraX));

  osystem_drawSphere(pEntry->pointEntry.X,pEntry->pointEntry.Y,pEntry->pointEntry.Z,pEntry->pointEntry.color,transformedSize);
}

void renderStyle3(primEntryStruct* pEntry)
{
  float transformedSize;

  transformedSize = (((float)pEntry->discEntry.size * (float)cameraY) / (float)(pEntry->discEntry.Z+cameraX));

  osystem_drawSphere(pEntry->discEntry.X,pEntry->discEntry.Y,pEntry->discEntry.Z,pEntry->discEntry.color,transformedSize);
}


void defaultRenderFunction(primEntryStruct* buffer)
{
  printf("Unsupported renderType\n");
}

typedef void (*renderFunction)(primEntryStruct* buffer);

renderFunction renderFunctions[]={
  renderStyle0, // line
  renderStyle1, // poly
  renderStyle2, // point
  renderStyle3, // sphere
  defaultRenderFunction,
  defaultRenderFunction,
  defaultRenderFunction,
  defaultRenderFunction,
};

typedef void (*primFunction)(int primType,char** ptr, char** out);

primFunction primFunctionTable[]={
  primType0,
  primType1,
  primType2,
  primType3,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
  primFunctionDefault,
};

int renderModel(int x,int y,int z,int alpha,int beta,int gamma,void* modelPtr)
{
  char* ptr = (char*)modelPtr;
  int numPrim;
  int i;
  char* out;

#ifdef MACOSX
  return 0;
#endif

#ifndef USE_GL2
#endif

  // reinit the 2 static tables
  positionInPointTable = 0;
  positionInPrimEntry = 0;
  //

  BBox3D1 = 0x7FFF;
  BBox3D2 = 0x7FFF;

  BBox3D3 = -0x7FFF;
  BBox3D4 = -0x7FFF;

  renderVar1 = 0;

  numOfPolyToRender = 0;
  
  renderVar2 = renderBuffer;

  modelFlags = READ_LE_S16(ptr);
  ptr+=2;
  ptr+=12;

  ptr+=READ_LE_S16(ptr) + 2;
  
  if(modelFlags&2)
  {
    if(!computeModel(x,y,z,alpha,beta,gamma,modelPtr,ptr))
    {
      BBox3D3 = -32000;
      BBox3D4 = -32000;
      BBox3D1 = 32000;
      BBox3D2 = 32000;
      return(2);
    }
  }
  else
  if(!(modelFlags&4))
  {
    if(!prerenderFlag0(x,y,z,alpha,beta,gamma,modelPtr,ptr))
    {
      BBox3D3 = -32000;
      BBox3D4 = -32000;
      BBox3D1 = 32000;
      BBox3D2 = 32000;
      return(2);
    }
  }
  else
  {
    printf("unsupported model type prerenderFlag4 in renderer !\n");

    BBox3D3 = -32000;
    BBox3D4 = -32000;
    BBox3D1 = 32000;
    BBox3D2 = 32000;
    return(2);
  }

  ptr = tempOutPtr;
  numPrim = READ_LE_S16(ptr);
  ptr+=2;

  if(!numPrim)
  {
    BBox3D3 = -32000;
    BBox3D4 = -32000;
    BBox3D1 = 32000;
    BBox3D2 = 32000;
    return(2);
  }

  out = primBuffer;

  for(i=0;i<numPrim;i++)
  {
    unsigned char primType = *(ptr++);

    if(primFunctionTable[primType]!=primFunctionDefault)
    {
      primFunctionTable[primType](primType,&ptr,&out);
    }
    else
    {
      break;
    }
  }

#if 0
  // TODO: poly sorting by depth
#ifdef USE_GL2
  source = renderBuffer;
#else
  inBuffer = renderBuffer;
  outBuffer = sortedBuffer;

  for(i=0;i<numOfPolyToRender;i++)
  {
    int j;
    int bestIdx;
    int bestDepth = -32000;
    char* readBuffer = renderBuffer;

    for(j=0;j<numOfPolyToRender;j++)
    {
      int depth = READ_LE_S16(readBuffer);

      if(depth>bestDepth)
      {
        bestIdx = j;
        bestDepth = depth;
      }

      readBuffer+=10;
    }

    memcpy(outBuffer,renderBuffer+10*bestIdx,10);
    *(short int*)(renderBuffer+10*bestIdx) = -32000;
    outBuffer+=10;
  }
  source = sortedBuffer;

#endif
#endif
  
//  

  if(!numOfPolyToRender)
  {
    BBox3D3 = -32000;
    BBox3D4 = -32000;
    BBox3D1 = 32000;
    BBox3D2 = 32000;
    return(1); // model ok, but out of screen
  }

//  source += 10 * 1;
  for(i=0;i<numOfPolyToRender;i++)
  {
    renderFunctions[primTable[i].type](&primTable[i]);
  }

//DEBUG
/*  for(i=0;i<numPointInPoly;i++)
  {
    int x;
    int y;

    x = renderPointList[i*3];
    y = renderPointList[i*3+1];

    if(x>=0 && x < 319 && y>=0 && y<199)
    {
      screen[y*320+x] = 15;
    }
  }*/
//

  return(0);
}

void computeScreenBox(int x, int y, int z, int alpha, int beta, int gamma, char* bodyPtr)
{
  char* ptr = (char*)bodyPtr;

  BBox3D1 = 0x7FFF;
  BBox3D2 = 0x7FFF;

  BBox3D3 = -0x7FFF;
  BBox3D4 = -0x7FFF;

  renderVar1 = 0;

  numOfPolyToRender = 0;
  
  renderVar2 = renderBuffer;

  modelFlags = READ_LE_S16(ptr);
  ptr+=2;
  ptr+=12;

  ptr+=READ_LE_S16(ptr) + 2;
  
  if(modelFlags&2)
  {
    computeModel(x,y,z,alpha,beta,gamma,bodyPtr,ptr);
  }
}
