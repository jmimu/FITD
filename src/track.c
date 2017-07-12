#include "common.h"
#include "math.h"
// From LBA:
int DoTrackVar1;
int GetAngle(int X1, int Y1, int X2, int Y2)
{
  int newX;
  int newY;
  int ebp;
  int edi;
  int eax;
  int tempExchange;
  int esi;
  int ebx;
  int flag;
  int difX;
  int difY;
  short int* tab3 = &cosTable[384];
  short int* tab2 = &cosTable[256];

  int destVal;
  int startAngle;
  int stopAngle;
  int finalAngle;


  difY = edi = Y2 - Y1;
  newY = edi * edi;

  difX = ebp = X2 - X1;
  newX = ebp * ebp;

  if (newX < newY)    // exchange X and Y
  {
    tempExchange = difX;
    difX = difY;
    difY = tempExchange;

    flag = 1;
  }
  else
  {
    flag = 0;
  }

  DoTrackVar1 = (int) sqrt(newX + newY);

  if (!DoTrackVar1)
    return (0);

  destVal = (difY << 14) / DoTrackVar1;

  startAngle = 0;
  stopAngle = 0x100;

  while (tab3[startAngle] > destVal)
  {
    startAngle++;
     /*
      * if (startAngle > 256) { printf ("Can't find angle %d...\n", destVal); exit (1); } 
      */
  }

  if (tab3[startAngle] != destVal)
  if ((tab3[startAngle - 1] + tab3[startAngle]) / 2 <= destVal)
  {
    startAngle--;
  }

  finalAngle = 128 + startAngle;

  if (difX <= 0)    // if we are going left
  {
    finalAngle = -finalAngle;
  }

  if (flag & 1)   // X and Y are exchanged -> 90° rotation needed
  {
    finalAngle = -finalAngle + 0x100;
  }

  return (finalAngle & 0x3FF);

 /*
  do
  {
    currentAngle=(startAngle+stopAngle)/2;

    if(destVal>tab3[currentAngle])
    {
      stopAngle=currentAngle;
    }
    else
    {
      startAngle=currentAngle;
      if(destVal==tab3[currentAngle])
      {
        goto endCalc;
      }
      currentAngle=stopAngle;
    }
  }while(--currentAngle);
  */

  esi = (int) tab3;
  edi = esi + 0x200;

  do
  {
    ebx = esi;
    ebx += edi;
    ebx >>= 1;

    if (eax > READ_LE_S16((void*)ebx))
    {
      edi = ebx;
    }
    else
    {
      esi = ebx;
      if (eax == READ_LE_S16((void*)ebx))
      {
        goto endCalc;
      }
      ebx = edi;
    }
    ebx -= esi;
  }while (--ebx > 2);

  if ((READ_LE_S16((void*)esi) + READ_LE_S16((void*)edi)) / 2 <= eax)
  {
    esi = edi;
  }

  endCalc:

  esi -= (int) tab2;
  eax = esi;
  eax >>= 1;

  if (ebp <= 0)
  {
    eax = -eax;
  }

  if (ebp & 1)    // (newX < newY) ie plus loin sur Y que sur X 
  {
    eax = -eax;
    eax += 0x100;
  }

  return (eax & 0x3FF);
}

int makeProportional(int x1, int x2, int y1, int y2)
{
  return x1 + ((x2 - x1) * y2) / y1;
}

int computeAngleModificatorToPositionSub1(int ax)
{
  int xOut;
  int yOut;

  makeRotationMtx(ax,0,1000,&xOut,&yOut);

  yOut *= angleCompZ;
  xOut *= angleCompX;

  yOut -= xOut;

  if(yOut==0)
    return(0);

  if(yOut>0)
    return(1);
  else
    return(-1);
}

int computeAngleModificatorToPosition(int x1,int z1, int beta, int x2, int z2)
{
  int resultMin;
  int resultMax;
  
  angleCompX = x2 - x1;
  angleCompZ = z2 - z1;
  angleCompBeta = beta;

  resultMin = computeAngleModificatorToPositionSub1(beta - 4);
  resultMax = computeAngleModificatorToPositionSub1(beta + 4);

  if(resultMax == -1 && resultMin == 1) // in the middle
  {
    return(computeAngleModificatorToPositionSub1(beta));
  }
  else
  {
    return(((resultMax+resultMin)+1)>>1);
  }
}

