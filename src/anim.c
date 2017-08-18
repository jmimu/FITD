#include "anim.h"
#include "common.h"
#include "vars.h"
#include "main.h"
#include "hqr.h"

int initAnimInBody(int frame, u8* anim, u8* body)
{
  s16 temp;
  s16 ax;
  s16 cx;
  s16 bx;
  u8* saveAnim;
  int i;
  int flag;

  flag = (*(s16*)body);

  temp = *(s16*)anim;
  anim+=2;

  if(frame >= temp)
  {
    return(0);
  }

  ax = *(s16*)anim;
  anim+=2;

  cx = ax;

  if(flag&8)
  {
    ax = ((ax<<4)+8)*frame;
  }
  else
  {
    ax = ((ax+1)<<3)*frame;
  }

  anim+=ax;

  animCurrentTime = *(s16*)anim;
  animKeyframeLength = animCurrentTime;

  if(!(flag&2))
  {
    return(0);
  }

  body +=14;

  *(u8**)(body+2) = anim;
  *(u16*)(body+6) = timer;

  body+= *(s16*)body;
  body+=2;

  ax = *(s16*)body;
  bx = ax;

  body+=(((ax << 1) + bx) << 1) +2;

  bx = ax = *(s16*)body;

  body+=bx<<1;

  if(cx > ax)
    cx = ax;

  body+=10;

  saveAnim = anim;

  anim += 8;

  for(i=0;i<cx;i++)
  {
    *(s16*)(body) = *(s16*)(anim);
    body+=2;
    anim+=2;
    *(s16*)(body) = *(s16*)(anim);
    body+=2;
    anim+=2;
    *(s16*)(body) = *(s16*)(anim);
    body+=2;
    anim+=2;
    *(s16*)(body) = *(s16*)(anim);
    body+=2;
    anim+=2;

    if(flag&8)
    {
      *(s16*)(body) = *(s16*)(anim);
      body+=2;
      anim+=2;
      *(s16*)(body) = *(s16*)(anim);
      body+=2;
      anim+=2;
      *(s16*)(body) = *(s16*)(anim);
      body+=2;
      anim+=2;
      *(s16*)(body) = *(s16*)(anim);
      body+=2;
      anim+=2;
    }

    body+=8;
  }

  anim = saveAnim;

  anim+=2;

  animRot2 = *(s16*)anim;
  anim+=2;
  animRot3 = *(s16*)anim;
  anim+=2;
  animRot1 = *(s16*)anim;
  anim+=2;

  return(1);

}

int anim(int animNum,int arg_2, int arg_4)
{
  if(animNum == currentProcessedActorPtr->ANIM)
  {
    if(!(currentProcessedActorPtr->flags & 1))
    {
      if(currentProcessedActorPtr->flags & 8)
      {
        deleteSub(currentProcessedActorIdx);
      }

      currentProcessedActorPtr->flags |= 1;

      initAnimInBody(currentProcessedActorPtr->FRAME, HQR_Get(listAnim,animNum), HQR_Get(listBody, currentProcessedActorPtr->bodyNum));

      currentProcessedActorPtr->field_40 = arg_2;
      currentProcessedActorPtr->field_42 = arg_4;
    }
    else
    {
      currentProcessedActorPtr->field_40 = arg_2;
      currentProcessedActorPtr->field_42 = arg_4;

      return(0);
    }
  }
  else
  {
    if(animNum == -1)
    {
      currentProcessedActorPtr->field_44 = -2;
      return(1);
    }

    if(!(currentProcessedActorPtr->flags & 1))
    {
      if(currentProcessedActorPtr->flags & 8)
      {
        deleteSub(currentProcessedActorIdx);
      }

      currentProcessedActorPtr->flags |= 1;

      initAnimInBody(currentProcessedActorPtr->FRAME, HQR_Get(listAnim,animNum), HQR_Get(listBody, currentProcessedActorPtr->bodyNum));

      currentProcessedActorPtr->field_40 = arg_2;
      currentProcessedActorPtr->field_42 = arg_4;
    }
    else
    {
      if((currentProcessedActorPtr->field_40 & 2) || (currentProcessedActorPtr->field_46 & 2))
        return(0);

    }

    currentProcessedActorPtr->field_44 = animNum;
    currentProcessedActorPtr->field_46 = arg_2;
    currentProcessedActorPtr->field_48 = arg_4;
  }

  return(1);
}

