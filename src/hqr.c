#include "hqr.h"
#include "common.h"
#include "freezetime.h"
#include "fileAccess.h"
#include "pak.h"

hqrSubEntryStruct* quickFindEntry(int index, int numMax, hqrSubEntryStruct* ptr) // no RE. Original was probably faster
{
  int i;

  for(i=0;i<numMax;i++)
  {
    if((ptr[i].key == index) && ptr[i].ptr)
    {
      return(&ptr[i]);
    }
  }

  return(NULL);
}

hqrEntryStruct* HQR_InitRessource(const char* name, int size, int numEntries)
{
  int i;
  hqrEntryStruct* dest;

  dest = (hqrEntryStruct*)malloc(sizeof(hqrEntryStruct));

  if(!dest)
    return NULL;

  numEntries = 2000;

  strcpy(dest->string,"        ");
  strncpy(dest->string,name,8);

  dest->sizeFreeData = size;
  dest->maxFreeData = size;
  dest->numMaxEntry = numEntries;
  dest->numUsedEntry = 0;
  dest->entries = (hqrSubEntryStruct*)malloc(numEntries*sizeof(hqrSubEntryStruct));

  for(i=0;i<numEntries;i++)
  {
    dest->entries[i].ptr = NULL;
  }

  return(dest);
}

int printTextSub1(hqrEntryStruct* hqrPtr,int size)
{
  hqrSubEntryStruct* dataPtr1;
  hqrSubEntryStruct* dataPtr2;
  int key;
  int entryNum;

  if(hqrPtr->sizeFreeData<size)
    return(-1);

  entryNum = hqrPtr->numUsedEntry;

  dataPtr1 = dataPtr2 = hqrPtr->entries;

  key = hqrKeyGen;

  dataPtr1[entryNum].key = key;

//  dataPtr1[entryNum].offset = hqrPtr->maxFreeData - hqrPtr->sizeFreeData;
  dataPtr1[entryNum].size = size;

  hqrPtr->numUsedEntry++;
  hqrPtr->sizeFreeData -= size;

  hqrKeyGen++;

  return(key);
}

u8* printTextSub2(hqrEntryStruct* hqrPtr, int index)
{
  hqrSubEntryStruct* ptr;
  hqrSubEntryStruct* dataPtr;

  if(index<0)
    return NULL;

  dataPtr = hqrPtr->entries;

  ptr = quickFindEntry(index, hqrPtr->numUsedEntry, dataPtr);

  if(!ptr)
    return NULL;

  return(ptr->ptr);
}

void moveHqrEntry(hqrEntryStruct* hqrPtr, int index)
{
/*  hqrSubEntryStruct* hqrSubPtr = (hqrSubEntryStruct*)(((char*)hqrPtr)+sizeof(hqrEntryStruct));
  hqrSubEntryStruct* hqrSubPtr2 = hqrSubPtr;

  int size = hqrSubPtr[index].size;

  if(hqrPtr->numUsedEntry - 1 > index ) //if not last entry
  {
    char* dest = hqrPtr->dataPtr + hqrSubPtr2[index].offset;
    char* src = dest + size;

    memcpy(dest,src,hqrPtr->dataPtr + hqrPtr->maxFreeData - src);

    dest = (char*)&hqrSubPtr2[index];
    src = (char*)&hqrSubPtr2[index+1];
    memcpy(dest,src,hqrPtr->numMaxEntry-(index+1) * sizeof(hqrSubEntryStruct));
  }*/

  int size = hqrPtr->entries[index].size;

  free(hqrPtr->entries[index].ptr);

  hqrPtr->numUsedEntry --;
  hqrPtr->sizeFreeData += size;
}

