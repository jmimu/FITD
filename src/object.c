#include "common.h"

int copyObjectToActor(int flag2, int var1, int foundName, short int flag, int x, int y, int z, int stage, int room, int alpha, int beta, int gamma, int var2, int var3, int var4, int var5)
{
  int i;
  int j;
  actorStruct* actorPtr = actorTable;
  char* bodyPtr;
  ZVStruct* zvPtr;

  for(i=0;i<NUM_MAX_ACTOR;i++)
  {
    if(actorPtr->field_0 == -1)
    break;

    actorPtr++;
  }

  if(i==NUM_MAX_ACTOR)
    return -1;

  currentProcessedActorPtr = actorPtr;
  currentProcessedActorIdx = i;

  actorPtr->bodyNum = flag2;
  actorPtr->flags = flag;
  actorPtr->stage = stage;
  actorPtr->room = room;
  actorPtr->worldX = actorPtr->roomX = x;
  actorPtr->worldY = actorPtr->roomY = y;
  actorPtr->worldZ = actorPtr->roomZ = z;

  if(room != currentDisplayedRoom)
  {
    actorPtr->worldX -= (s16)((roomDataTable[currentDisplayedRoom].worldX - roomDataTable[actorPtr->room].worldX) * 10);
    actorPtr->worldY += (s16)((roomDataTable[currentDisplayedRoom].worldY - roomDataTable[actorPtr->room].worldY) * 10);
    actorPtr->worldZ += (s16)((roomDataTable[currentDisplayedRoom].worldZ - roomDataTable[actorPtr->room].worldZ) * 10);
  }

  actorPtr->alpha = alpha;
  actorPtr->beta = beta;
  actorPtr->gamma = gamma;

  actorPtr->dynFlags = 1;

  actorPtr->ANIM = var2;
  actorPtr->FRAME = var3;

  actorPtr->field_40 = var4;
  actorPtr->field_42 = var5;

  actorPtr->END_FRAME = 1;
  actorPtr->END_ANIM = 1;
  actorPtr->field_44 = -1;
  actorPtr->field_46 = 0;
  actorPtr->field_48 = -1;

  actorPtr->modX = 0;
  actorPtr->modY = 0;
  actorPtr->modZ = 0;

  for(j=0;j<3;j++)
  {
    actorPtr->COL[j] = -1;
  }

  actorPtr->COL_BY = -1;
  actorPtr->HARD_DEC = -1;
  actorPtr->HARD_COL = -1;

  if(gameId != AITD1)
  {
    actorPtr->hardMat = -1;
  }

  actorPtr->rotate.oldAngle = 0;
  actorPtr->rotate.newAngle = 0;
  actorPtr->rotate.param = 0;

  actorPtr->field_60.oldAngle = 0;
  actorPtr->field_60.newAngle = 0;
  actorPtr->field_60.param = 0;

  actorPtr->falling = 0;

  actorPtr->field_72 = 0;

  actorPtr->speed = 0;

  actorPtr->trackMode = 0;
  actorPtr->trackNumber = -1;

  actorPtr->animActionType = 0;
  actorPtr->HIT = -1;
  actorPtr->HIT_BY = -1;

  if(flag2 != -1)
  {
    bodyPtr = HQR_Get(listBody,actorPtr->bodyNum);

    if(var2 != -1)
    {
      char* animPtr = HQR_Get(listAnim,var2);

      initAnimInBody(var3,animPtr,bodyPtr);

      actorPtr->field_4C = getAnimParam(animPtr);
      actorPtr->END_ANIM = 0;
      actorPtr->flags |= 1;

      //			computeScreenBox(actorPtr->field_22 + actorPtr->field_5A, actorPtr->field_24 + actorPtr->field_5C, actorPtr->anim + actorPtr->field_5E, actorPtr->alpha, actorPtr->beta, actorPtr->gamma, bodyPtr);

      if(BBox3D1<0)
        BBox3D1 = 0;

      if(BBox3D3>319)
        BBox3D3 = 319;

      if(BBox3D2<0)
        BBox3D2 = 0;

      if(BBox3D4>199)
        BBox3D4 = 199;

      actorPtr->field_14 = BBox3D1;
      actorPtr->field_16 = BBox3D2;
      actorPtr->field_18 = BBox3D3;
      actorPtr->field_1A = BBox3D4;
    }
    else
    {
      if(!(actorPtr->flags & 4))
      {
        actorPtr->flags &= 0xFFFE;
      }
    }
  }

  startChrono(&actorPtr->ROOM_CHRONO);
  startChrono(&actorPtr->CHRONO);

  zvPtr = &actorPtr->zv;

  switch(var1)
  {
    case 0:
    {
      if(flag2!=-1)
      {
        getZvMax(bodyPtr,zvPtr);
      }
      else
      {
        makeDefaultZV(zvPtr);
      }
      break;
    }
    case 1:
    {
      if(flag2!=-1)
      {
        getZvNormal(bodyPtr,zvPtr);
      }
      else
      {
        makeDefaultZV(zvPtr);
      }
      break;
    }
    case 2:
    {
      if(flag2!=-1)
      {
        getZvCube(bodyPtr,zvPtr);
      }
      else
      {
        makeDefaultZV(zvPtr);
      }
      break;
    }
    case 3:
    {
      if(flag2!=-1)
      {
        getZvRot(bodyPtr,zvPtr,alpha,beta,gamma);
      }
      else
      {
        makeDefaultZV(zvPtr);
      }
      break;
    }
    case 4:
    {
      if(gameId < AITD3)
      {
        char* roomDataPtr = (char*)getRoomData(room);
        int numElements;
        int j;

        roomDataPtr += *(short int*)roomDataPtr;

        numElements = *(short int*)roomDataPtr;
        roomDataPtr+=2;

        for(j=0;j<numElements;j++)
        {
          if(*(short int*)(roomDataPtr+0xE) == 9)
          {
            if(*(short int*)(roomDataPtr+0xC) == foundName)
            {
              int tempX;
              int tempY;
              int tempZ;

              zvPtr->ZVX1 = READ_LE_S16(roomDataPtr+0x00);
              zvPtr->ZVX2 = READ_LE_S16(roomDataPtr+0x02);
              zvPtr->ZVY1 = READ_LE_S16(roomDataPtr+0x04);
              zvPtr->ZVY2 = READ_LE_S16(roomDataPtr+0x06);
              zvPtr->ZVZ1 = READ_LE_S16(roomDataPtr+0x08);
              zvPtr->ZVZ2 = READ_LE_S16(roomDataPtr+0x0A);

              x = 0;
              y = 0;
              z = 0;

              tempX = ((*(short int*)roomDataPtr) + (*(short int*)(roomDataPtr+2)))/2;
              actorPtr->worldX = tempX;
              actorPtr->roomX = tempX;

              tempY = ((*(short int*)(roomDataPtr+4)) + (*(short int*)(roomDataPtr+6)))/2;
              actorPtr->worldY = tempY;
              actorPtr->roomY = tempY;

              tempZ = ((*(short int*)(roomDataPtr+8)) + (*(short int*)(roomDataPtr+0xA)))/2;
              actorPtr->worldZ = tempZ;
              actorPtr->roomZ = tempZ;

              if(room != currentDisplayedRoom)
              {
                actorPtr->worldX = (roomDataTable[currentDisplayedRoom].worldX - roomDataTable[room].worldX) * 10;
                actorPtr->worldY = (roomDataTable[currentDisplayedRoom].worldY - roomDataTable[room].worldY) * 10;
                actorPtr->worldZ = (roomDataTable[currentDisplayedRoom].worldZ - roomDataTable[room].worldZ) * 10;
              }

              break;
            }
          }

          roomDataPtr+=0x10;
        }

        if(j==numElements)
        {
          makeDefaultZV(zvPtr);
        }
      }

      break;
    }
    default:
    {
      printf("Unsupported ZV type in copyObjectToActor\n");
      exit(1);
      break;
    }
  }

  zvPtr->ZVX1 += x;
  zvPtr->ZVX2 += x;

  zvPtr->ZVY1 += y;
  zvPtr->ZVY2 += y;

  zvPtr->ZVZ1 += z;
  zvPtr->ZVZ2 += z;

  return(i);
}