void manualRot(int param)
{
  if(input4&4)
  {
    if(currentProcessedActorPtr->field_72!=1)
    {
      currentProcessedActorPtr->rotate.param = 0;
    }

    currentProcessedActorPtr->field_72 = 1;

    if(currentProcessedActorPtr->rotate.param == 0)
    {
      int oldBeta = currentProcessedActorPtr->beta;

      if(currentProcessedActorPtr->speed != 0)
      {
        startActorRotation(oldBeta,oldBeta+0x100,param/2,&currentProcessedActorPtr->rotate);
      }
      else
      {
        startActorRotation(oldBeta,oldBeta+0x100,param,&currentProcessedActorPtr->rotate);
      }
    }

    currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
  }
  if(input4&8)
  {
    if(currentProcessedActorPtr->field_72!=-1)
    {
      currentProcessedActorPtr->rotate.param = 0;
    }

    currentProcessedActorPtr->field_72 = -1;

    if(currentProcessedActorPtr->rotate.param == 0)
    {
      int oldBeta = currentProcessedActorPtr->beta;

      if(currentProcessedActorPtr->speed != 0)
      {
        startActorRotation(oldBeta,oldBeta-0x100,param/2,&currentProcessedActorPtr->rotate);
      }
      else
      {
        startActorRotation(oldBeta,oldBeta-0x100,param,&currentProcessedActorPtr->rotate);
      }
    }

    currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
  }
  if(!(input4&0xC))
  {
    currentProcessedActorPtr->field_72 = 0;
    currentProcessedActorPtr->rotate.param = 0;
  }
}

#define DISTANCE_TO_POINT_TRESSHOLD 400

unsigned int lastTimeForward = 0;

char* getRoomLink(unsigned int room1, unsigned int room2)
{
  int i;
  short int numOfZones;
  char* zoneData = (char*)getRoomData(room1);  
  char* bestZone;

  zoneData += *(short int*)(zoneData);
  numOfZones = *(short int*)zoneData;
  zoneData+=2;

  bestZone = zoneData;

  for(i=0;i<numOfZones;i++)
  {
    if(*(short int*)(zoneData+14) == 4)
    {
      bestZone = zoneData;

      if(*(short int*)(zoneData+12) == room2)
      {
        return bestZone;
      }
    }

    zoneData += 16;
  }

  return bestZone;
}

