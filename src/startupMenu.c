#include "common.h"

void drawStartupMenu(int selectedEntry)
{
  int currentY = 76;
  int currentTextNum = 0;

  drawAITDBox(160,100,320,80);

  while(currentTextNum<3)
  {
    if(currentTextNum == selectedEntry) // hilight selected entry
    {
      fillBox(10,currentY,309,currentY+16,100);
      drawSlectedText(160,currentY,currentTextNum+11,15,4);
    }
    else
    {
      drawText(160,currentY,currentTextNum+11,4);
    }

    currentY+=16; // next line
    currentTextNum++; // next text
  }
}

int processStartupMenu(void)
{
  int currentSelectedEntry = 0;
  unsigned int chrono;
  int selectedEntry = -1;

  flushScreen();

  drawStartupMenu(0);
#ifdef USE_GL
  osystem_startFrame();
  osystem_stopFrame();
  osystem_CopyBlockPhys((unsigned char*)screen,0,0,320,200);
#endif
  flipScreen();
  make3dTatouUnk1(16,0);
  startChrono(&chrono);

  while(evalChrono(&chrono) <= 0x10000) // exit loop only if time out or if choice made
  {
#ifdef USE_GL
  osystem_CopyBlockPhys((unsigned char*)screen,0,0,320,200);
  osystem_startFrame();
#endif

    if(selectedEntry!=-1 || evalChrono(&chrono) > 0x10000)
    {
      break;
    }

    process_events();
    readKeyboard();

    if(inputKey&1) // up key
    {
      currentSelectedEntry--;

      if(currentSelectedEntry<0)
      {
        currentSelectedEntry = 2;
      }

      drawStartupMenu(currentSelectedEntry);
      flipScreen();
//      menuWaitVSync();

      startChrono(&chrono);

      while(inputKey)
      {
        readKeyboard();
      }
    }

    
    if(inputKey&2) // down key
    {
      currentSelectedEntry++;

      if(currentSelectedEntry>2)
      {
        currentSelectedEntry = 0;
      }

      drawStartupMenu(currentSelectedEntry);
      //menuWaitVSync();
      flipScreen();

      startChrono(&chrono);

      while(inputKey)
      {
        readKeyboard();
      }
    } 

    if(input2 == 28 || (input2 != 28 && input1!=0)) // select current entry
    {
      selectedEntry = currentSelectedEntry;
    }
#ifdef USE_GL
    osystem_stopFrame();
    flipScreen();
#endif
  }

  if(selectedEntry==2) // if exit game, do not fade
  {
    fadeOut(16,0);
  }

  readKeyboard();
  while(inputKey)
  {
    readKeyboard();
  }

  return(selectedEntry);
}
