#include "common.h"

#include "SDL.h"
#include "SDL_thread.h"

int musicThread(void *dummy);

int threadTimer(void *test) // AITD time is 50 frames per seconds ie,  a frame every 20 miliseconds
{
    int temp = SDL_GetTicks();
    while (1)
  {
      SDL_Delay(2); // granularity restriction
      if(SDL_GetTicks() - temp >= 15)
        {
         // timeGlobal++;
            temp = SDL_GetTicks();
        }
  }
    return (0);
}

void startThreadTimer()
{
 //   SDL_CreateThread(threadTimer, NULL);
 //   SDL_CreateThread(musicThread, NULL);
}
