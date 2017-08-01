// seg 55

#include "common.h"
#include "bmp.h"

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
  u32 fileOffset;
  char* ptr=0;
  u32 size=0;

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

int loadPakToPtr(char* name, int index, u8* ptr)
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
  u8* lptr;

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
  u32 fileOffset;
  u32 additionalDescriptorSize;
  pakInfoStruct pakInfo;
  char* ptr=0;
  u32 size=0;

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


void PAK_debug(char* name, int index,pakInfoStruct *pakInfo,u8 * compressedDataPtr,u8 * uncompressedDataPtr)
{
  char buffer[256];
  FILE* fHandle;
  
  //info
  sprintf(buffer,"debug/%s_%04X.txt",name,index);
  fHandle = fopen(buffer,"w");
  fprintf(fHandle,"PAK %s %d\n",name,index);
  fprintf(fHandle,"pakInfo.discSize %d\n",pakInfo->discSize);
  fprintf(fHandle,"pakInfo.uncompressedSize %d\n",pakInfo->uncompressedSize);
  fprintf(fHandle,"pakInfo.compressionFlag %d\n",pakInfo->compressionFlag);
  fprintf(fHandle,"pakInfo.info5 %d\n",pakInfo->info5);
  fprintf(fHandle,"pakInfo.offset %d\n",pakInfo->offset);
  fclose(fHandle);
  
  //compressed
  sprintf(buffer,"debug/%s_%04X_pak.dat",name,index);
  fHandle = fopen(buffer,"wb");
  fwrite(compressedDataPtr,1,pakInfo->discSize,fHandle); 
  fclose(fHandle);
  
  //uncompressed
  sprintf(buffer,"debug/%s_%04X_unpak.dat",name,index);
  fHandle = fopen(buffer,"wb");
  fwrite(uncompressedDataPtr,1,pakInfo->uncompressedSize,fHandle); 
  fclose(fHandle);
  
  if (pakInfo->uncompressedSize==64000)
  {
    //picture
    sprintf(buffer,"debug/%s_%04X_unpak.pgm",name,index);
    fHandle = fopen(buffer,"wb");
    fwrite("P5 320 200 255 ",1,15,fHandle); 
    fwrite(uncompressedDataPtr,1,64000,fHandle); 
    fclose(fHandle);
    
    
    //save as bmp
    sprintf(buffer,"debug/%s_%04X_unpak.bmp",name,index);
    saveBMP(buffer, uncompressedDataPtr, palette, 320, 200);
  }
  
  if (pakInfo->uncompressedSize==64770)
  {
    //picture
    sprintf(buffer,"debug/%s_%04X_unpak.pgm",name,index);
    fHandle = fopen(buffer,"wb");
    fwrite("P5 320 200 255 ",1,15,fHandle); 
    fwrite(uncompressedDataPtr+770,1,64000,fHandle); 
    fclose(fHandle);
  }
}

u8* loadPak(char* name, u32 index)
{
#ifdef USE_UNPACKED_DATA
  char buffer[256];
  FILE* fHandle;
  int size;
  u8* ptr;

  sprintf(buffer,"%s/%04X.OUT",name,index);

  fHandle = fopen(buffer,"rb");

  if(!fHandle)
    return NULL;

  fseek(fHandle,0L,SEEK_END);
  size = ftell(fHandle);
  fseek(fHandle,0L,SEEK_SET);

  ptr = (u8*)malloc(size);

  if (fread(ptr,size,1,fHandle)!=1)
    printf("Error reading ptr!\n");
  fclose(fHandle);

  return ptr;
#else
  char bufferName[256];
  FILE* fileHandle;
  u32 fileOffset;
  u32 additionalDescriptorSize;
  pakInfoStruct pakInfo;
  u8* ptr=0;

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
        ptr = (u8*)malloc(pakInfo.discSize);
        if (fread(ptr,pakInfo.discSize,1,fileHandle)!=1)
        printf("Error reading nameBuffer!\n");
        break;
      }
    case 1:
      {
        u8 * compressedDataPtr = (u8 *) malloc(pakInfo.discSize);
        if (fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle)!=1)
        printf("Error reading nameBuffer!\n");
        ptr = (u8 *) malloc(pakInfo.uncompressedSize);

        PAK_explode(compressedDataPtr, ptr, pakInfo.discSize, pakInfo.uncompressedSize, pakInfo.info5);

        PAK_debug(name, index,&pakInfo,compressedDataPtr,ptr);

        free(compressedDataPtr);
        break;
      }
    case 4:
      {
        u8 * compressedDataPtr = (u8 *) malloc(pakInfo.discSize);
        if (fread(compressedDataPtr, pakInfo.discSize, 1, fileHandle)!=1)
        printf("Error reading nameBuffer!\n");
        ptr = (u8 *) malloc(pakInfo.uncompressedSize);

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