u8* HQR_Get(hqrEntryStruct* hqrPtr, int index)
{
  //printf("HQR_Get from %s\n",hqrPtr->string);
  hqrSubEntryStruct* foundEntry;
  
  if(index<0)
    return NULL;

  foundEntry = quickFindEntry(index,hqrPtr->numUsedEntry,hqrPtr->entries);

  if(foundEntry)
  {
    foundEntry->lastTimeUsed = timer;
    hqrVar1 = 0;
    //printf("Found entry: %p\n",foundEntry->ptr);
    return(foundEntry->ptr);
  }
  else
  {
/*    int size;
    unsigned int time;
    char* ptr;
    
    freezeTime();
    size = getPakSize(hqrPtr->string,index);

    if(size>=hqrPtr->maxFreeData)
    {
      theEnd(1,hqrPtr->string);
    }

    time = timer;

    foundEntry = hqrSubPtr;

    while(size>hqrPtr->sizeFreeData || hqrPtr->numUsedEntry>= hqrPtr->numMaxEntry)
    {
      int bestEntry = 0;
      unsigned int bestTime = 0;
      int entryIdx = 0;

      for(entryIdx = 0; entryIdx<hqrPtr->numUsedEntry; entryIdx++)
      {
        if(time - foundEntry[entryIdx].lastTimeUsed > bestTime)
        {
          bestTime = time - foundEntry[entryIdx].lastTimeUsed;
          bestEntry = entryIdx;
        }
      }

      moveHqrEntry(hqrPtr,bestEntry);
    }

    ptr = hqrPtr->dataPtr + (hqrPtr->maxFreeData - hqrPtr->sizeFreeData);

    if(!loadPakToPtr(hqrPtr->string,index,ptr))
    {
      theEnd(1,hqrPtr->string);
    }

    hqrVar1 = 1;

    foundEntry[hqrPtr->numUsedEntry].key = index;
    foundEntry[hqrPtr->numUsedEntry].lastTimeUsed = timer;
    foundEntry[hqrPtr->numUsedEntry].offset = hqrPtr->maxFreeData - hqrPtr->sizeFreeData;
    foundEntry[hqrPtr->numUsedEntry].size = size;

    hqrPtr->numUsedEntry++;
    hqrPtr->sizeFreeData -= size;

    unfreezeTime();*/

    int size;
    unsigned int time;
    u8* ptr;
    int i;
    
    freezeTime();
    size = getPakSize(hqrPtr->string,index);

    if(size>=hqrPtr->maxFreeData)
    {
      theEnd(1,hqrPtr->string);
    }

    time = timer;

    for(i=0;i<hqrPtr->numMaxEntry;i++)
    {
      if(hqrPtr->entries[i].ptr == NULL)
      {
        foundEntry = &hqrPtr->entries[i];
        break;
      }
    }

      ASSERT(foundEntry);

//    foundEntry = hqrSubPtr;

    hqrVar1 = 1;

    foundEntry->key = index;
    foundEntry->lastTimeUsed = timer;
    //foundEntry[hqrPtr->numUsedEntry].offset = hqrPtr->maxFreeData - hqrPtr->sizeFreeData;
    foundEntry->size = size;
    foundEntry->ptr = (u8 *)malloc(size);

    ptr = foundEntry->ptr;

    loadPakToPtr(hqrPtr->string,index,foundEntry->ptr);

    hqrPtr->numUsedEntry++;
    hqrPtr->sizeFreeData -= size;

    unfreezeTime();

    //printf("HQR_Get: \n");
    //for (i=0;i<size;i++)
    //    printf("%u ",*(ptr+i));
    //printf("\n");

    return(ptr);
  }
}

hqrEntryStruct* HQR_Init(int size,int numEntry)
{
  int i;
  hqrEntryStruct* dest;
  char* dest2;

  ASSERT(size > 0);
  ASSERT(numEntry > 0);

  dest = (hqrEntryStruct*)malloc(sizeof(hqrEntryStruct));

  numEntry = 2000;

  ASSERT_PTR(dest);

  if(!dest)
    return NULL;

  dest2 = (char*)malloc(size);

  ASSERT_PTR(dest2);

  if(!dest2)
    return NULL;

  strcpy(dest->string,"_MEMORY_");

  dest->sizeFreeData = size;
  dest->maxFreeData = size;
  dest->numMaxEntry = numEntry;
  dest->numUsedEntry = 0;
  dest->entries = (hqrSubEntryStruct*)malloc(numEntry*sizeof(hqrSubEntryStruct));

  for(i=0;i<numEntry;i++)
  {
    dest->entries[i].ptr = NULL;
  }

  return(dest);
}

void HQR_Reset(hqrEntryStruct* hqrPtr)
{
  int i;
  hqrPtr->sizeFreeData = hqrPtr->maxFreeData;
  hqrPtr->numUsedEntry = 0;

  for(i=0;i<hqrPtr->numMaxEntry;i++)
  {
    if(hqrPtr->entries[i].ptr)
      free(hqrPtr->entries[i].ptr);

    hqrPtr->entries[i].ptr = NULL;
  }

}

void HQR_Free(hqrEntryStruct* hqrPtr)
{
  int i;

  if(!hqrPtr)
    return;

  for(i=0;i<hqrPtr->numMaxEntry;i++)
  {
    if(hqrPtr->entries[i].ptr)
      free(hqrPtr->entries[i].ptr);
  }

  free(hqrPtr);
}
