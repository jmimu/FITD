#include "common.h"

int initAnimInBody(int frame, char* anim, char* body)
{
  short int temp;
  short int ax;
  short int cx;
  short int bx;
  char* saveAnim;
  int i;
  int flag;

  flag = (*(short int*)body);

  temp = *(short int*)anim;
  anim+=2;

  if(frame >= temp)
  {
    return(0);
  }

  ax = *(short int*)anim;
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

  animCurrentTime = *(short int*)anim;
  animKeyframeLength = animCurrentTime;

  if(!(flag&2))
  {
    return(0);
  }

  body +=14;

  *(char**)(body+2) = anim;
  *(unsigned short int*)(body+6) = timer;

  body+= *(short int*)body;
  body+=2;

  ax = *(short int*)body;
  bx = ax;

  body+=(((ax << 1) + bx) << 1) +2;

  bx = ax = *(short int*)body;

  body+=bx<<1;

  if(cx > ax)
    cx = ax;

  body+=10;

  saveAnim = anim;

  anim += 8;

  for(i=0;i<cx;i++)
  {
    *(short int*)(body) = *(short int*)(anim);
    body+=2;
    anim+=2;
    *(short int*)(body) = *(short int*)(anim);
    body+=2;
    anim+=2;
    *(short int*)(body) = *(short int*)(anim);
    body+=2;
    anim+=2;
    *(short int*)(body) = *(short int*)(anim);
    body+=2;
    anim+=2;

    if(flag&8)
    {
      *(short int*)(body) = *(short int*)(anim);
      body+=2;
      anim+=2;
      *(short int*)(body) = *(short int*)(anim);
      body+=2;
      anim+=2;
      *(short int*)(body) = *(short int*)(anim);
      body+=2;
      anim+=2;
      *(short int*)(body) = *(short int*)(anim);
      body+=2;
      anim+=2;
    }

    body+=8;
  }

  anim = saveAnim;

  anim+=2;

  animRot2 = *(short int*)anim;
  anim+=2;
  animRot3 = *(short int*)anim;
  anim+=2;
  animRot1 = *(short int*)anim;
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

void initBufferAnim(char* buffer, char* bodyPtr)
{
  int flag = *(short int*)bodyPtr;
  if(flag & 2)
  {
    char* source = bodyPtr+0x10;
    short int ax;
    int cx;
    int i;

    *(unsigned short int*)(source+4) = (unsigned short int)timer;
    *(char**)(source) = buffer;

    source += *(short int*)(source-2);

    ax = *(short int*)(source);

    ax = (((ax * 2) + ax)*2)+2;

    source += ax;

    cx = *(short int*)source;
    
    source += cx*2;

    buffer+= 8;
    source+= 10;

    for(i=0;i<cx;i++)
    {
      *(short int*)(buffer) = *(short int*)(source);
      *(short int*)(buffer+2) = *(short int*)(source+2);
      *(short int*)(buffer+4) = *(short int*)(source+4);
      *(short int*)(buffer+6) = *(short int*)(source+6);

      buffer+=8;
      source+=8;

      if(flag&8)
      {
        *(short int*)(buffer) = *(short int*)(source);
        *(short int*)(buffer+2) = *(short int*)(source+2);
        *(short int*)(buffer+4) = *(short int*)(source+4);
        *(short int*)(buffer+6) = *(short int*)(source+6);

        buffer+=8;
        source+=8;
      }

      source+=8;
    }

  }
}

short int getAnimParam(char* animPtr)
{
  return(*(short int*)animPtr);
}

short int getAnimType(char** bodyPtr) // local
{
  short int temp = *(short int*)animVar1;

  animVar1+=2;

  animVar4+=2;

  *(short int*)(*bodyPtr) = temp;
  (*bodyPtr)+=2;

  return(temp);
}

void processAnimRotation(char** bodyPtr, int bp, int bx) // local
{
  short int oldRotation = *(short int*)animVar4;
  short int newRotation;
  short int diff;
  
  animVar4+=2;

  newRotation = *(short int*)animVar1;
  animVar1+=2;

  diff = newRotation - oldRotation;

  if(diff == 0)
  {
    *(short int*)(*bodyPtr) = newRotation;
  }
  else
  {
    if(diff <= 0x200)
    {
      if(diff >= -0x200)
      {
        *(short int*)(*bodyPtr) = ((diff*bp)/bx) + oldRotation;
      }
      else
      {
        newRotation += 0x400;
        newRotation -= oldRotation;

        *(short int*)(*bodyPtr) = ((newRotation*bp)/bx) + oldRotation;
      }
    }
    else
    {
      oldRotation += 0x400;
      newRotation -= oldRotation;

      *(short int*)(*bodyPtr) = ((newRotation*bp)/bx) + oldRotation;
    }
  }

  (*bodyPtr)+=2;
}

void processAnimTranslation(char** bodyPtr, int bp, int bx) // local
{
  short int cx = *(short int*)animVar4;
  short int ax;
  animVar4+=2;

  ax = *(short int*)animVar1;
  animVar1+=2;

  if(ax == cx)
  {
    *(short int*)(*bodyPtr) = ax;
  }
  else
  {
    *(short int*)(*bodyPtr) = (((ax - cx)*bp)/bx) + cx;
  }

  (*bodyPtr)+=2;
}

short int setInterAnimObjet(int frame, char* animPtr, char* bodyPtr)
{
  int numOfBonesInAnim = *(short int*)(animPtr+2);
  unsigned short int keyframeLength;
  unsigned short int timeOfKeyframeStart;
  char* animBufferPtr;
  int ax;
  unsigned short int bx;
  unsigned short int time;
  int bp;
  int flag;

  flag = *(short int*)bodyPtr;
  
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

  keyframeLength = *(unsigned short int*)animPtr; // keyframe length

  if(!((*(short int*)bodyPtr) & 2)) // do not anim if the model can't be animated
  {
    return(0);
  }

  bodyPtr+=16;

  animVar3 = bodyPtr;

  timeOfKeyframeStart = *(unsigned short int*)(bodyPtr+4); // time of start of keyframe
  
  animBufferPtr = *(char**)(bodyPtr);

  if(!animBufferPtr)
  {
    animBufferPtr = animVar1;
  }

  // animVar4 = ptr to previous key frame
  animVar4 = animBufferPtr;

  bodyPtr+= *(short int*)(bodyPtr-2);

  ax = *(short int*)bodyPtr;
  ax = (ax*6)+2;
  bodyPtr+=ax; // skip the points data

  ax = *(short int*)bodyPtr; // num of bones
  bx = ax;
  bodyPtr+=bx*2; // skip bones idx table

  if(numOfBonesInAnim > ax)
  {
    numOfBonesInAnim = ax;
  }

  bodyPtr+=10; // skip bone 0

  time = (unsigned short int)timer - timeOfKeyframeStart;

  bx = keyframeLength;
  bp = time;

  if(time<keyframeLength) // interpole keyframe
  {
    char* animVar1Backup = animVar1;
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

    animRot2 = ((*(short int*)(animVar1))*bp)/bx; // X
    animRot3 = ((*(short int*)(animVar1+2))*bp)/bx; // Y
    animRot1 = ((*(short int*)(animVar1+4))*bp)/bx; // Z

    animVar1+=6;

    animCurrentTime = bx;
    animKeyframeLength = bp;

    return(0);
  }
  else // change keyframe
  {
    char* tempBx = animVar1;
    char* si = animVar1;


    si+=8;

    do
    {
      *(short int*)(bodyPtr) = *(short int*)(si);
      *(short int*)(bodyPtr+2) = *(short int*)(si+2);
      *(short int*)(bodyPtr+4) = *(short int*)(si+4);
      *(short int*)(bodyPtr+6) = *(short int*)(si+6);

      bodyPtr+=8;
      si+=8;

      if(flag&8)
      {
        *(short int*)(bodyPtr) = *(short int*)(si);
        *(short int*)(bodyPtr+2) = *(short int*)(si+2);
        *(short int*)(bodyPtr+4) = *(short int*)(si+4);
        *(short int*)(bodyPtr+6) = *(short int*)(si+6);
        bodyPtr+=8;
        si+=8;
      }

      bodyPtr+=8;

    }while(--numOfBonesInAnim);

    *(char**)animVar3 = animVar1;

    *(unsigned short int*)(animVar3+4) = (unsigned short int)timer;

    tempBx+=2;

    animCurrentTime = bx;
    animKeyframeLength = bx;

    animRot2 = *(short int*)(tempBx);
    animRot3 = *(short int*)(tempBx+2);
    animRot1 = *(short int*)(tempBx+4);

    tempBx += 6;

    return(1);

  }

}
