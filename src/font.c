#include "common.h"

char* fontVar1 = NULL;
short int fontSm1 = 0;
short int fontSm2 = 0x1234;
char* fontVar4 = NULL;
extern char* fontVar4;
char* fontVar5 = NULL;
extern char* fontVar5;
short int currentFontColor = 0;
extern short int currentFontColor;
short int fontSm4 = 2;
short int fontSm5 = 1;
short int fontSm3 = 18;
short int fontVar6 = 0;
short int fontSm7 = 0x1234;
short int fontSm8 = 0x1234;
short int fontSm9 = 0x80;

unsigned char flagTable[]= {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

void initFont(char* fontData, int color)
{
  short int tempDx;
  short int tempAxFlip;

  fontVar1 = fontData; // fontPtr

  tempDx = READ_LE_S16(fontData); // alignement
  fontData+=2;

  fontSm1 = *(fontData++); // character height
  fontSm2 = *(fontData++); // character size

  if(!fontSm2)
  {
    fontSm2 = READ_LE_S16(fontData);
  }
  
  fontData+=2;

  tempAxFlip = READ_LE_S16(fontData);
  fontData+=2;

  tempAxFlip = ((tempAxFlip & 0xFF) << 8 ) | ((tempAxFlip &0xFF00) >> 8);

  fontVar4 = fontData;

  fontVar5 = fontVar1 + tempAxFlip - (tempDx&0xFF)*2;

  currentFontColor = color;

  fontSm3 = color;
}

void initFont2(int var1, int var2)
{
  fontSm4 = var1;
  fontSm5 = var2;
}

int computeStringWidth(char* string)
{
  int width = 0;
  unsigned char character;

  while((character = ((unsigned char)(*(string++)))))
  {
    char* dataPtr;
    unsigned short int data;

    dataPtr = fontVar5 + character*2;
    data = READ_LE_S16(dataPtr);
  
    data>>=4;

    data &= 0xF;

    if(!data)
    {
      width += fontSm4;
    }

    width += fontSm5;
    width += data;
  }

  return(width);
}

void renderText(int x, int y, char* surface, char* string)
{
  unsigned char character;

  fontVar6 = x;
  fontSm7 = y;

  if(gameId >= AITD2)
  {
    return;
  }

  while((character = *((unsigned char*)(string++))))
  {
    char* dataPtr;
    unsigned short int data;
    unsigned short int dx;

    dataPtr = fontVar5 + character*2;
    data = READ_LE_U16(dataPtr);
  
    data = ((data & 0xFF)<<8)| ((data&0xFF00)>>8);

    dx = data;

    data >>= 12;

    if(data&0xF) // real character (width != 0)
    {
      char* characterPtr;
      int bp;
      int ch;

      dx &= 0xFFF;

      characterPtr = (dx>>3) + fontVar4;

      fontSm9 = flagTable[dx & 7];

      bp = fontSm7;

      fontSm8 = fontVar6;

      ch;

      for(ch = fontSm1; ch>0; ch--)
      {
        char* outPtr = screen + bp*320 + fontSm8;
        

        int dh = fontSm9;
        int cl = data&0xF;

        int al = *characterPtr;

        int bx;

        bp++;

        for(bx = 0; cl>0; cl--)
        {
          if(dh & al)
          {
            *(outPtr) = (char)fontSm3;
          }

          outPtr++;

          dh = ((dh>>1) & 0x7F) | ((dh<<7)&0x80);

          if(dh&0x80)
          {
            bx++;
            al = *(characterPtr + bx);
          }
        }

        characterPtr += fontSm2;
      }

      fontVar6 += data&0xF;
    }
    else // space character
    {
      fontVar6 += fontSm4;
    }

    fontVar6 += fontSm5;
  }
}

void drawSlectedText(int x, int y, int index, int color1, int color2)
{
  textEntryStruct* entryPtr;
  char* textPtr;

  entryPtr = getTextFromIdx(index);

  if(!entryPtr)
    return;

  x -= (entryPtr->width/2); // center

  textPtr = entryPtr->textPtr;

  initFont(fontData,color2);
  renderText(x,y+1,screen,textPtr);

  initFont(fontData,color1);
  renderText(x,y,screen,textPtr);

}

void drawText(int x, int y, int index, int color)
{
  textEntryStruct* entryPtr;
  char* textPtr;

  entryPtr = getTextFromIdx(index);

  if(!entryPtr)
    return;

  x -= (entryPtr->width/2); // center

  textPtr = entryPtr->textPtr;

  initFont(fontData,color);

  renderText(x,y+1,screen,textPtr);
}
