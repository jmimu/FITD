// seg 55

#include "common.h"

typedef struct pakInfoStruct // warning: allignement unsafe
{
  long int discSize;
  long int uncompressedSize;
  char compressionFlag;
  char info5;
  short int offset;
};

typedef struct pakInfoStruct pakInfoStruct;

//#define USE_UNPACKED_DATA

void readPakInfo(pakInfoStruct* pPakInfo, FILE* fileHandle)
{
  if (fread(&pPakInfo->discSize,4,1,fileHandle)!=1)
    printf("Error reading discSize!\n");
  if (fread(&pPakInfo->uncompressedSize,4,1,fileHandle)!=1)
    printf("Error reading uncompressedSize!\n");
  if (fread(&pPakInfo->compressionFlag,1,1,fileHandle)!=1)
    printf("Error reading compressionFlag!\n");
  if (fread(&pPakInfo->info5,1,1,fileHandle)!=1)
    printf("Error reading info5!\n");
  if (fread(&pPakInfo->offset,2,1,fileHandle)!=1)
    printf("Error reading offset!\n");

  pPakInfo->discSize = READ_LE_U32(&pPakInfo->discSize);
  pPakInfo->uncompressedSize = READ_LE_U32(&pPakInfo->uncompressedSize);
  pPakInfo->offset = READ_LE_U16(&pPakInfo->offset);
}

unsigned int PAK_getNumFiles(char* name)
{
  char bufferName[256];
  FILE* fileHandle;
  long int fileOffset;
  char* ptr=0;
  long int size=0;

  strcpy(bufferName, name); // temporary until makeExtention is coded
  strcat(bufferName,".PAK");

  fileHandle = fopen(bufferName,"rb");

  ASSERT(fileHandle);

  fseek(fileHandle,4,SEEK_CUR);
  if (fread(&fileOffset,4,1,fileHandle)!=1)
    printf("Error reading fileOffset!\n");
#ifdef BIG_ENDIAN
  fileOffset = READ_LE_U32(&fileOffset);
#endif
  fclose(fileHandle);

  return((fileOffset/4)-2);
}

int loadPakToPtr(char* name, int index, char* ptr)
{
#ifdef USE_UNPACKED_DATA
  char buffer[256];
  FILE* fHandle;
  int size;

  sprintf(buffer,"%s/%04X.OUT",name,index);

  fHandle = fopen(buffer,"rb");

  if(!fHandle)
    return(0);

  fseek(fHandle,0L,SEEK_END);
  size = ftell(fHandle);
  fseek(fHandle,0L,SEEK_SET);

  if (fread(ptr,size,1,fHandle)!=1)
    printf("Error reading ptr!\n");
  fclose(fHandle);

  return(1);
#else
  char* lptr;

  lptr = loadPak(name,index);

  memcpy(ptr,lptr,getPakSize(name,index));

  free(lptr);

  return(1);
#endif
}

int getPakSize(char* name, int index)
{
#ifdef USE_UNPACKED_DATA
  char buffer[256];
  FILE* fHandle;
  int size;

  sprintf(buffer,"%s/%04X.OUT",name,index);

  fHandle = fopen(buffer,"rb");

  if(!fHandle)
    return(0);

  fseek(fHandle,0L,SEEK_END);
  size = ftell(fHandle);
  fseek(fHandle,0L,SEEK_SET);

  fclose(fHandle);

  return (size);
#else
  char bufferName[256];
  FILE* fileHandle;
  long int fileOffset;
  long int additionalDescriptorSize;
  pakInfoStruct pakInfo;
  char* ptr=0;
  long int size=0;

  strcpy(bufferName, name); // temporary until makeExtention is coded
  strcat(bufferName,".PAK");

  fileHandle = fopen(bufferName,"rb");

  if(fileHandle) // a bit stupid, should return NULL right away
  {
    fseek(fileHandle,(index+1)*4,SEEK_SET);

    if (fread(&fileOffset,4,1,fileHandle)!=1)
      printf("Error reading fileOffset!\n");
#ifdef BIG_ENDIAN
    fileOffset = READ_LE_U32(&fileOffset);
#endif
    fseek(fileHandle,fileOffset,SEEK_SET);

    if (fread(&additionalDescriptorSize,4,1,fileHandle)!=1)
      printf("Error reading additionalDescriptorSize!\n");
#ifdef BIG_ENDIAN
    additionalDescriptorSize = READ_LE_U32(&additionalDescriptorSize);
#endif

    readPakInfo(&pakInfo,fileHandle);

    fseek(fileHandle,pakInfo.offset,SEEK_CUR);

    if(pakInfo.compressionFlag == 0) // uncompressed
    {
      size = pakInfo.discSize;
    }
    else if(pakInfo.compressionFlag == 1) // compressed
    {
      size = pakInfo.uncompressedSize;
    }
    else if(pakInfo.compressionFlag == 4)
    {
      size = pakInfo.uncompressedSize;
    }

    fclose(fileHandle);
  }

  return size;
#endif
}

