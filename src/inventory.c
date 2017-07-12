#include "common.h"

int statusLeft;
int statusTop;
int statusRight;
int statusBottom;

int numInventoryActions;
short int inventoryActionTable[5];

int drawTopStatusBox(int arg_0, int arg_2, int arg_4)
{
  int var_A = currentMenuTop+1;
  int var_6 = arg_0;
  int var_8;
  int i;
  int currentObj;
  objectStruct* objPtr;

  drawAITDBox(160,50,320,100);

  var_A = currentMenuTop+1;

  for(i=0;i<5;i++)
  {
    if(arg_0>=numObjInInventory)
      break;

    currentObj = inventory[arg_0];

    objPtr = &objectTable[currentObj];

    if(arg_0 == arg_2)
    {
      if(arg_4 == 15)
      {
        fillBox(0xA,var_A,0x135,var_A+0x10,0x64);
      }

      drawSlectedText(160,var_A,objPtr->foundName,arg_4,4);

      var_8 = currentObj;
    }
    else
    {
      drawText(160,var_A,objPtr->foundName,4);
    }

    var_A += 0x10;
    arg_0++;
  }

  printTextSub5(298,0x10,0x10,aitdBoxGfx);

  if(var_6+5 < numObjInInventory)
  {
    printTextSub5(298,74,9,aitdBoxGfx);
  }

  return(var_8);
}

void renderInventoryObject(int arg)
{
  setClipSize(statusLeft,statusTop,statusRight,statusBottom);
  fillBox(statusLeft,statusTop,statusRight,statusBottom,0);

  statusVar1 -= 8;

  rotateModel(0,0,0,60,statusVar1,0,24000);
  renderModel(0,0,0,0,0,0,currentFoundBody);

  if(arg!=-1)
  {
    char buffer[256];
    initFont(fontData,4);
    sprintf(buffer,"%d",vars[arg]);
    renderText(statusLeft+4,statusTop+4,screen,buffer);
  }

  menuWaitVSync();
}

void drawInventoryActions(int arg)
{
  int var_2;
  int i;
  
  drawAITDBox(240,150,160,100);

  var_2 = 150 - ((numInventoryActions<<4)/2);

  for(i=0;i<numInventoryActions;i++)
  {
    if(arg == i)
    {
      fillBox(170,var_2,309,var_2+16,100);
      drawSlectedText(240,var_2,inventoryActionTable[i],15,4);
    }
    else
    {
      drawText(240,var_2,inventoryActionTable[i],4);
    }

    var_2 += 16;
  }
}

void makeStatusScreen(void)
{
  int exitMenu = 0;
  int var_10 = 0;
  int firstTime = 1;
  unsigned int chrono;
  int var_14;
  int var_16;
  int var_18;
  int var_1A;
  int var_1C;
  int var_8;
  int var_A;
  int var_C;
  int var_E;

  if(!numObjInInventory)
    return;

  var_1C = 0;
  var_14 = -1;
  var_18 = 0;
  var_A = 0;
  var_8 = 2;

  statusVar1 = 0;

  freezeTime();
  //pauseShaking();

  if(lightVar1!=0)
  {
    //makeBlackPalette();
  }

  drawAITDBox(80,150,160,100);

  statusLeft = currentMenuLeft;
  statusTop = currentMenuTop;
  statusRight = currentMenuRight;
  statusBottom = currentMenuBottom;

  setupSMCode(((statusRight-statusLeft)/2)+statusLeft,((statusBottom-statusTop)/2) + statusTop,128,400,390);

  while(!exitMenu)
  {
#ifdef USE_GL
    osystem_CopyBlockPhys((unsigned char*)screen,0,0,320,200);
    osystem_startFrame();
    osystem_cleanScreenKeepZBuffer();
#endif

    readKeyboard();
    process_events();

    input3 = input2;
    input4 = inputKey;
    button = input1;

    if(!input3 && !input4 && !button)
    {
      var_8 = 0;
    }

    if(input3 == 1)
    {
      var_10 = 0;
      exitMenu = 1;
    }

    if(var_A == 0)
    {
      if(var_8<1)
      {
        if((input3 == 0x1C) || (button != 0) || (input4 == 0xC))
        {
          drawTopStatusBox(var_1C,var_18,14);
          menuWaitVSync();
          var_A = 1;
          var_14 = -1;
          var_1A = 0;

          while(input2)
          {
            readKeyboard();
          }
        }
        else
        {
          if(input4&1 && var_18>0)
          {   
            var_18--;
          }

          if(input4&2 && var_18 < (numObjInInventory-1))
          {
            var_18++;
          }

          if(var_1C+5 <= var_18)
          {
            var_1C++;
          }

          if(var_18 < var_1C)
          {
            var_1C--;
          }

          if(input3 || input4 || button)
          {
            if(var_8==0)
            {
              var_8 = 1;
              startChrono(&chrono);
            }
          }
        }
      }
      else
      {
        if(var_8 == 1)
        {
          if(evalChrono(&chrono)>0x280000)
          {
            var_8 = -1;
          }
        }
      }

      if(var_14!=var_18)
      {
        var_16 = drawTopStatusBox(var_1C,var_18,15);

        currentFoundBodyIdx = objectTable[var_16].foundBody;

        currentFoundBody = HQR_Get(listBody,currentFoundBodyIdx);

        var_C = objectTable[var_16].flags2;

        numInventoryActions = 0;
        var_E = 0;

        while(var_E<11)
        {
          if(var_C & (1<<var_E))
          {
            if(numInventoryActions<5)
            {
              inventoryActionTable[numInventoryActions++] = var_E+23;
            }
          }
          var_E ++;
        }

        drawInventoryActions(-1);
        flipScreen();

        var_14 = var_18;
      }
    }
    else
    {
      if(var_8<1)
      {
        if(input3 == 0x1C || button)
        {
          var_18 = inventory[var_18];
          action = (1 << (233 + inventoryActionTable[var_1A]));
          var_10 = 1;
          exitMenu = 1;
        }

        if(input4 & 0xC)
        {
          drawInventoryActions(-1);
          var_A = 0;
          var_14 = -1;
          var_8 = 2;
          continue;
        }

        if(input4&1 && var_1A>0)
        {
          var_1A --;
        }

        if(input4&2 && var_1A<(numInventoryActions-1))
        {
          var_1A++;
        }

        if(input3 || input4 || button)
        {
          if(var_8==0)
          {
            var_8 = 1;
            startChrono(&chrono);
          }
        }
      }
      else
      {
        if(var_8==1)
        {
          if(evalChrono(&chrono)>0x280000)
          {
            var_8 = -1;
          }
        }

        if(var_14 != var_1A)
        {
          var_14 = var_1A;
          drawInventoryActions(var_14);
          menuWaitVSync();
        }
      }
    }
    renderInventoryObject(objectTable[var_16].field_24);

    if(firstTime)
    {
      firstTime = 0;
      if(lightVar1)
      {
        make3dTatouUnk1(0x40,0);
      }
    }

    flipScreen();
  }

  unfreezeTime();

  mainVar1 = 1;

  readKeyboard();
  while(input1 || input2 || inputKey)
  {
    readKeyboard();
  }

  input4 = 0;
  input3 = 0;
  button = 0;

  if(var_10 == 1)
  {
    updateInHand(var_18);
  }

  //updateShaking();
}
