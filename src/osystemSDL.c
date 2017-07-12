#include "common.h"

#ifndef USE_GL

/***************************************************************************
                          mainSDL.cpp  -  description
                             -------------------
    begin                : Mon Jun 3 2002
    copyright            : (C) 2002 by Yaz0r
    email                : yaz0r@yaz0r.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "SDL.h"
#include "SDL_thread.h"
//#include "SDL_sound.h"
#include "osystem.h"

int osystem_mouseRight;
int osystem_mouseLeft;

char *tempBuffer;
SDL_Surface *sdl_buffer;
SDL_Surface *sdl_buffer320x200;
SDL_Surface *sdl_buffer640x400;
SDL_Surface *sdl_bufferStretch;
SDL_Surface *sdl_bufferRGBA;
SDL_Surface *sdl_screen;  // that's the SDL global object for the screen
SDL_Color sdl_colors[256];
SDL_Surface *surfaceTable[16];
char RGBA_Pal[256*4];
//TTF_Font *font;

void osystem_delay(int time)
{
    SDL_Delay(time);
}

void osystem_updateImage()
{
}

/*void OSystem::getMouseStatus(mouseStatusStruct * mouseData)
{

    SDL_GetMouseState(&mouseData->X, &mouseData->Y);

    mouseData->left = mouseLeft;
    mouseData->right = mouseRight;

    mouseLeft = 0;
    mouseRight = 0;
}*/

void osystem_init()  // that's the constructor of the system dependent
            // object used for the SDL port
{
    unsigned char *keyboard;
    int size;

    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
  }

    atexit(SDL_Quit);

//  Sound_Init();

//  atexit(Sound_Quit);

/*    if (TTF_Init() < 0)
  {
      fprintf(stderr, "Couldn't initialize TTF: %s\n", SDL_GetError());
      exit(1);
  }
    atexit(TTF_Quit);

    int rendersolid = 0;
    int renderstyle = 0;
    int rendertype = 0;

    int ptsize = 11;

    font = TTF_OpenFont("verdana.ttf", ptsize);

    if (font == NULL)
  {
      fprintf(stderr, "Couldn't load %d pt font from %s: %s\n", ptsize, "verdana.ttf",
        SDL_GetError());
      exit(2);
  }

    TTF_SetFontStyle(font, renderstyle);*/

    SDL_WM_SetCaption("Alone in the dark", "AITD");

   // SDL_ShowCursor (SDL_DISABLE);

   // SDL_EnableUNICODE (SDL_ENABLE); // not much used in fact

    SDL_PumpEvents();

    keyboard = SDL_GetKeyState(&size);

    keyboard[SDLK_RETURN] = 0;

    sdl_screen = SDL_SetVideoMode(640, 400, 32, SDL_SWSURFACE/*|SDL_FULLSCREEN*/);

    if (sdl_screen == NULL)
  {
      fprintf(stderr, "Couldn't set 640x400x32 video mode: %s\n", SDL_GetError());
      exit(1);
  }

    osystem_mouseLeft = 0;
    osystem_mouseRight = 0;
}

void osystem_putpixel(int x, int y, int pixel)
{
    int bpp = sdl_screen->format->BytesPerPixel;

   /*
    * Here p is the address to the pixel we want to set 
    */
    Uint8 *p = (Uint8 *) sdl_screen->pixels + y * sdl_screen->pitch + x * bpp;

    *p = pixel;
}

void osystem_setPalette(byte * palette)
{
  SDL_Color *sdl_colorsTemp;
  memcpy(RGBA_Pal,palette,256*4);
   // int i;
    sdl_colorsTemp = (SDL_Color *) palette;

    SDL_SetColors(sdl_buffer, sdl_colorsTemp, 0, 256);

    SDL_BlitSurface(sdl_buffer, NULL, sdl_screen, NULL);

    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void osystem_getPalette(char* palette)
{
  memcpy(palette,RGBA_Pal,256*4);
}

void osystem_setPalette320x200(byte * palette)
{
   // int i;
    SDL_Color *sdl_colorsTemp = (SDL_Color *) palette;

    SDL_SetColors(sdl_buffer320x200, sdl_colorsTemp, 0, 256);

//    SDL_BlitSurface(sdl_buffer320x200, NULL, sdl_screen, NULL);

  //  SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void osystem_fadeBlackToWhite()
{
    int i;

    SDL_Color colorPtr[256];

    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);

    for (i = 0; i < 256; i += 3)
  {
      memset(colorPtr, i, 1024);
      SDL_SetPalette(sdl_screen, SDL_PHYSPAL, colorPtr, 0, 256);
  }
}

void osystem_flip(unsigned char *videoBuffer)
{
    SDL_BlitSurface(sdl_buffer, NULL, sdl_screen, NULL);

    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void osystem_draw320x200BufferToScreen(unsigned char *videoBuffer)
{
  SDL_BlitSurface(sdl_buffer320x200,NULL,sdl_bufferRGBA,NULL);

//  sdl_bufferStretch=zoomSurface(sdl_bufferRGBA, 2, 2.4, SMOOTHING_ON);

//  SDL_FillRect(sdl_screen,NULL,0);

    SDL_BlitSurface(sdl_bufferStretch, NULL, sdl_screen, NULL);

    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);

  SDL_FreeSurface(sdl_bufferStretch);
}

void osystem_CopyBlockPhys(unsigned char *videoBuffer, int left, int top, int right, int bottom)
{
    SDL_Rect rectangle;

    rectangle.x = left;
    rectangle.y = top;
    rectangle.w = right - left +1 ;
    rectangle.h = bottom - top +1 ;

    SDL_BlitSurface(sdl_buffer, &rectangle, sdl_screen, &rectangle);

    SDL_UpdateRect(sdl_screen, left, top, right - left +1, bottom - top+1);
}

void osystem_initBuffer(char *buffer, int width, int height)
{   
  sdl_buffer = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8, width, 0, 0, 0, 0);
}

