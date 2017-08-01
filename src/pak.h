#ifndef _PAK_
#define _PAK_


u8 *loadPak(char* name, u32 index);
int loadPakToPtr(char* name, int index, u8 *ptr);
int getPakSize(char* name, int index);
unsigned int PAK_getNumFiles(char* name);


#endif
