#include "common.h"

void printString(int index, int textIndex, int selectedIndex)
{
  int topPosition;

  topPosition = (((currentMenuBottom - currentMenuTop) / 2) + currentMenuTop) + (index << 4) - 56;

  if(index == selectedIndex)
  {
    drawSlectedText(0xA0,topPosition,textIndex,0xF,4);
  }
  else
  {
    drawText(0xA0,topPosition,textIndex,4);
  }
}

void drawSystemMenuLayout(int selectedStringNumber)
{
  int backupTop;
  int backupBottom;
  int backupLeft;
  int backupRight;

  drawAITDBox(160,100,320,200);

  backupTop = currentMenuTop;
  backupBottom = currentMenuBottom;
  backupLeft = currentMenuLeft;
  backupRight = currentMenuRight;

  drawAITDBox(80,55,120,70);

  //scaleDownImage(40,35,aux2);

  currentMenuTop = backupTop;
  currentMenuBottom = backupBottom;
  currentMenuLeft = backupLeft;
  currentMenuRight = backupRight;

  setClipSize(currentMenuLeft,currentMenuTop,currentMenuRight,currentMenuBottom);

  printString(0,48,selectedStringNumber);
  printString(1,45,selectedStringNumber);
  printString(2,46,selectedStringNumber);
  printString(3,41+musicEnabled,selectedStringNumber);
  printString(4,43/*+soundToggle*/,selectedStringNumber);
  printString(5,49/*+detailToggle*/,selectedStringNumber);
  printString(6,47,selectedStringNumber);

  menuWaitVSync();
}

void processSystemMenu(void)
{
  int entry = -1;
  int exitMenu = 0;
  int currentSelectedEntry;

  freezeTime();
  //pauseShaking();

  if(lightVar1)
  {
    //makeBlackPalette();
  }

  //clearScreenSystemMenu(unkScreenVar,aux2);

  currentSelectedEntry = 0;

  while(!exitMenu)
  {
    readKeyboard();
    flushScreen();

    drawSystemMenuLayout(currentSelectedEntry);
    osystem_CopyBlockPhys(screen,0,0,320,200);
    osystem_startFrame();

    if(lightVar1)
    {
      make3dTatouUnk1(0x40,0);
    }

  //  while(!exitMenu)
    {
      input3 = input2;
      button = input1;
      input4 = inputKey;

      if(!input5)
      {
        if(input3 == 0x1C || button) // enter
        {
          switch(currentSelectedEntry)
          {
          case 0: // exit menu
            exitMenu = 1;
            break;
          case 1: // save
            makeSave(45);
            break;
          case 2: // load
            if(restoreSave(46,1))
            {
              mainVar1 = 2;
              unfreezeTime();
              //updateShaking();
              return;
            }
            break;
            
          }
        }
        else
        {
          if(input3 == 0x1B)
          {
            exitMenu = 1;
          }
          if(input4 == 1) // up
          {
            currentSelectedEntry--;

            if(currentSelectedEntry<0)
              currentSelectedEntry = 6;

            input5 = 1;
          }
          if(input4 == 2) // bottom
          {
            currentSelectedEntry++;

            if(currentSelectedEntry>6)
              currentSelectedEntry = 0;

            input5 = 1;
          }
        }
      }
      else
      {
        if(!input3 && !input4)
        {
          input5 = 0;
        }
      }
    }

    flipScreen();
  }
}