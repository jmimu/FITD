#ifndef _FILE_ACCESS_
#define _FILE_ACCESS_
#include "types.h"
void theEnd(int type, char* name);
u8 *loadFromItd(char* name);
u8 *loadPakSafe(char* name, u32 index);
#endif
