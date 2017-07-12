#include "common.h"

unsigned long int etageVar0Size = 0;
unsigned long int numGlobalCamera = 0;
cameraDataStruct* globalCameraDataTable = NULL;

void loadFloor(int floorNumber)
{
  int i;
  int expectedNumberOfRoom;
  int expectedNumberOfCamera;
  unsigned int cameraDataSize;
  char buffer[256];
  char buffer2[256];

  if(etageVar1)
  {
    free(etageVar1);
    free(etageVar0);
  }

  //stopSounds();

  HQR_Reset(listBody);
  HQR_Reset(listAnim);

  currentEtage = floorNumber;

  if(gameId < AITD3)
  {
    sprintf(buffer,"ETAGE%02d",floorNumber);

    etageVar0Size = getPakSize(buffer,0);
    cameraDataSize = getPakSize(buffer,1);

    etageVar0 = loadPakSafe(buffer,0);
    etageVar1 = loadPakSafe(buffer,1);
  }

  currentCamera = -1;
  needChangeRoom = 1;
  changeFloor = 0;

  //////////////////////////////////

  if(roomDataTable)
  {
    free(roomDataTable);
    roomDataTable = NULL;
  }

  expectedNumberOfRoom = getNumberOfRoom();

  for(i=0;i<expectedNumberOfRoom;i++)
  {
    u32 j;
    u8* roomData;
    u8* hardColData;
    u8* sceZoneData;
    roomDataStruct* currentRoomDataPtr;

    if(roomDataTable)
    {
      roomDataTable = (roomDataStruct*)realloc(roomDataTable,sizeof(roomDataStruct)*(i+1));
    }
    else
    {
      roomDataTable = (roomDataStruct*)malloc(sizeof(roomDataStruct));
    }

    if(gameId >= AITD3)
    {
      char buffer[256];

      if(gameId == AITD3)
      {
        sprintf(buffer,"SAL%02d",floorNumber);
      }
      else
      {
        sprintf(buffer,"ETAGE%02d",floorNumber);
      }

      roomData = loadPakSafe(buffer,i);
    }
    else
    {
      roomData = (etageVar0 + READ_LE_U32(etageVar0 + i * 4));
    }
    currentRoomDataPtr = &roomDataTable[i];

    currentRoomDataPtr->worldX = READ_LE_S16(roomData+4);
    currentRoomDataPtr->worldY = READ_LE_S16(roomData+6);
    currentRoomDataPtr->worldZ = READ_LE_S16(roomData+8);

    currentRoomDataPtr->numCameraInRoom = READ_LE_U16(roomData+0xA);

    currentRoomDataPtr->cameraIdxTable = (u16*)malloc(currentRoomDataPtr->numCameraInRoom*sizeof(s16));

    for(j=0;j<currentRoomDataPtr->numCameraInRoom;j++)
    {
      currentRoomDataPtr->cameraIdxTable[j] = READ_LE_U16(roomData+0xC+2*j);
    }

    // hard col read

    hardColData = roomData + READ_LE_U16(roomData);
    currentRoomDataPtr->numHardCol = READ_LE_U16(hardColData);
    hardColData+=2;

    if(currentRoomDataPtr->numHardCol)
    {
      currentRoomDataPtr->hardColTable = (hardColStruct*)malloc(sizeof(hardColStruct)*currentRoomDataPtr->numHardCol);

      for(j=0;j<currentRoomDataPtr->numHardCol;j++)
      {
        ZVStruct* zvData;

        zvData = &currentRoomDataPtr->hardColTable[j].zv;

        zvData->ZVX1 = READ_LE_S16(hardColData+0x00);
        zvData->ZVX2 = READ_LE_S16(hardColData+0x02);
        zvData->ZVY1 = READ_LE_S16(hardColData+0x04);
        zvData->ZVY2 = READ_LE_S16(hardColData+0x06);
        zvData->ZVZ1 = READ_LE_S16(hardColData+0x08);
        zvData->ZVZ2 = READ_LE_S16(hardColData+0x0A);

        currentRoomDataPtr->hardColTable[j].parameter = READ_LE_U16(hardColData+0x0C);
        currentRoomDataPtr->hardColTable[j].type = READ_LE_U16(hardColData+0x0E);

        hardColData+=0x10;
      }
    }
    else
    {
      currentRoomDataPtr->hardColTable = NULL;
    }

    // sce zone read

    sceZoneData = roomData + READ_LE_U16(roomData+2);
    currentRoomDataPtr->numSceZone = READ_LE_U16(sceZoneData);
    sceZoneData+=2;

    if(currentRoomDataPtr->numSceZone)
    {
      currentRoomDataPtr->sceZoneTable = (sceZoneStruct*)malloc(sizeof(sceZoneStruct)*currentRoomDataPtr->numSceZone);

      for(j=0;j<currentRoomDataPtr->numSceZone;j++)
      {
        ZVStruct* zvData;

        zvData = &currentRoomDataPtr->sceZoneTable[j].zv;

        zvData->ZVX1 = READ_LE_S16(sceZoneData+0x00);
        zvData->ZVX2 = READ_LE_S16(sceZoneData+0x02);
        zvData->ZVY1 = READ_LE_S16(sceZoneData+0x04);
        zvData->ZVY2 = READ_LE_S16(sceZoneData+0x06);
        zvData->ZVZ1 = READ_LE_S16(sceZoneData+0x08);
        zvData->ZVZ2 = READ_LE_S16(sceZoneData+0x0A);

        currentRoomDataPtr->sceZoneTable[j].parameter = READ_LE_U16(sceZoneData+0x0C);
        currentRoomDataPtr->sceZoneTable[j].type = READ_LE_U16(sceZoneData+0x0E);

        sceZoneData+=0x10;
      }
    }
    else
    {
      currentRoomDataPtr->sceZoneTable = NULL;
    }
  }
  ///////////////////////////////////

  /////////////////////////////////////////////////
  // camera stuff

  if(gameId >= AITD3)
  {
    char buffer[256];

    if(gameId == AITD3)
    {
      sprintf(buffer,"CAM%02d",floorNumber);
    }
    else
    {
      sprintf(buffer,"CAMSAL%02d",floorNumber);
    }

    expectedNumberOfCamera = PAK_getNumFiles(buffer);
  }
  else
  {
    expectedNumberOfCamera = ((READ_LE_U32(etageVar1))/4);
  }

  globalCameraDataTable = (cameraDataStruct*)malloc(sizeof(cameraDataStruct)*expectedNumberOfCamera);

  for(i=0;i<expectedNumberOfCamera;i++)
  {
    int k;
    unsigned int offset;
    unsigned char* currentCameraData;

    if(gameId >= AITD3)
    {
      char buffer[256];

      if(gameId == AITD3)
      {
        sprintf(buffer,"CAM%02d",floorNumber);
      }
      else
      {
        sprintf(buffer,"CAMSAL%02d",floorNumber);
      }

      offset = 0;
      cameraDataSize = 1;
      currentCameraData = loadPakSafe(buffer,i);
    }
    else
    {
      offset = READ_LE_U32(etageVar1 + i * 4);
    }

    // load cameras
    if(offset<cameraDataSize)
    {
      unsigned char* backupDataPtr;

      if(gameId<AITD3)
      {
        currentCameraData = (etageVar1 + READ_LE_U32(etageVar1 + i * 4));
      }

      backupDataPtr = currentCameraData;

      globalCameraDataTable[i].alpha = READ_LE_U16(currentCameraData+0x00);
      globalCameraDataTable[i].beta  = READ_LE_U16(currentCameraData+0x02);
      globalCameraDataTable[i].gamma = READ_LE_U16(currentCameraData+0x04);

      globalCameraDataTable[i].x = READ_LE_U16(currentCameraData+0x06);
      globalCameraDataTable[i].y = READ_LE_U16(currentCameraData+0x08);
      globalCameraDataTable[i].z = READ_LE_U16(currentCameraData+0x0A);

      globalCameraDataTable[i].focal1 = READ_LE_U16(currentCameraData+0x0C);
      globalCameraDataTable[i].focal2 = READ_LE_U16(currentCameraData+0x0E);
      globalCameraDataTable[i].focal3 = READ_LE_U16(currentCameraData+0x10);

      globalCameraDataTable[i].numCameraZoneDef = READ_LE_U16(currentCameraData+0x12);

      currentCameraData+=0x14;

      globalCameraDataTable[i].cameraZoneDefTable = (cameraZoneDefStruct*)malloc(sizeof(cameraZoneDefStruct)*globalCameraDataTable[i].numCameraZoneDef);

      ASSERT(globalCameraDataTable[i].cameraZoneDefTable);

      for(k=0;k<globalCameraDataTable[i].numCameraZoneDef;k++)
      {
        cameraZoneDefStruct* pCurrentCameraZoneDefEntry;

        pCurrentCameraZoneDefEntry = &globalCameraDataTable[i].cameraZoneDefTable[k];

        pCurrentCameraZoneDefEntry->dummy1 = READ_LE_U16(currentCameraData+0x00);
        pCurrentCameraZoneDefEntry->dummy2 = READ_LE_U16(currentCameraData+0x02);
        pCurrentCameraZoneDefEntry->dummy3 = READ_LE_U16(currentCameraData+0x04);
        pCurrentCameraZoneDefEntry->dummy4 = READ_LE_U16(currentCameraData+0x06);
        pCurrentCameraZoneDefEntry->dummy5 = READ_LE_U16(currentCameraData+0x08);
        pCurrentCameraZoneDefEntry->dummy6 = READ_LE_U16(currentCameraData+0x0A);

        if(gameId != AITD1)
        {
          pCurrentCameraZoneDefEntry->dummy7 = READ_LE_U16(currentCameraData+0x0C);
          pCurrentCameraZoneDefEntry->dummy8 = READ_LE_U16(currentCameraData+0x0E);
       }

        // load camera zone
        {
          unsigned char* pZoneData;
          int numZones;
          int j;

          pZoneData = backupDataPtr + globalCameraDataTable[i].cameraZoneDefTable[k].dummy3;
          //pZoneData = currentCameraData;

          pCurrentCameraZoneDefEntry->numZones = numZones =READ_LE_U16(pZoneData);
          pZoneData+=2;

          pCurrentCameraZoneDefEntry->cameraZoneEntryTable = (cameraZoneEntryStruct*)malloc(sizeof(cameraZoneEntryStruct)*numZones);

          ASSERT(pCurrentCameraZoneDefEntry->cameraZoneEntryTable);

          for(j=0;j<pCurrentCameraZoneDefEntry->numZones;j++)
          {
            int pointIdx;
            int numPoints;

            pCurrentCameraZoneDefEntry->cameraZoneEntryTable[j].numPoints = numPoints = READ_LE_U16(pZoneData);
            pZoneData+=2;

            pCurrentCameraZoneDefEntry->cameraZoneEntryTable[j].pointTable = (cameraZonePointStruct*)malloc(sizeof(cameraZonePointStruct)*(numPoints+1));

            for(pointIdx = 0; pointIdx < pCurrentCameraZoneDefEntry->cameraZoneEntryTable[j].numPoints; pointIdx++)
            {
              pCurrentCameraZoneDefEntry->cameraZoneEntryTable[j].pointTable[pointIdx].x = READ_LE_U16(pZoneData);
              pZoneData+=2;
              pCurrentCameraZoneDefEntry->cameraZoneEntryTable[j].pointTable[pointIdx].y = READ_LE_U16(pZoneData);
              pZoneData+=2;
            }

            pCurrentCameraZoneDefEntry->cameraZoneEntryTable[j].pointTable[numPoints].x = pCurrentCameraZoneDefEntry->cameraZoneEntryTable[j].pointTable[0].x; // copy first point to last position
            pCurrentCameraZoneDefEntry->cameraZoneEntryTable[j].pointTable[numPoints].y = pCurrentCameraZoneDefEntry->cameraZoneEntryTable[j].pointTable[0].y;
          }
        }

        if(gameId == AITD1)
          currentCameraData+=0x0C;
        else
          currentCameraData+=0x10;

        if(gameId == TIMEGATE)
        {
          currentCameraData+=4;
        }
      }
    }
    else
    {
      break;
    }
  }

  numGlobalCamera = i-1;

 // globalCameraDataTable = (cameraDataStruct*)realloc(globalCameraDataTable,sizeof(cameraDataStruct)*numGlobalCamera);

/*    roomCameraData+=0x14;

  }*/
}
