#ifndef _ANIM_
#define _ANIM_

#include "types.h"

int anim(int animNum,int arg_2, int arg_4);
int initAnimInBody(int frame, u8 *anim, u8 *body);
short int setInterAnimObjet(int frame, u8* animPtr, u8* bodyPtr);
short int getAnimParam(u8* animPtr);
void initBufferAnim(u8 *buffer, u8 *bodyPtr);


#endif