void initBufferAnim(u8* buffer, u8* bodyPtr)
{
  int flag = *(s16*)bodyPtr;
  if(flag & 2)
  {
    u8* source = bodyPtr+0x10;
    s16 ax;
    int cx;
    int i;

    *(u16*)(source+4) = (u16)timer;
    *(u8**)(source) = buffer;

    source += *(s16*)(source-2);

    ax = *(s16*)(source);

    ax = (((ax * 2) + ax)*2)+2;

    source += ax;

    cx = *(s16*)source;
    
    source += cx*2;

    buffer+= 8;
    source+= 10;

    for(i=0;i<cx;i++)
    {
      *(s16*)(buffer) = *(s16*)(source);
      *(s16*)(buffer+2) = *(s16*)(source+2);
      *(s16*)(buffer+4) = *(s16*)(source+4);
      *(s16*)(buffer+6) = *(s16*)(source+6);

      buffer+=8;
      source+=8;

      if(flag&8)
      {
        *(s16*)(buffer) = *(s16*)(source);
        *(s16*)(buffer+2) = *(s16*)(source+2);
        *(s16*)(buffer+4) = *(s16*)(source+4);
        *(s16*)(buffer+6) = *(s16*)(source+6);

        buffer+=8;
        source+=8;
      }

      source+=8;
    }

  }
}

s16 getAnimParam(u8* animPtr)
{
  return(*(s16*)animPtr);
}

s16 getAnimType(u8** bodyPtr) // local
{
  s16 temp = *(s16*)animVar1;

  animVar1+=2;

  animVar4+=2;

  *(s16*)(*bodyPtr) = temp;
  (*bodyPtr)+=2;

  return(temp);
}

void processAnimRotation(u8** bodyPtr, int bp, int bx) // local
{
    //printf("animVar4: %p\n",animVar4);
    //printf("bodyPtr: %p\n",bodyPtr);
    //printf("*bodyPtr: %p\n",*bodyPtr);

  s16 oldRotation = *(s16*)animVar4;
  s16 newRotation;
  s16 diff;
  
  animVar4+=2;

  newRotation = *(s16*)animVar1;
  animVar1+=2;

  diff = newRotation - oldRotation;

  if(diff == 0)
  {
    *(s16*)(*bodyPtr) = newRotation;
  }
  else
  {
    if(diff <= 0x200)
    {
      if(diff >= -0x200)
      {
        *(s16*)(*bodyPtr) = ((diff*bp)/bx) + oldRotation;
      }
      else
      {
        newRotation += 0x400;
        newRotation -= oldRotation;

        *(s16*)(*bodyPtr) = ((newRotation*bp)/bx) + oldRotation;
      }
    }
    else
    {
      oldRotation += 0x400;
      newRotation -= oldRotation;

      *(s16*)(*bodyPtr) = ((newRotation*bp)/bx) + oldRotation;
    }
  }

  (*bodyPtr)+=2;
}

void processAnimTranslation(u8** bodyPtr, int bp, int bx) // local
{
  s16 cx = *(s16*)animVar4;
  s16 ax;
  animVar4+=2;

  ax = *(s16*)animVar1;
  animVar1+=2;

  if(ax == cx)
  {
    *(s16*)(*bodyPtr) = ax;
  }
  else
  {
    *(s16*)(*bodyPtr) = (((ax - cx)*bp)/bx) + cx;
  }

  (*bodyPtr)+=2;
}

