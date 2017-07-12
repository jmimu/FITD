#include "common.h"

bool timerSaved = false;
unsigned int timerSavedValue = 0;

void freezeTime(void)
{
  if(!timerSaved)
  {
    timerSavedValue = timeGlobal;
    timerSaved = true;
  }
}

void unfreezeTime(void)
{
  if(timerSaved)
  {
    timeGlobal = timerSavedValue;
    timerSaved = false;
  }
}