void osystem_crossFade(char *buffer, char *palette)
{
    SDL_Surface *backupSurface;
    SDL_Surface *newSurface;
    SDL_Surface *tempSurface;
    Uint32 rmask, gmask, bmask, amask;
  int i;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    backupSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 32, rmask, gmask, bmask, 0);
    newSurface =
  SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 640, 480, 32, rmask, gmask, bmask, 0);

    tempSurface = SDL_CreateRGBSurfaceFrom(buffer, 640, 480, 8, 640, 0, 0, 0, 0);
    SDL_SetColors(tempSurface, (SDL_Color *) palette, 0, 256);

    SDL_BlitSurface(sdl_screen, NULL, backupSurface, NULL);
    SDL_BlitSurface(tempSurface, NULL, newSurface, NULL);

#ifndef FASTDEBUG

/*    for (i = 0; i < 16; i++)
  {
      SDL_BlitSurface(backupSurface, NULL, surfaceTable[i], NULL);
      SDL_SetAlpha(newSurface, SDL_SRCALPHA | SDL_RLEACCEL, i * 16);
      SDL_BlitSurface(newSurface, NULL, surfaceTable[i], NULL);
  }

    for (i = 0; i < 16; i++)
  {
      SDL_BlitSurface(surfaceTable[i], NULL, sdl_screen, NULL);
      SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
  }*/

    for (i = 0; i < 8; i++)
  {
      SDL_BlitSurface(backupSurface, NULL, surfaceTable[i], NULL);
      SDL_SetAlpha(newSurface, SDL_SRCALPHA | SDL_RLEACCEL, i * 32);
      SDL_BlitSurface(newSurface, NULL, surfaceTable[i], NULL);
        SDL_BlitSurface(surfaceTable[i], NULL, sdl_screen, NULL);
      SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
        SDL_Delay(20);

  }

#endif

    SDL_BlitSurface(newSurface, NULL, sdl_screen, NULL);
    SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);

    SDL_FreeSurface(backupSurface);
    SDL_FreeSurface(newSurface);
    SDL_FreeSurface(tempSurface);
}

int posInStream = 0;
volatile bool deviceStatus = false;

void my_audio_callback(void *userdata, Uint8 *stream, int len)
{
/*  Sound_Sample *sample = (Sound_Sample *)userdata;
  Uint8* input = (Uint8*)sample->buffer;

  if(posInStream+len < sample->buffer_size)
  {
    memcpy(stream,input+posInStream,len);
    posInStream+=len;
  }
  else
  {
    len = sample->buffer_size - posInStream;
    memcpy(stream,input+posInStream,len);
    posInStream+=len;
  }*/
}

void osystem_playSample(char* sampleName)
{
/*  Sound_Sample *sample;
  Sound_AudioInfo info;

  return;
#ifdef UNIX
  return;
#endif
  info.channels = 0;
  info.format = 0;
  info.rate = 0;

  sample = Sound_NewSampleFromFile(sampleName,&info,5000);
  Sound_DecodeAll(sample);

  if(deviceStatus)
  {
    SDL_CloseAudio();
  }

  posInStream = 0;
  {
    SDL_AudioSpec *desired, *obtained;
    SDL_AudioSpec *hardware_spec;*/

    /* Allocate a desired SDL_AudioSpec */
    //desired = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));

    /* Allocate space for the obtained SDL_AudioSpec */
    //obtained = (SDL_AudioSpec*)malloc(sizeof(SDL_AudioSpec));

    /* 22050Hz - FM Radio quality */
    //desired->freq=sample->actual.rate;

    /* 16-bit signed audio */
    //desired->format=sample->actual.format;

    /* Mono */
    //desired->channels=sample->actual.channels;

    /* Large audio buffer reduces risk of dropouts but increases response time */
    //desired->samples=512;

    /* Our callback function */
    //desired->callback=my_audio_callback;

    //desired->userdata=(void*)sample;

    /* Open the audio device */
    /*if ( SDL_OpenAudio(desired, obtained) < 0 ){
    fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
    exit(-1);
    }
    /* desired spec is no longer needed */
    /*free(desired);
    hardware_spec=obtained;
    /* Start playing */
    /*SDL_PauseAudio(0);
    deviceStatus = true;
  }*/
}

void osystem_startBgPoly()
{
}

void osystem_endBgPoly()
{
}

void osystem_addBgPolyPoint(int x, int y)
{
}


#endif
