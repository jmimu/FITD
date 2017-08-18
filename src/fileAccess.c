#include "common.h"
#include "vars.h"
#include "main.h"
#include "pak.h"

// seg 20
void theEnd(int type, char* name)
{
//  freeScene();
  freeAll();
  printf("Error: %s\n", name);
  exit(1);
}
u8* loadFromItd(char* name)
{
  FILE* fHandle;
  u8* ptr;
  fHandle = fopen(name,"rb");
  if(!fHandle)
  {
    theEnd(0,name);
    return NULL;
  }
  fseek(fHandle,0,SEEK_END);
  fileSize = ftell(fHandle);
  fseek(fHandle,0,SEEK_SET);
  ptr = (u8*)malloc(fileSize);

  if(!ptr)
  {
    theEnd(1,name);
    return NULL;
  }
  if (fread(ptr,fileSize,1,fHandle)!=1)
    printf("Error reading \"%s\"\n",name);
  
  fclose(fHandle);
  return(ptr);
}

u8* loadPakSafe(char* name, u32 index)
{
  u8* ptr;
  ptr = loadPak(name, index);
  if(!ptr)
  {
    theEnd(0,name);
  }
  return ptr;
}
