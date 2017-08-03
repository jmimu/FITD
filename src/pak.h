#ifndef _PAK_
#define _PAK_

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
unsigned int PAK_getNumFiles(char* name);
int loadPakToPtr(char* name, int index, u8* ptr);
int getPakSize(char* name, int index);
void PAK_debug(char* name, int index,pakInfoStruct *pakInfo,u8 * compressedDataPtr,u8 * uncompressedDataPtr);
u8* loadPak(char* name, u32 index);

#endif