char* loadPak(char* name, int index)
{
#ifdef USE_UNPACKED_DATA
  char buffer[256];
  FILE* fHandle;
  int size;
  char* ptr;

  sprintf(buffer,"%s/%04X.OUT",name,index);

  fHandle = fopen(buffer,"rb");

  if(!fHandle)
    return NULL;

  fseek(fHandle,0L,SEEK_END);
  size = ftell(fHandle);
  fseek(fHandle,0L,SEEK_SET);

  ptr = (char*)malloc(size);

  if (fread(ptr,size,1,fHandle)!=1)
    printf("Error reading ptr!\n");
  fclose(fHandle);

  return ptr;
#else
  char bufferName[256];
  FILE* fileHandle;
  long int fileOffset;
  long int additionalDescriptorSize;
  pakInfoStruct pakInfo;
  char* ptr=0;

  //makeExtention(bufferName, name, ".PAK");
  strcpy(bufferName, name); // temporary until makeExtention is coded
  strcat(bufferName,".PAK");

  fileHandle = fopen(bufferName,"rb");

  if(fileHandle) // a bit stupid, should return NULL right away
  {
    char nameBuffer[256];

    fseek(fileHandle,(index+1)*4,SEEK_SET);

    if (fread(&fileOffset,4,1,fileHandle)!=1)
      printf("Error reading fileOffset!\n");

#ifdef BIG_ENDIAN
    fileOffset = READ_LE_U32(&fileOffset);
#endif

    fseek(fileHandle,fileOffset,SEEK_SET);

    if (fread(&additionalDescriptorSize,4,1,fileHandle)!=1)
      printf("Error reading additionalDescriptorSize!\n");

#ifdef BIG_ENDIAN
    additionalDescriptorSize = READ_LE_U32(&additionalDescriptorSize);
#endif

    readPakInfo(&pakInfo,fileHandle);

    if(pakInfo.offset)
    {
      ASSERT(pakInfo.offset<256);

      if (fread(nameBuffer,pakInfo.offset,1,fileHandle)!=1)
        printf("Error reading nameBuffer!\n");
#ifdef INTERNAL_DEBUGGER
      printf("Loading %s/%s\n", name,nameBuffer+2);
#endif
    }
    else
    {
      fseek(fileHandle,pakInfo.offset,SEEK_CUR);
    }

    switch(pakInfo.compressionFlag)
    {
    case 0:
      {
        ptr = (char*)malloc(pakInfo.discSize);
        if (fread(ptr,pakInfo.discSize,1,fileHandle)!=1)
        printf("Error reading nameBuffer!\n");
        break;
      }
    case 1:
      {
        char * compressedDataPtr = (char *) malloc(pakInfo.discSize);
        if (fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle)!=1)
        printf("Error reading nameBuffer!\n");
        ptr = (char *) malloc(pakInfo.uncompressedSize);

        PAK_explode(compressedDataPtr, ptr, pakInfo.discSize, pakInfo.uncompressedSize, pakInfo.info5);

        free(compressedDataPtr);
        break;
      }
    case 4:
      {
        char * compressedDataPtr = (char *) malloc(pakInfo.discSize);
        if (fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle)!=1)
        printf("Error reading nameBuffer!\n");
        ptr = (char *) malloc(pakInfo.uncompressedSize);

        PAK_deflate(compressedDataPtr, ptr, pakInfo.discSize, pakInfo.uncompressedSize);

        free(compressedDataPtr);
        break;
      }
    }
    fclose(fileHandle);
  }

  return ptr;
#endif
}
