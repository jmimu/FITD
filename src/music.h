#ifndef _MUSIC_H_
#define _MUSIC_H_

extern int nextMusic;
extern unsigned int musicChrono;
extern short int currentMusic;

int initMusicDriver(void);
int musicUpdate(void *udata, u8 *stream, int len);
void playMusic(int musicNumber);
void callMusicUpdate(void);
void destroyMusicDriver(void);
int fadeMusic(int param1, int param2, int param3);
#endif
