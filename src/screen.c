// seg 048

#include "common.h"

void setupScreen(void)
{
  screen = (char*)malloc(64800);

  screenBufferSize = 64800;

  unkScreenVar2 = 3;

  // TODO: remain of screen init

}

void flipScreen()
{
  int i;
  char paletteRGBA[256*4];
  char* outPtr = scaledScreen;
  char* inPtr = unkScreenVar;
#ifdef USE_GL
  osystem_flip(NULL);
  return;
#endif

  memcpy(unkScreenVar,screen,320*200);

  for(i=0;i<256;i++)
  {
    paletteRGBA[i*4] = palette[i*3];
    paletteRGBA[i*4+1] = palette[i*3+1];
    paletteRGBA[i*4+2] = palette[i*3+2];
    paletteRGBA[i*4+3] = -1;
  }

  outPtr = scaledScreen;
  inPtr = unkScreenVar;

  for(i=0;i<200;i++)
  {
    int j;
    char* copySource = outPtr;

    for(j=0;j<320;j++)
    {
      *(outPtr++) = *(inPtr);
      *(outPtr++) = *(inPtr++);
    }

    // copy line
    for(j=0;j<640;j++)
    {
      *(outPtr++) = *(copySource++);
    }
    
  }

  //osystem_setPalette(paletteRGBA);
  osystem_flip((unsigned char*)scaledScreen);
}

void flushScreen(void)
{
  int i;
  int j;

  for(i=0;i<200;i++)
  {
    for(j=0;j<320;j++)
    {
      *(screen+i*320+j) = 0;
    }
  }
}
