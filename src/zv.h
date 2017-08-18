#ifndef _ZV_H_
#define _ZV_H_

#include "types.h"
#include "vars.h"

void getZvCube(u8* bodyPtr, ZVStruct* zvPtr);
void getZvNormal(u8 *bodyPtr, ZVStruct* zvPtr);
void getZvMax(u8 *bodyPtr, ZVStruct* zvPtr);
void makeDefaultZV(ZVStruct* zvPtr);
void getZvMax(u8* bodyPtr, ZVStruct* zvPtr);
#endif
