#include "common.h"

void drawPartOfAITDBox(int left, int top, int index, char* gfxData)
{
  char* outPtr;
  char* inPtr;

  int width;
  int height;

  int offset;

  int i;
  int j;

  if(gameId != AITD1)
    return;

  outPtr = screen + top*320 + left;
  inPtr = gfxData + READ_LE_U16(index * 2 + gfxData); // alignement unsafe

  inPtr +=4;

  width = READ_LE_U16(inPtr); // alignement unsafe
  inPtr+=2;
  height = READ_LE_U16(inPtr); // alignement unsafe
  inPtr+=2;

  offset = 320 - width;

  for(i=0;i<height;i++)
  {
    for(j=0;j<width;j++)
    {
      *(outPtr++) = *(inPtr++);
    }

    outPtr+=offset;
  }
}

void drawAITDBox(int x, int y, int width, int height)
{
  int top;
  int right;
  int left;
  int bottom;

  int currentLeftPosition;
  int currentTopPosition;

  int halfWidth;
  int halfHeight;

  setClipSize(0,0,319,199);

  halfWidth = width/2;
  currentLeftPosition = left = x - halfWidth;

  halfHeight = height/2;
  currentTopPosition = top = y - halfHeight;

  right = x + halfWidth;
  bottom = y + halfHeight;

  drawPartOfAITDBox(currentLeftPosition,currentTopPosition,0,aitdBoxGfx); // draw top left corner
    
  while(1) // draw top bar
  {
    currentLeftPosition += 20;

    if(right - 20 <= currentLeftPosition)
      break;

    drawPartOfAITDBox(currentLeftPosition,currentTopPosition,4,aitdBoxGfx);
  }

  drawPartOfAITDBox(currentLeftPosition,currentTopPosition,1,aitdBoxGfx); // draw top right corner

  currentLeftPosition = left;

  while(1) // draw left bar
  {
    currentTopPosition += 20;

    if(bottom - 20 <= currentTopPosition)
      break;

    drawPartOfAITDBox(currentLeftPosition,currentTopPosition,6,aitdBoxGfx);
  }

  currentLeftPosition = right - 8;
  currentTopPosition = top + 20;

  while(bottom - 20 > currentTopPosition)
  {
    drawPartOfAITDBox(currentLeftPosition,currentTopPosition,7,aitdBoxGfx);

    currentTopPosition += 20;
  }

  currentLeftPosition = left;

  drawPartOfAITDBox(currentLeftPosition,currentTopPosition,2,aitdBoxGfx); // draw bottom left corner

  while(1) // draw bottom bar
  {
    currentLeftPosition += 20;

    if(right-20 <= currentLeftPosition)
      break;

    drawPartOfAITDBox(currentLeftPosition,currentTopPosition+12,5,aitdBoxGfx);
  }

  drawPartOfAITDBox(currentLeftPosition,currentTopPosition,3,aitdBoxGfx); // draw bottom right corner

  drawPartOfAITDBox(x-20,currentTopPosition+12,8,aitdBoxGfx); // draw "in the dark"

  currentMenuLeft = left + 8;
  currentMenuTop = top + 8;
  currentMenuRight = right - 9;
  currentMenuBottom = bottom - 9;

  fillBox(currentMenuLeft,currentMenuTop,currentMenuRight,currentMenuBottom,0);
  setClipSize(currentMenuLeft,currentMenuTop,currentMenuRight,currentMenuBottom);

}
