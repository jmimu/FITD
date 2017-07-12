#ifndef _ROOM_H_
#define _ROOM_H_

struct hardColStruct;
typedef struct hardColStruct hardColStruct;

#include "vars.h" // temporary fix to cross include

struct hardColStruct
{
  ZVStruct zv;
  u32 type;
  u32 parameter;
};

struct sceZoneStruct
{
  ZVStruct zv;
  u32 type;
  u32 parameter;
};

typedef struct sceZoneStruct sceZoneStruct;

struct cameraZonePointStruct
{
  s16 x;
  s16 y;
};

typedef struct cameraZonePointStruct cameraZonePointStruct;

struct cameraZoneEntryStruct
{
  u16 numPoints;

  cameraZonePointStruct* pointTable;
};

typedef struct cameraZoneEntryStruct cameraZoneEntryStruct;

struct cameraZoneDefStruct
{
  s16 dummy1;
  s16 dummy2;
  s16 dummy3;
  s16 dummy4;
  s16 dummy5;
  s16 dummy6;
  s16 dummy7;
  s16 dummy8;

  u16 numZones;
  cameraZoneEntryStruct* cameraZoneEntryTable;
};

typedef struct cameraZoneDefStruct cameraZoneDefStruct;

struct cameraDataStruct
{
  s16 alpha;
  s16 beta;
  s16 gamma;

  s16 x;
  s16 y;
  s16 z;

  s16 focal1;
  s16 focal2;
  s16 focal3;

  u16 numCameraZoneDef;

  cameraZoneDefStruct* cameraZoneDefTable;
};
typedef struct cameraDataStruct cameraDataStruct;

struct roomDataStruct
{
  u32 numCameraInRoom;

  u32 numHardCol;
  hardColStruct* hardColTable;

  u32 numSceZone;
  sceZoneStruct* sceZoneTable;

  s32 worldX;
  s32 worldY;
  s32 worldZ;

  u16* cameraIdxTable;
};
typedef struct roomDataStruct roomDataStruct;

extern cameraDataStruct* cameraDataTable[NUM_MAX_CAMERA_IN_ROOM];
extern cameraZoneDefStruct* currentCameraZoneList[NUM_MAX_CAMERA_IN_ROOM];
extern roomDataStruct* roomDataTable;

roomDefStruct* getRoomData(int roomNumber);
void loadRoom(int roomNumber);
int getNumberOfRoom();

#endif
