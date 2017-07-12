#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef _WIN32
#include "config.h"
#endif

#ifdef MACOSX
#define UNIX
#endif

#define NUM_MAX_CAMERA_IN_ROOM 20

#define HAS_YM3812 1

#ifndef USE_GL
#define USE_GL
#endif

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u32;

typedef signed char s8;
typedef signed short int s16;
typedef signed long int s32;

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#ifdef WIN32
#include <search.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <assert.h>
#endif

#ifdef _DEBUG
#define ASSERT(exp) assert(exp)
#else
#define ASSERT(exp)
#endif

#ifdef _DEBUG
#define ASSERT_PTR(exp) assert(exp)
#else
#define ASSERT_PTR(exp)
#endif

#ifdef USE_GL
#include <GL/gl.h>      // Header File For The OpenGL32 Library
#include <GL/glu.h>     // Header File For The GLu32 Library
//#include <gl\glaux.h>   // Header File For The Glaux Library
#endif

#include "SDL.h"

//////////////// GAME SPECIFIC DEFINES

//#define NUM_MAX_OBJ         300
#define NUM_MAX_ACTOR       50
#define NUM_MAX_TEXT        40
#define NUM_MAX_MESSAGE     5
#define INVENTORY_SIZE      30


// 250
#define NUM_MAX_TEXT_ENTRY  1000

//////////////////

enum enumCVars
{
  SAMPLE_PAGE,
  BODY_FLAMME,
  MAX_WEIGHT_LOADABLE,
	TEXTE_CREDITS,
	SAMPLE_TONNERRE,
	INTRO_DETECTIVE,
	INTRO_HERITIERE,
	WORLD_NUM_PERSO,
	CHOOSE_PERSO,
	SAMPLE_CHOC,
	SAMPLE_PLOUF,
	REVERSE_OBJECT,
	KILLED_SORCERER,
	LIGHT_OBJECT,
	FOG_FLAG,
	DEAD_PERSO,
  JET_SARBACANE,
  TIR_CANON,
  JET_SCALPEL,
  POIVRE,
  DORTOIR,
  EXT_JACK,
  NUM_MATRICE_PROTECT_1,
  NUM_MATRICE_PROTECT_2,
  NUM_PERSO,
  TYPE_INVENTAIRE,
  PROLOGUE,
  POIGNARD,
  MATRICE_FORME,
  MATRICE_COULEUR,

  UNKNOWN_CVAR // for table padding, shouldn't be called !
};

typedef enum enumCVars enumCVars;

extern enumCVars AITD1KnownCVars[];
extern enumCVars AITD2KnownCVars[];

extern enumCVars* currentCVarTable;

int getCVarsIdx(enumCVars);

//////////////////////

#define	SAMPLE_PAGE				0
#define	BODY_FLAMME				1
#define	MAX_WEIGHT_LOADABLE		2
#define	TEXTE_CREDITS			3
#define	SAMPLE_TONNERRE			4
#define	INTRO_DETECTIVE			5
#define	INTRO_HERITIERE			6
#define	WORLD_NUM_PERSO			7
#define	CHOOSE_PERSO			8
#define	SAMPLE_CHOC				9
#define	SAMPLE_PLOUF			10
#define	REVERSE_OBJECT			11
#define	KILLED_SORCERER			12
#define	LIGHT_OBJECT			13
#define	FOG_FLAG				14
#define	DEAD_PERSO				15


//////////////////

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
#ifndef UNIX
typedef unsigned int uint;
#endif
typedef signed char int8;
typedef signed short int16;
typedef signed long int32;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long U32;
typedef signed char S8;
typedef signed short S16;
typedef signed long S32;

#define TYPE_MASK 0x1D1

#define ANIM_ONCE             0
#define ANIM_REPEAT           1
#define ANIM_UNINTERRUPTABLE  2
#define ANIM_RESET            4

// temp triangulation stuff
//#include <bool.h>
//#include <geometry.h>
int triangulate_polygon(int ncontours,int cntr[],double (*vertices)[2],int (*triangles)[3]);
#include "room.h"
#include "vars.h"
#include "main.h"
#include "fileAccess.h"
#include "screen.h"
#include "videoMode.h"
#include "pak.h"
#include "unpack.h"
#include "tatou.h"
#include "threadCode.h"
#include "renderer.h"
#include "input.h"
#include "version.h"
#include "cosTable.h"
#include "hqr.h"
#include "time.h"
#include "font.h"
#include "aitdBox.h"
#include "save.h"
#include "anim.h"
#include "animAction.h"
#include "actorList.h"
#include "mainLoop.h"
#include "inventory.h"
#include "startupMenu.h"
#include "systemMenu.h"
#include "floor.h"
#include "object.h"
#include "zv.h"
#include "music.h"
#include "fmopl.h"

// debugger
#ifdef INTERNAL_DEBUGGER
#include "debugger.h"
#endif

// scripting
#include "track.h"
#include "life.h"
#include "life_2.h"
#include "evalVar.h"

#include "osystem.h"


////

//typedef unsigned char byte;

#ifdef UNIX
#define FORCEINLINE static inline
#else
#ifdef WIN32
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE inline
#endif
#endif

FORCEINLINE u16 READ_LE_U16(void *ptr)
{
#ifdef MACOSX
  return (((u8*)ptr)[1]<<8)|((u8*)ptr)[0];
#else
  return *(u16*)ptr;
#endif
}

FORCEINLINE s16 READ_LE_S16(void *ptr)
{
  return (s16)READ_LE_U16(ptr);
}

FORCEINLINE u16 READ_BE_U16(void *ptr)
{
#ifdef MACOSX
  return *(u16*)ptr;
#else
  return (((u8*)ptr)[1]<<8)|((u8*)ptr)[0];
#endif
}

FORCEINLINE s16 READ_BE_S16(void *ptr)
{
  return (s16)READ_BE_S16(ptr);
}

FORCEINLINE u32 READ_LE_U32(void *ptr)
{
#ifdef MACOSX
  return (((u8*)ptr)[3]<<24)|(((u8*)ptr)[2]<<16)|(((u8*)ptr)[1]<<8)|((u8*)ptr)[0];
#else
  return *(u32*)ptr;
#endif
}

FORCEINLINE s32 READ_LE_S32(void *ptr)
{
  return (s32)READ_LE_U32(ptr);
}

FORCEINLINE u32 READ_BE_U32(void *ptr)
{
#ifdef MACOSX
  return *(u32*)ptr;
#else
  return (((u8*)ptr)[3]<<24)|(((u8*)ptr)[2]<<16)|(((u8*)ptr)[1]<<8)|((u8*)ptr)[0];
#endif
}

FORCEINLINE s32 READ_BE_S32(void *ptr)
{
  return (s32)READ_LE_U32(ptr);
}

#endif