s16 setInterAnimObjet(int frame, u8* animPtr, u8* bodyPtr)
{
  //printf("setInterAnimObjet: %d %p %p\n",frame,animPtr,bodyPtr);
  //printf("check bodyPtr1: %p %p\n",bodyPtr,*(u8**)(bodyPtr));
  //~ for (long i=0;i<100;i++)
  //~ {
      //~ printf("%d: %d ",i,*(bodyPtr+i));
  //~ }

  //printf("\n");
  int numOfBonesInAnim = *(s16*)(animPtr+2);
  u16 keyframeLength;
  u16 timeOfKeyframeStart;
  u8* animBufferPtr;
  u16 ax;
  u16 bx;
  u16 time;
  int bp;
  int flag;

  flag = *(s16*)bodyPtr;
  
  animPtr+=4;

  if(flag&8)
  {
    animPtr += ((numOfBonesInAnim<<4)+8)*frame; // seek to keyframe
  }
  else
  {
    animPtr += ((numOfBonesInAnim+1)*8)*frame; // seek to keyframe
  }

  // animVar1 = ptr to the current keyFrame
  animVar1 = animPtr;

  keyframeLength = *(u16*)animPtr; // keyframe length

  if(!((*(s16*)bodyPtr) & 2)) // do not anim if the model can't be animated
  {
    return(0);
  }
  //printf("check bodyPtr1.8: %p %p\n",bodyPtr,*(u8**)(bodyPtr));


  bodyPtr+=16;
  //printf("check bodyPtr1.9: %p %p %p\n",bodyPtr,*(u8**)(bodyPtr),bodyPtr);

  animVar3 = bodyPtr;
  //printf("animVar3: %p\n",animVar3);

  timeOfKeyframeStart = *(u16*)(bodyPtr+4); // time of start of keyframe
  
  //printf("check bodyPtr2: %p %p\n",bodyPtr,*(u8**)(bodyPtr));


  //printf("Set animBufferPtr to %p\n",*(u8**)(bodyPtr));
  animBufferPtr = *(u8**)(bodyPtr);

  animBufferPtr=(u8*)((u32)animBufferPtr);//TODO: is it the ony way to fix 64 bit crash?

  if(!animBufferPtr)
  {
    animBufferPtr = animVar1;
    //printf("Set animBufferPtr to %p\n",animVar1);
  }

  // animVar4 = ptr to previous key frame
  //printf("Set animVar4 to %p\n",animBufferPtr);
  animVar4 = animBufferPtr;

  bodyPtr+= *(s16*)(bodyPtr-2);

  ax = *(s16*)bodyPtr;
  ax = (ax*6)+2;
  bodyPtr+=ax; // skip the points data

  ax = *(s16*)bodyPtr; // num of bones
  bx = ax;
  bodyPtr+=bx*2; // skip bones idx table

  if(numOfBonesInAnim > ax)
  {
    numOfBonesInAnim = ax;
  }

  bodyPtr+=10; // skip bone 0

  time = (u16)timer - timeOfKeyframeStart;

  bx = keyframeLength;
  bp = time;

  if(time<keyframeLength) // interpole keyframe
  {
    u8* animVar1Backup = animVar1;
    // skip bone 0 anim
    animVar4 += 8; // anim buffer
    animVar1 += 8; // current keyframe ptr

    if(!(flag&8))
    {
      do
      {
        switch(getAnimType(&bodyPtr))
        {
        case 0:
          {
            processAnimRotation(&bodyPtr,bp,bx);
            processAnimRotation(&bodyPtr,bp,bx);
            processAnimRotation(&bodyPtr,bp,bx);
            break;
          }
        case 1:
        case 2:
          {
            processAnimTranslation(&bodyPtr,bp,bx);
            processAnimTranslation(&bodyPtr,bp,bx);
            processAnimTranslation(&bodyPtr,bp,bx);
            break;
          }
        }

        bodyPtr+=8;
      }
      while(--numOfBonesInAnim);
    }
    else
    {
      do
      {
        switch(getAnimType(&bodyPtr))
        {
        case 0:
          {
            animVar4 += 6;
            animVar1 += 6;
            bodyPtr += 6;
            break;
          }
        case 1:
        case 2:
          {
            processAnimTranslation(&bodyPtr,bp,bx);
            processAnimTranslation(&bodyPtr,bp,bx);
            processAnimTranslation(&bodyPtr,bp,bx);
            break;
          }
        }

        processAnimRotation(&bodyPtr,bp,bx);
        processAnimRotation(&bodyPtr,bp,bx);
        processAnimRotation(&bodyPtr,bp,bx);

        animVar4 += 2;
        animVar1 += 2;
        bodyPtr += 10;

      }
      while(--numOfBonesInAnim);
    }

    animVar1 = animVar1Backup;

    animVar1+=2;

    animRot2 = ((*(s16*)(animVar1))*bp)/bx; // X
    animRot3 = ((*(s16*)(animVar1+2))*bp)/bx; // Y
    animRot1 = ((*(s16*)(animVar1+4))*bp)/bx; // Z

    animVar1+=6;

    animCurrentTime = bx;
    animKeyframeLength = bp;

    return(0);
  }
  else // change keyframe
  {
    u8* tempBx = animVar1;
    u8* si = animVar1;


    si+=8;

    do
    {
      *(s16*)(bodyPtr) = *(s16*)(si);
      *(s16*)(bodyPtr+2) = *(s16*)(si+2);
      *(s16*)(bodyPtr+4) = *(s16*)(si+4);
      *(s16*)(bodyPtr+6) = *(s16*)(si+6);

      bodyPtr+=8;
      si+=8;

      if(flag&8)
      {
        *(s16*)(bodyPtr) = *(s16*)(si);
        *(s16*)(bodyPtr+2) = *(s16*)(si+2);
        *(s16*)(bodyPtr+4) = *(s16*)(si+4);
        *(s16*)(bodyPtr+6) = *(s16*)(si+6);
        bodyPtr+=8;
        si+=8;
      }

      bodyPtr+=8;

    }while(--numOfBonesInAnim);

    *(u8**)animVar3 = animVar1;
    *(u16*)(animVar3+4) = (u16)timer;

    tempBx+=2;

    animCurrentTime = bx;
    animKeyframeLength = bx;

    animRot2 = *(s16*)(tempBx);
    animRot3 = *(s16*)(tempBx+2);
    animRot1 = *(s16*)(tempBx+4);

    tempBx += 6;

    return(1);

  }

}