void processTrack(void)
{
  switch(currentProcessedActorPtr->trackMode)
  {
  case 1: // manual
    {
      manualRot(60);
      if(input4&1) // forward
      {
        if(timer - lastTimeForward < 10 && currentProcessedActorPtr->speed != 4)
          currentProcessedActorPtr->speed = 5;
        else
        if(currentProcessedActorPtr->speed == 0 || currentProcessedActorPtr->speed == -1)
          currentProcessedActorPtr->speed = 4;

/*        if(currentProcessedActorPtr->speed>0 && currentProcessedActorPtr->speed<4)
          currentProcessedActorPtr->speed = 5; */


        lastTimeForward = timer;
      }
      else
      {
        if(currentProcessedActorPtr->speed>0 && currentProcessedActorPtr->speed<=4)
        {
          currentProcessedActorPtr->speed--;
        }
        else
        {
          currentProcessedActorPtr->speed = 0;
        }
      }

      if(input4&2) // backward
      {
        if(currentProcessedActorPtr->speed == 0 || currentProcessedActorPtr->speed >= 4)
          currentProcessedActorPtr->speed = -1;

        if(currentProcessedActorPtr->speed == 5)
          currentProcessedActorPtr->speed = 0;
      }

      break;
    }
  case 2: // follow
    {
      int followedActorIdx = objectTable[currentProcessedActorPtr->trackNumber].ownerIdx;

      if(followedActorIdx == -1)
      {
        currentProcessedActorPtr->field_72 = 0;
        currentProcessedActorPtr->speed = 0;
      }
      else
      {
        actorStruct* followedActorPtr = &actorTable[followedActorIdx];

        int roomNumber = followedActorPtr->room;
        int x = followedActorPtr->roomX;
        int y = followedActorPtr->roomY;
        int z = followedActorPtr->roomZ;
        int angleModif;

        if(currentProcessedActorPtr->room != roomNumber)
        {
          char* link = getRoomLink(currentProcessedActorPtr->room,roomNumber);

          x = *(short int*)(link)+(((*(short int*)(link+2))-(*(short int *)(link))) / 2);
          y = *(short int*)(link+4)+(((*(short int*)(link+6))-(*(short int *)(link+4))) / 2);
          z = *(short int*)(link+8)+(((*(short int*)(link+10))-(*(short int *)(link+8))) / 2);
        }

        angleModif = computeAngleModificatorToPosition( currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                                  currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                                  currentProcessedActorPtr->beta, x, z );

        if( currentProcessedActorPtr->rotate.param == 0 || currentProcessedActorPtr->field_72 != angleModif )
        {
          startActorRotation( currentProcessedActorPtr->beta, currentProcessedActorPtr->beta - (angleModif << 8), 60, &currentProcessedActorPtr->rotate);
        }

        currentProcessedActorPtr->field_72 = angleModif;

        if( currentProcessedActorPtr->field_72 == 0 )
        {
          currentProcessedActorPtr->rotate.param = 0;
        }
        else
        {
          currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
        }

        currentProcessedActorPtr->speed = 4;

      }
      break;
    }
  case 3: // track
    {
      char* trackPtr = HQR_Get(listTrack,currentProcessedActorPtr->trackNumber);
      short int trackMacro;
      
      trackPtr+=currentProcessedActorPtr->positionInTrack * 2;

      trackMacro = *(short int*)trackPtr;
      trackPtr += 2;

      //printf("Track macro %X\n",trackMacro);

      switch(trackMacro)
      {
      case 0: // warp
        {
          int roomNumber = *(short int*)(trackPtr);
          trackPtr += 2;

          if(currentProcessedActorPtr->room != roomNumber)
          {
            if(genVar9 == currentProcessedActorIdx)
            {
              needChangeRoom = 1;
              newRoom = roomNumber;
            }

            currentProcessedActorPtr->room = roomNumber;
          }

          currentProcessedActorPtr->zv.ZVX1 -= currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
          currentProcessedActorPtr->zv.ZVX2 -= currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
          currentProcessedActorPtr->zv.ZVY1 -= currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
          currentProcessedActorPtr->zv.ZVY2 -= currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
          currentProcessedActorPtr->zv.ZVZ1 -= currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;
          currentProcessedActorPtr->zv.ZVZ2 -= currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;

          currentProcessedActorPtr->worldX = currentProcessedActorPtr->roomX = *(short int*)(trackPtr);
          trackPtr += 2;
          currentProcessedActorPtr->worldY = currentProcessedActorPtr->roomY = *(short int*)(trackPtr);
          trackPtr += 2;
          currentProcessedActorPtr->worldZ = currentProcessedActorPtr->roomZ = *(short int*)(trackPtr);
          trackPtr += 2;

          currentProcessedActorPtr->worldX -= (s16)((roomDataTable[currentDisplayedRoom].worldX - roomDataTable[currentProcessedActorPtr->room].worldX) * 10);
          currentProcessedActorPtr->worldY += (s16)((roomDataTable[currentDisplayedRoom].worldY - roomDataTable[currentProcessedActorPtr->room].worldY) * 10);
          currentProcessedActorPtr->worldZ += (s16)((roomDataTable[currentDisplayedRoom].worldZ - roomDataTable[currentProcessedActorPtr->room].worldZ) * 10);

          currentProcessedActorPtr->zv.ZVX1 += currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
          currentProcessedActorPtr->zv.ZVX2 += currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
          currentProcessedActorPtr->zv.ZVY1 += currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
          currentProcessedActorPtr->zv.ZVY2 += currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
          currentProcessedActorPtr->zv.ZVZ1 += currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;
          currentProcessedActorPtr->zv.ZVZ2 += currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;

          currentProcessedActorPtr->speed = 0;
          currentProcessedActorPtr->field_72 = 0;
          currentProcessedActorPtr->rotate.param = 0;
          currentProcessedActorPtr->positionInTrack += 5;

          break;
        }
      case 1: // goToPosition
        {
          int roomNumber = *(short int*)(trackPtr);
          int x;
          int y;
          int z;
          unsigned int distanceToPoint;
          
          trackPtr += 2;

          x = *(short int*)(trackPtr);
          trackPtr += 2;
          y = 0;
          z = *(short int*)(trackPtr);
          trackPtr += 2;
    
          if(roomNumber != currentProcessedActorPtr->room)
          {
            // TODO: fix bug here...
            x -= (roomDataTable[currentProcessedActorPtr->room].worldX - roomDataTable[roomNumber].worldX) * 10;
            z -= (roomDataTable[currentProcessedActorPtr->room].worldZ - roomDataTable[roomNumber].worldZ) * 10;
          }

          distanceToPoint = computeDistanceToPoint( currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                                      currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                                      x,z );


          if(distanceToPoint >= DISTANCE_TO_POINT_TRESSHOLD) // not yet at position
          {
            int angleModif = computeAngleModificatorToPosition( currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                                      currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                                      currentProcessedActorPtr->beta,
                                      x,z );

            if(currentProcessedActorPtr->rotate.param == 0 || currentProcessedActorPtr->field_72 != angleModif)
            {
              startActorRotation(currentProcessedActorPtr->beta, currentProcessedActorPtr->beta - (angleModif<<6), 15, &currentProcessedActorPtr->rotate);
            }

            currentProcessedActorPtr->field_72 = angleModif;

            if(!angleModif)
            {
              currentProcessedActorPtr->rotate.param = 0;
            }
            else
            {
              currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
            }
          }
          else // reached position
          {
            currentProcessedActorPtr->positionInTrack += 4;
          }
            
          break;
        }
      case 2: // stop
        {
          currentProcessedActorPtr->speed = 0;
          currentProcessedActorPtr->trackNumber = -1;
          setMoveMode(0,0);
          break;
        }
      case 3:
        {
          currentProcessedActorPtr->positionInTrack = 0;
          break;
        }
      case 4: // MARK
        {
          currentProcessedActorPtr->MARK = *(short int*)(trackPtr);
          trackPtr += 2;
          currentProcessedActorPtr->positionInTrack += 2;
          break;
        }
      case 0x5:
        {
          currentProcessedActorPtr->speed = 4;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }
      case 0x6:
        {
          currentProcessedActorPtr->speed = 5;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }
      case 0x7:
        {
          currentProcessedActorPtr->speed = 0;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }
      case 0x9:
        {
          int betaDif = *(short int*)(trackPtr);
          trackPtr += 2;

          if(((currentProcessedActorPtr->beta - betaDif)&0x3FF) > 0x200)
          {
            currentProcessedActorPtr->field_72 = 1;
          }
          else
          {
            currentProcessedActorPtr->field_72 = -1;
          }

          if(!currentProcessedActorPtr->rotate.param)
          {
            startActorRotation(currentProcessedActorPtr->beta, betaDif, 120, &currentProcessedActorPtr->rotate);
          }

          currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);

          if(currentProcessedActorPtr->beta == betaDif)
          {
            currentProcessedActorPtr->field_72 = 0;

            currentProcessedActorPtr->positionInTrack+=2;
          }

          break;
        }
      case 0xA:
        {
          currentProcessedActorPtr->dynFlags &= 0xFFFE;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }
      case 0xB:
        {
          currentProcessedActorPtr->dynFlags |= 1;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }
      case 0xD:
        {
          currentProcessedActorPtr->flags &= 0xFFBF;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }
      case 0xE:
        {
          currentProcessedActorPtr->flags |= 0x40;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }
      case 0x10:
        {
          int objNum = currentProcessedActorPtr->field_0;

          objectTable[objNum].x = currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
          objectTable[objNum].y = currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
          objectTable[objNum].z = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;

          currentProcessedActorPtr->positionInTrack++;

          break;
        }
      case 0x11: // walk up/down stairs on X
        {
          int x;
          int y;
          int z;
          int objX;
          int objY;
          int objZ;
          
          x = *(short int*)(trackPtr);
          trackPtr += 2;
          y = *(short int*)(trackPtr);
          trackPtr += 2;
          z = *(short int*)(trackPtr);
          trackPtr += 2;
          
          objX = objectTable[currentProcessedActorPtr->field_0].x;
          objY = objectTable[currentProcessedActorPtr->field_0].y;
          objZ = objectTable[currentProcessedActorPtr->field_0].z;

          if(   currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY < y - 100
            ||  currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY > y + 100)
          {
            int propX = makeProportional(objY, y, x - objX, (currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX) - objX);

            int difY = propX - currentProcessedActorPtr->worldY;
            int angleModif;

            currentProcessedActorPtr->worldY += difY;
            currentProcessedActorPtr->roomY += difY;
            currentProcessedActorPtr->zv.ZVY1 += difY;
            currentProcessedActorPtr->zv.ZVY2 += difY;

            angleModif = computeAngleModificatorToPosition( currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                                      currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                                      currentProcessedActorPtr->beta,
                                      x,z );

            if(!currentProcessedActorPtr->rotate.param || currentProcessedActorPtr->field_72 != angleModif)
            {
              startActorRotation(currentProcessedActorPtr->beta, currentProcessedActorPtr->beta - (angleModif<<8), 60, &currentProcessedActorPtr->rotate);
            }

            currentProcessedActorPtr->field_72 = angleModif;

            if(angleModif)
            {
              currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
            }
            else
            {
              currentProcessedActorPtr->rotate.param = 0;
            }

          }
          else
          {
            int difY = y - currentProcessedActorPtr->worldY;

            currentProcessedActorPtr->modY = 0;
            currentProcessedActorPtr->worldY += difY;
            currentProcessedActorPtr->roomY += difY;
            currentProcessedActorPtr->zv.ZVY1 += difY;
            currentProcessedActorPtr->zv.ZVY2 += difY;

            currentProcessedActorPtr->positionInTrack +=4;
          }
          
          break;
        }
      case 0x12: // walk up/down stairs on Z
        {
          int x;
          int y;
          int z;
          int objX;
          int objY;
          int objZ;
          
          x = *(short int*)(trackPtr);
          trackPtr += 2;
          y = *(short int*)(trackPtr);
          trackPtr += 2;
          z = *(short int*)(trackPtr);
          trackPtr += 2;

          objX = objectTable[currentProcessedActorPtr->field_0].x;
          objY = objectTable[currentProcessedActorPtr->field_0].y;
          objZ = objectTable[currentProcessedActorPtr->field_0].z;

          if(   currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY < y - 100
            ||  currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY > y + 100)
          {
            int propZ = makeProportional(objY, y, z - objZ, (currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ) - objZ);

            int difY = propZ - currentProcessedActorPtr->worldY;
            
            int angleModif;

            currentProcessedActorPtr->worldY += difY;
            currentProcessedActorPtr->roomY += difY;
            currentProcessedActorPtr->zv.ZVY1 += difY;
            currentProcessedActorPtr->zv.ZVY2 += difY;

            angleModif = computeAngleModificatorToPosition( currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                                      currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                                      currentProcessedActorPtr->beta,
                                      x,z );

            if(!currentProcessedActorPtr->rotate.param || currentProcessedActorPtr->field_72 != angleModif)
            {
              startActorRotation(currentProcessedActorPtr->beta, currentProcessedActorPtr->beta - (angleModif<<8), 60, &currentProcessedActorPtr->rotate);
            }

            currentProcessedActorPtr->field_72 = angleModif;

            if(angleModif)
            {
              currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
            }
            else
            {
              currentProcessedActorPtr->rotate.param = 0;
            }

          }
          else
          {
            int difY = y - currentProcessedActorPtr->worldY;

            currentProcessedActorPtr->modY = 0;
            currentProcessedActorPtr->worldY += difY;
            currentProcessedActorPtr->roomY += difY;
            currentProcessedActorPtr->zv.ZVY1 += difY;
            currentProcessedActorPtr->zv.ZVY2 += difY;

            currentProcessedActorPtr->positionInTrack +=4;
          }
          
          break;
        }
      case 0x13: // rotate
        {
          currentProcessedActorPtr->alpha = *(short int*)(trackPtr);
          trackPtr += 2;
          currentProcessedActorPtr->beta = *(short int*)(trackPtr);
          trackPtr += 2;
          currentProcessedActorPtr->gamma = *(short int*)(trackPtr);
          trackPtr += 2;

          currentProcessedActorPtr->field_72 = 0;

          currentProcessedActorPtr->positionInTrack +=4;

          break;
        }
      default:
        {
          printf("Unknown track macro %X\n",trackMacro);
          exit(1);
          break;
        }
      }

      break;
    }
  }

  currentProcessedActorPtr->beta &= 0x3FF;
}

