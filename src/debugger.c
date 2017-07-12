#include "common.h"

#ifdef INTERNAL_DEBUGGER

typedef struct
{
  u32 X;
  u32 Y;
  u8 name[40];
  u8* returnVar;
} debugger_buttonEntry;

bool debuggerVar_debugMenuDisplayed = false;

////// debug var used in engine
bool debuggerVar_drawModelZv = false;
bool debuggerVar_drawCameraCoverZone = false;
bool debuggerVar_noHardClip = false;
bool debuggerVar_topCamera = false;
long int debufferVar_topCameraZoom = -4000;

bool debuggerVar_useBlackBG = false;
///////////////////////////////

u8 debugger_mainDebugButtonVar_toggleDrawModelZv = 0;
u8 debugger_mainDebugButtonVar_toggleDrawCameraCoverZone = 0;

const debugger_buttonEntry debugger_mainDebugButtonTable[]=
{
  {0,0,"Draw model ZV",&debugger_mainDebugButtonVar_toggleDrawModelZv},
  {0,8,"Draw camera cover zone",&debugger_mainDebugButtonVar_toggleDrawCameraCoverZone},
  {0,0,"",NULL},
};

const debugger_buttonEntry* debugger_currentMenu;

void debugger_processDebugMenu(void)
{
}

void debugger_enterMainDebug(void)
{
  debuggerVar_debugMenuDisplayed = true;

  debugger_currentMenu = debugger_mainDebugButtonTable;
}

void debugger_drawMenuEntry(const debugger_buttonEntry* menuEntry)
{
  osystem_drawDebugText(menuEntry->X,menuEntry->Y,menuEntry->name);
}

void debugger_draw(void)
{
  if(debuggerVar_debugMenuDisplayed)
  {
    u32 numEntryInMenu=0;
    u32 currentMenuEntry;

    while(debugger_currentMenu[numEntryInMenu].name[0] != 0)
    {
      numEntryInMenu++;
    }

    for(currentMenuEntry=0; currentMenuEntry<numEntryInMenu; currentMenuEntry++)
    {
      debugger_drawMenuEntry(&debugger_currentMenu[currentMenuEntry]);
    }
  }
}

#endif
