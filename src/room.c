#include "common.h"

/*
Room data:

u16 offsetToCameraCoverZones
u16 offsetToHardCol
s16 roomPositionX
s16 roomPositionY
s16 roomPositionZ

etageVar1 -> table for camera data

*/

roomDataStruct* roomDataTable = NULL;
cameraDataStruct* cameraDataTable[NUM_MAX_CAMERA_IN_ROOM];
cameraZoneDefStruct* currentCameraZoneList[NUM_MAX_CAMERA_IN_ROOM];

roomDefStruct* getRoomData(int roomNumber)
{
  return (roomDefStruct*)(etageVar0 + READ_LE_U32(etageVar0 + roomNumber * 4));
}

int getNumberOfRoom()
{
  int i;
  int j = 0;

  if(gameId >= AITD3)
  {
    char buffer[256];

    if(gameId == AITD3)
    {
      sprintf(buffer,"SAL%02d",currentEtage);
    }
    else
    {
      sprintf(buffer,"ETAGE%02d",currentEtage);
    }

    return PAK_getNumFiles(buffer);
  }
  else
  {

    int numMax = (((READ_LE_U32(etageVar0))/4));

    for(i=0;i<numMax;i++)
    {
      if(etageVar0Size >= READ_LE_U32(etageVar0 + i * 4))
      {
        j++;
      }
      else
      {
        return j;
      }
    }
  }
  return j;
}

void loadRoom(int roomNumber)
{
  int i;
  int cameraVar0;
  int cameraVar1;
  int cameraVar2;
  int oldCameraIdx;
  roomDefStruct* roomDataPtr;
  int var_1A = 0;
  int var_10 = -1;

  freezeTime();

  ASSERT(roomNumber >=0);

  if(currentCamera == -1)
  {
    oldCameraIdx = -1;
  }
  else
  {
    cameraVar0 = roomDataTable[currentDisplayedRoom].worldX;
    cameraVar1 = roomDataTable[currentDisplayedRoom].worldY;
    cameraVar2 = roomDataTable[currentDisplayedRoom].worldZ;

    oldCameraIdx = roomDataTable[currentDisplayedRoom].cameraIdxTable[currentCamera];
  }

  if(gameId < AITD3)
  {
    cameraPtr = (char*)getRoomData(roomNumber); // TODO: obsolete
    roomDataPtr = getRoomData(roomNumber);
    pCurrentRoomData = getRoomData(roomNumber);
  }

  currentDisplayedRoom = roomNumber;

  numCameraInRoom = roomDataTable[roomNumber].numCameraInRoom;

  ASSERT(numCameraInRoom < NUM_MAX_CAMERA_IN_ROOM);

/*
  var_20 = cameraPtr + roomDataPtr->offsetToPosDef;
  numCameraZone = *(short int*)var_20;
  var_20 += 2;
  cameraZoneData = var_20;

  var_20 = cameraPtr + roomDataPtr->offsetToCameraDef;
  numRoomZone = *(short int*)var_20;
  var_20 += 2;
  roomZoneData = var_20;*/
 
  ASSERT(numCameraInRoom < NUM_MAX_CAMERA_IN_ROOM);

  for(i=0;i<numCameraInRoom;i++) // build all the camera list
  {
    unsigned int currentCameraIdx;
    unsigned int j;

    currentCameraIdx = roomDataTable[currentDisplayedRoom].cameraIdxTable[i]; // indexes are between the roomDefStruct and the first zone data

    ASSERT(currentCameraIdx<=numGlobalCamera);

    if(oldCameraIdx == currentCameraIdx)
    {
      var_1A = i;
      var_10 = currentCameraIdx;
    }

    if(gameId < AITD3)
    {
      roomVar5[i] = etageVar1 + READ_LE_U32(etageVar1 + currentCameraIdx * 4);
    }

    cameraDataTable[i] = &(globalCameraDataTable[currentCameraIdx]);

    currentCameraIdx = cameraDataTable[i]->numCameraZoneDef;

    // scan for the zone data related to the current room
    for(j=0;j<currentCameraIdx;j++)
    {
      if(cameraDataTable[i]->cameraZoneDefTable[j].dummy1 == currentDisplayedRoom)
        break;
    }

    ASSERT(cameraDataTable[i]->cameraZoneDefTable[j].dummy1 == currentDisplayedRoom);

    currentCameraZoneList[i] = &cameraDataTable[i]->cameraZoneDefTable[j];
  }

  if(oldCameraIdx != -1) // if a camera was selected before loading room
  {
    int var_E = (roomDataTable[roomNumber].worldX - cameraVar0) * 10;
    int var_C = (roomDataTable[roomNumber].worldY - cameraVar1) * 10;
    int var_A = (roomDataTable[roomNumber].worldZ - cameraVar2) * 10;

    for(i=0;i<NUM_MAX_ACTOR;i++)
    {
      if(actorTable[i].field_0 != -1)
      {
 /*       if(gameId == AITD1) // special case. In AITD1, the load room function was always triggered just after the actor was moved in the new room.
                            // it is not always the case in later games. Maybe we could generalize the AITD2 way...
        {
          actorTable[i].worldX -= var_E;
          actorTable[i].worldY += var_C;
          actorTable[i].worldZ += var_A;
        }
        else*/
        {
          if(i!=genVar9)
          {
            actorTable[i].worldX -= var_E;
            actorTable[i].worldY += var_C;
            actorTable[i].worldZ += var_A;
          }
          else
          {
            actorTable[i].worldX = actorTable[i].roomX;
            actorTable[i].worldY = actorTable[i].roomY;
            actorTable[i].worldZ = actorTable[i].roomZ;
          }
        }
      }
    }
  }

  startGameVar1 = var_1A;
  mainVar1 = 1;
  needChangeRoom = 0;
  unfreezeTime();
}