void processTrack2(void)
{
  switch(currentProcessedActorPtr->trackMode)
  {
  case 1: // manual
    {
      manualRot(60);
      if(input4&1) // forward
      {
        if(timer - lastTimeForward < 10 && currentProcessedActorPtr->speed != 4)
          currentProcessedActorPtr->speed = 5;
        else
        if(currentProcessedActorPtr->speed == 0 || currentProcessedActorPtr->speed == -1)
          currentProcessedActorPtr->speed = 4;

/*        if(currentProcessedActorPtr->speed>0 && currentProcessedActorPtr->speed<4)
          currentProcessedActorPtr->speed = 5; */


        lastTimeForward = timer;
      }
      else
      {
        if(currentProcessedActorPtr->speed>0 && currentProcessedActorPtr->speed<=4)
        {
          currentProcessedActorPtr->speed--;
        }
        else
        {
          currentProcessedActorPtr->speed = 0;
        }
      }

      if(input4&2) // backward
      {
        if(currentProcessedActorPtr->speed == 0 || currentProcessedActorPtr->speed >= 4)
          currentProcessedActorPtr->speed = -1;

        if(currentProcessedActorPtr->speed == 5)
          currentProcessedActorPtr->speed = 0;
      }

      break;
    }
  case 2: // follow
    {
      int followedActorIdx = objectTable[currentProcessedActorPtr->trackNumber].ownerIdx;

      if(followedActorIdx == -1)
      {
        currentProcessedActorPtr->field_72 = 0;
        currentProcessedActorPtr->speed = 0;
      }
      else
      {
        actorStruct* followedActorPtr = &actorTable[followedActorIdx];

        int roomNumber = followedActorPtr->room;
        int x = followedActorPtr->roomX;
        int y = followedActorPtr->roomY;
        int z = followedActorPtr->roomZ;
        int angleModif;

        if(currentProcessedActorPtr->room != roomNumber)
        {
        /*  char* link = getRoomLink(currentProcessedActorPtr->room,roomNumber);

          x = *(short int*)(link)+(((*(short int*)(link+2))-(*(short int *)(link))) / 2);
          y = *(short int*)(link+4)+(((*(short int*)(link+6))-(*(short int *)(link+4))) / 2);
          z = *(short int*)(link+8)+(((*(short int*)(link+10))-(*(short int *)(link+8))) / 2); */
        }

        angleModif = computeAngleModificatorToPosition( currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                                  currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                                  currentProcessedActorPtr->beta, x, z );

        if( currentProcessedActorPtr->rotate.param == 0 || currentProcessedActorPtr->field_72 != angleModif )
        {
          startActorRotation( currentProcessedActorPtr->beta, currentProcessedActorPtr->beta - (angleModif << 8), 60, &currentProcessedActorPtr->rotate);
        }

        currentProcessedActorPtr->field_72 = angleModif;

        if( currentProcessedActorPtr->field_72 == 0 )
        {
          currentProcessedActorPtr->rotate.param = 0;
        }
        else
        {
          currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
        }

        currentProcessedActorPtr->speed = 4;

      }
      break;
    }
  case 3: // track
    {
      char* trackPtr = HQR_Get(listTrack,currentProcessedActorPtr->trackNumber);
      short int trackMacro;
      
      trackPtr+=currentProcessedActorPtr->positionInTrack * 2;

      trackMacro = *(short int*)trackPtr;
      trackPtr += 2;

      //printf("Track macro %X\n",trackMacro);

      switch(trackMacro)
      {
      case 0: // warp
        {
          int roomNumber = *(short int*)(trackPtr);
          trackPtr += 2;

          if(currentProcessedActorPtr->room != roomNumber)
          {
            if(genVar9 == currentProcessedActorIdx)
            {
              needChangeRoom = 1;
              newRoom = roomNumber;
            }

            currentProcessedActorPtr->room = roomNumber;
          }

          currentProcessedActorPtr->zv.ZVX1 -= currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
          currentProcessedActorPtr->zv.ZVX2 -= currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
          currentProcessedActorPtr->zv.ZVY1 -= currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
          currentProcessedActorPtr->zv.ZVY2 -= currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
          currentProcessedActorPtr->zv.ZVZ1 -= currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;
          currentProcessedActorPtr->zv.ZVZ2 -= currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;

          currentProcessedActorPtr->worldX = currentProcessedActorPtr->roomX = *(short int*)(trackPtr);
          trackPtr += 2;
          currentProcessedActorPtr->worldY = currentProcessedActorPtr->roomY = *(short int*)(trackPtr);
          trackPtr += 2;
          currentProcessedActorPtr->worldZ = currentProcessedActorPtr->roomZ = *(short int*)(trackPtr);
          trackPtr += 2;

          currentProcessedActorPtr->worldX -= (s16)((roomDataTable[currentDisplayedRoom].worldX - roomDataTable[currentProcessedActorPtr->room].worldX) * 10);
          currentProcessedActorPtr->worldY += (s16)((roomDataTable[currentDisplayedRoom].worldY - roomDataTable[currentProcessedActorPtr->room].worldY) * 10);
          currentProcessedActorPtr->worldZ += (s16)((roomDataTable[currentDisplayedRoom].worldZ - roomDataTable[currentProcessedActorPtr->room].worldZ) * 10);

          currentProcessedActorPtr->zv.ZVX1 += currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
          currentProcessedActorPtr->zv.ZVX2 += currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
          currentProcessedActorPtr->zv.ZVY1 += currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
          currentProcessedActorPtr->zv.ZVY2 += currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
          currentProcessedActorPtr->zv.ZVZ1 += currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;
          currentProcessedActorPtr->zv.ZVZ2 += currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;

          currentProcessedActorPtr->speed = 0;
          currentProcessedActorPtr->field_72 = 0;
          currentProcessedActorPtr->rotate.param = 0;
          currentProcessedActorPtr->positionInTrack += 5;

          break;
        }
      case 1: // goToPosition
        {
          int roomNumber = *(short int*)(trackPtr);
          int x;
          int y;
          int z;
          unsigned int distanceToPoint;
          
          trackPtr += 2;

          x = *(short int*)(trackPtr);
          trackPtr += 2;
          y = 0;
          z = *(short int*)(trackPtr);
          trackPtr += 2;
    
          if(roomNumber != currentProcessedActorPtr->room)
          {
            // TODO: fix bug here...
            x -= (roomDataTable[currentProcessedActorPtr->room].worldX - roomDataTable[roomNumber].worldX) * 10;
            z -= (roomDataTable[currentProcessedActorPtr->room].worldZ - roomDataTable[roomNumber].worldZ) * 10;
          }

          distanceToPoint = computeDistanceToPoint( currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                                      currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                                      x,z );


          if(distanceToPoint >= DISTANCE_TO_POINT_TRESSHOLD) // not yet at position
          {
            int angleModif = computeAngleModificatorToPosition( currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                                      currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                                      currentProcessedActorPtr->beta,
                                      x,z );

            if(currentProcessedActorPtr->rotate.param == 0 || currentProcessedActorPtr->field_72 != angleModif)
            {
              startActorRotation(currentProcessedActorPtr->beta, currentProcessedActorPtr->beta - (angleModif<<6), 15, &currentProcessedActorPtr->rotate);
            }

            currentProcessedActorPtr->field_72 = angleModif;

            if(!angleModif)
            {
              currentProcessedActorPtr->rotate.param = 0;
            }
            else
            {
              currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
            }
          }
          else // reached position
          {
            currentProcessedActorPtr->positionInTrack += 4;
          }
            
          break;
        }
      case 2: // stop
        {
          currentProcessedActorPtr->speed = 0;
          currentProcessedActorPtr->trackNumber = -1;
          setMoveMode(0,0);
          break;
        }
      case 3:
        {
          currentProcessedActorPtr->positionInTrack = 0;
          break;
        }
      case 4: // MARK
        {
          currentProcessedActorPtr->MARK = *(short int*)(trackPtr);
          trackPtr += 2;
          currentProcessedActorPtr->positionInTrack += 2;
          break;
        }
      case 5:
        {
          break;
        }
      case 0x6:
        {
          int betaDif = *(short int*)(trackPtr);
          trackPtr += 2;

          if(((currentProcessedActorPtr->beta - betaDif)&0x3FF) > 0x200)
          {
            currentProcessedActorPtr->field_72 = 1;
          }
          else
          {
            currentProcessedActorPtr->field_72 = -1;
          }

          if(!currentProcessedActorPtr->rotate.param)
          {
            startActorRotation(currentProcessedActorPtr->beta, betaDif, 120, &currentProcessedActorPtr->rotate);
          }

          currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);

          if(currentProcessedActorPtr->beta == betaDif)
          {
            currentProcessedActorPtr->field_72 = 0;

            currentProcessedActorPtr->positionInTrack+=2;
          }

          break;
        }
      case 0x7:
        {
          currentProcessedActorPtr->dynFlags &= 0xFFFE;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }
      case 0x8:
        {
          currentProcessedActorPtr->dynFlags |= 1;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }
      case 0xA:
        {
          currentProcessedActorPtr->flags &= 0xFFBF;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }
      case 0xB:
        {
          currentProcessedActorPtr->flags |= 0x40;
          currentProcessedActorPtr->positionInTrack++;
          break;
        }/*
      case 0x10:
        {
          int objNum = currentProcessedActorPtr->field_0;

          objectTable[objNum].x = currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX;
          objectTable[objNum].y = currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY;
          objectTable[objNum].z = currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ;

          currentProcessedActorPtr->positionInTrack++;

          break;
        }
      case 0x11: // walk up/down stairs on X
        {
          int x;
          int y;
          int z;
          int objX;
          int objY;
          int objZ;
          
          x = *(short int*)(trackPtr);
          trackPtr += 2;
          y = *(short int*)(trackPtr);
          trackPtr += 2;
          z = *(short int*)(trackPtr);
          trackPtr += 2;
          
          objX = objectTable[currentProcessedActorPtr->field_0].x;
          objY = objectTable[currentProcessedActorPtr->field_0].y;
          objZ = objectTable[currentProcessedActorPtr->field_0].z;

          if(   currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY < y - 100
            ||  currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY > y + 100)
          {
            int propX = makeProportional(objY, y, x - objX, (currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX) - objX);

            int difY = propX - currentProcessedActorPtr->worldY;
            int angleModif;

            currentProcessedActorPtr->worldY += difY;
            currentProcessedActorPtr->roomY += difY;
            currentProcessedActorPtr->zv.ZVY1 += difY;
            currentProcessedActorPtr->zv.ZVY2 += difY;

            angleModif = computeAngleModificatorToPosition( currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                                      currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                                      currentProcessedActorPtr->beta,
                                      x,z );

            if(!currentProcessedActorPtr->rotate.param || currentProcessedActorPtr->field_72 != angleModif)
            {
              startActorRotation(currentProcessedActorPtr->beta, currentProcessedActorPtr->beta - (angleModif<<8), 60, &currentProcessedActorPtr->rotate);
            }

            currentProcessedActorPtr->field_72 = angleModif;

            if(angleModif)
            {
              currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
            }
            else
            {
              currentProcessedActorPtr->rotate.param = 0;
            }

          }
          else
          {
            int difY = y - currentProcessedActorPtr->worldY;

            currentProcessedActorPtr->modY = 0;
            currentProcessedActorPtr->worldY += difY;
            currentProcessedActorPtr->roomY += difY;
            currentProcessedActorPtr->zv.ZVY1 += difY;
            currentProcessedActorPtr->zv.ZVY2 += difY;

            currentProcessedActorPtr->positionInTrack +=4;
          }
          
          break;
        }
      case 0x12: // walk up/down stairs on Z
        {
          int x;
          int y;
          int z;
          int objX;
          int objY;
          int objZ;
          
          x = *(short int*)(trackPtr);
          trackPtr += 2;
          y = *(short int*)(trackPtr);
          trackPtr += 2;
          z = *(short int*)(trackPtr);
          trackPtr += 2;

          objX = objectTable[currentProcessedActorPtr->field_0].x;
          objY = objectTable[currentProcessedActorPtr->field_0].y;
          objZ = objectTable[currentProcessedActorPtr->field_0].z;

          if(   currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY < y - 100
            ||  currentProcessedActorPtr->roomY + currentProcessedActorPtr->modY > y + 100)
          {
            int propZ = makeProportional(objY, y, z - objZ, (currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ) - objZ);

            int difY = propZ - currentProcessedActorPtr->worldY;
            
            int angleModif;

            currentProcessedActorPtr->worldY += difY;
            currentProcessedActorPtr->roomY += difY;
            currentProcessedActorPtr->zv.ZVY1 += difY;
            currentProcessedActorPtr->zv.ZVY2 += difY;

            angleModif = computeAngleModificatorToPosition( currentProcessedActorPtr->roomX + currentProcessedActorPtr->modX,
                                      currentProcessedActorPtr->roomZ + currentProcessedActorPtr->modZ,
                                      currentProcessedActorPtr->beta,
                                      x,z );

            if(!currentProcessedActorPtr->rotate.param || currentProcessedActorPtr->field_72 != angleModif)
            {
              startActorRotation(currentProcessedActorPtr->beta, currentProcessedActorPtr->beta - (angleModif<<8), 60, &currentProcessedActorPtr->rotate);
            }

            currentProcessedActorPtr->field_72 = angleModif;

            if(angleModif)
            {
              currentProcessedActorPtr->beta = updateActorRotation(&currentProcessedActorPtr->rotate);
            }
            else
            {
              currentProcessedActorPtr->rotate.param = 0;
            }

          }
          else
          {
            int difY = y - currentProcessedActorPtr->worldY;

            currentProcessedActorPtr->modY = 0;
            currentProcessedActorPtr->worldY += difY;
            currentProcessedActorPtr->roomY += difY;
            currentProcessedActorPtr->zv.ZVY1 += difY;
            currentProcessedActorPtr->zv.ZVY2 += difY;

            currentProcessedActorPtr->positionInTrack +=4;
          }
          
          break;
        }
      case 0x13: // rotate
        {
          currentProcessedActorPtr->alpha = *(short int*)(trackPtr);
          trackPtr += 2;
          currentProcessedActorPtr->beta = *(short int*)(trackPtr);
          trackPtr += 2;
          currentProcessedActorPtr->gamma = *(short int*)(trackPtr);
          trackPtr += 2;

          currentProcessedActorPtr->field_72 = 0;

          currentProcessedActorPtr->positionInTrack +=4;

          break;
        } */
      default:
        {
          printf("Unknown track macro %X\n",trackMacro);
          exit(1);
          break;
        }
      }

      break;
    }
  }

  currentProcessedActorPtr->beta &= 0x3FF;
}
