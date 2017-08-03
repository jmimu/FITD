#ifndef _PAK_
#define _PAK_

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

struct pakInfoStruct // warning: allignement unsafe
{
  u32 discSize;
  u32 uncompressedSize;
  char compressionFlag;
  char info5;
  u16 offset;
};
typedef struct pakInfoStruct pakInfoStruct;

//#define USE_UNPACKED_DATA

void readPakInfo(pakInfoStruct* pPakInfo, FILE* fileHandle);
unsigned int PAK_getNumFiles(const char* name);
int loadPakToPtr(const char* name, int index, u8* ptr);
int getPakSize(const char* name, int index);
void PAK_debug(const char* name, int index,pakInfoStruct *pakInfo,u8 * compressedDataPtr,u8 * uncompressedDataPtr);
u8* loadPak(const char* name, u32 index);

#endif
