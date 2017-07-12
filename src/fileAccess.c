#include "common.h"
// seg 20
void theEnd(int type, char* name){//  freeScene();  freeAll();
  printf("Error: %s\n", name);
  exit(1);}
char* loadFromItd(char* name){  FILE* fHandle;  char* ptr;
  fHandle = fopen(name,"rb");
  if(!fHandle)  {    theEnd(0,name);    return NULL;  }
  fseek(fHandle,0,SEEK_END);  fileSize = ftell(fHandle);  fseek(fHandle,0,SEEK_SET);
  ptr = (char*)malloc(fileSize);  if(!ptr)  {    theEnd(1,name);    return NULL;  }
  fread(ptr,fileSize,1,fHandle);
  fclose(fHandle);
  return(ptr);}

char* loadPakSafe(char* name, int index){  char* ptr;
  ptr = loadPak(name, index);
  if(!ptr)  {    theEnd(0,name);  }
  return ptr;}
