#ifndef _PAK_
#define _PAK_

#include <stdio.h>
#include "types.h"

struct pakInfoStruct // warning: allignement unsafe
{
  u32 discSize;
  u32 uncompressedSize;
  char compressionFlag;
  char info5;
  u16 offset;

  u32 dummyFiller;//fix unsafe alignment
};
typedef struct pakInfoStruct pakInfoStruct;

//#define USE_UNPACKED_DATA

void makeExtention(char* bufferName,const char* name,const char* extension);

void readPakInfo(pakInfoStruct* pPakInfo, FILE* fileHandle);
unsigned int PAK_getNumFiles(const char* name);
int loadPakToPtr(const char* name, int index, u8* ptr);
int getPakSize(const char* name, int index);
//void PAK_debug(const char* name, int index,pakInfoStruct *pakInfo,u8 * compressedDataPtr,u8 * uncompressedDataPtr);
u8* loadPak(const char* name, u32 index);

#endif
