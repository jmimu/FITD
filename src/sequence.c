#include "common.h"

void convertPaletteIfRequired(unsigned char* lpalette)
{
  if(gameId >= JACK && gameId < AITD3)
  {
    int i;
    unsigned char* ptr2 = lpalette;
    for(i=0;i<256;i++)
    {
      int j;
      for(j=0;j<3;j++)
      {
        unsigned int composante = *(ptr2);
        composante*=255;
        composante/=63;
        *(ptr2++) = composante&0xFF;
      }
    }
  }
}

char* sequenceListAITD2[]=
{
  "BATL",
  "GRAP",
  "CLE1",
  "CLE2",
  "COOK",
  "EXPL",
  "FALA",
  "FAL2",
  "GLIS",
  "GREN",
  "JEND",
  "MANI",
  "MER",
  "TORD",
  "PANT",
  "VERE",
  "PL21",
  "PL22",
  "ENDX",
  "SORT",
  "EFER",
  "STAR",
  "MEDU",
  "PROL",
  "GRAS",
  "STRI",
  "ITRO",
  "BILL",
  "PIRA",
  "PIR2",
  "VENT",
  "FIN",
  "LAST"
};

void unapckSequenceFrame(unsigned char* source,unsigned char* dest)
{
  unsigned char byteCode;

  byteCode = *(source++);

  while(byteCode)
  {
    if(!(--byteCode)) // change pixel or skip pixel
    {
      unsigned char changeColor;

      changeColor = *(source++);

      if(changeColor)
      {
        *(dest++) = changeColor;
      }
      else
      {
        dest++;
      }
    }
    else
    if(!(--byteCode)) // change 2 pixels or skip 2 pixels
    {
      unsigned char changeColor;

      changeColor = *(source++);

      if(changeColor)
      {
        *(dest++) = changeColor;
        *(dest++) = changeColor;
      }
      else
      {
        dest+=2;
      }
    }
    else
    if(!(--byteCode)) // fill or skip
    {
      unsigned char size;
      unsigned char fillColor;

      size = *(source++);
      fillColor = *(source++);

      if(fillColor)
      {
        int i;

        for(i=0;i<size;i++)
        {
          *(dest++) = fillColor;
        }
      }
      else
      {
        dest+=size;
      }
    }
    else // large fill of skip
    {
      unsigned short int size;
      unsigned char fillColor;

      size = *(unsigned short int*)source;
      source+=2;
      fillColor = *(source++);

      if(fillColor)
      {
        int i;

        for(i=0;i<size;i++)
        {
          *(dest++) = fillColor;
        }
      }
      else
      {
        dest+=size;
      }
    }

    byteCode = *(source++);
  }
}

void playSequence(int sequenceIdx, int fadeStart, int fadeOutVar)
{
#define SPEED 70              /* Ticks per Frame */
#define SLEEP_MIN 20          /* Minimum time a sleep takes, usually 2*GRAN */
#define SLEEP_GRAN 1         /* Granularity of sleep */

int frames=0;                   /* Number of frames displayed */
s32 t_start,t_left;
u32 t_end;
s32 q=0; 

  int var_4 = 1;
  int var_6 = 0;
  int var_8 = 1;
  unsigned char localPalette[0x300];

  while(!var_6)
  {
    int si = 0;
    int sequenceParamIdx;

    while(si < var_8)
    {
      char buffer[256];
      frames++;
      t_start=SDL_GetTicks();

      timeGlobal++;

      timer = timeGlobal;

      if(gameId == AITD2)
      {
        strcpy(buffer,sequenceListAITD2[sequenceIdx]);
      }
      if(gameId == AITD3)
      {
        sprintf(buffer,"AN%d",sequenceIdx);
      }

      if(!loadPakToPtr(buffer,si,screen))
      {
        theEnd(0,buffer);
      }

      if(!si) // first frame
      {
        memcpy(localPalette,screen,0x300); // copy palette
        memcpy(aux,screen+0x300,64000);
        var_8 = *(unsigned short int*)(screen+64768);

        if(gameId < AITD3)
          convertPaletteIfRequired(localPalette);

        if(var_4 != 0)
        {
    /*      if(fadeStart & 1)
          {
            fadeOut(0x10,0);
          }
          if(fadeStart & 4)
          {
             //memset(palette,0,0); // hu ?
            fadeInSub1(localPalette);
            flipOtherPalette(palette);
          } */

          osystem_setPalette(localPalette);
          copyPalette(localPalette,palette);
        }
      }
      else // not first frame
      {
        unsigned long int frameSize;

        frameSize = *(unsigned long int*)screen;

        if(frameSize < 64000) // key frame
        {
          unapckSequenceFrame(screen+4,aux);
        }
        else // delta frame
        {
          copyToScreen(screen,aux);
        }
      }

      for(sequenceParamIdx = 0; sequenceParamIdx < numSequenceParam; sequenceParamIdx++)
      {
        if(sequenceParams[sequenceParamIdx].frame == si)
        {
          playSound(sequenceParams[sequenceParamIdx].sample);
        }
      }

      // TODO: here, timming management
      // TODO: fade management

      osystem_CopyBlockPhys(aux,0,0,320,200);
      osystem_startFrame();
      flipScreen();

      si++;

      process_events();
      readKeyboard();

      t_end=t_start+SPEED;
      t_left=t_start-SDL_GetTicks()+SPEED;

      if(t_left>0){
          if(t_left>SLEEP_MIN)
              SDL_Delay(t_left-SLEEP_GRAN);
          while(SDL_GetTicks()<t_end){ q++; };
      }
    }

    fadeOutVar--;

    if(fadeOutVar==0)
    {
      var_6=1;
    }
  }
}