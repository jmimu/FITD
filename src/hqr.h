#ifndef _HQR_
#define _HQR_
char* HQR_Get(hqrEntryStruct* hqrPtr, int index);
int printTextSub1(hqrEntryStruct* hqrPtr,int size);
char* printTextSub2(hqrEntryStruct* hqrPtr, int index);
hqrEntryStruct* HQR_InitRessource(char* name, int size, int numEntries);
hqrEntryStruct* HQR_Init(int size,int numEntry);
void HQR_Reset(hqrEntryStruct* hqrPtr);
void HQR_Free(hqrEntryStruct* hqrPtr);
#endif
