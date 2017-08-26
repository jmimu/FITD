#include "alonefloor.h"

#include <fstream>
#include <sstream>

AloneFloor::AloneFloor():roomDataTable(0),mRooms(0),mCams(0)
{

}

AloneFloor::~AloneFloor()
{
    if (roomDataTable) delete roomDataTable;
}

bool AloneFloor::load(AloneFile *rooms,AloneFile *cams)
{
    mRooms=rooms;
    mCams=cams;

    gameTypeEnum gameId=AITD1;
    u8* etageVar0=rooms->mDecomprData;
    u8* etageVar1=cams->mDecomprData;
    printf("DATA: %x %x %x %x\n",etageVar0[0],etageVar0[1],etageVar0[2],etageVar0[3]);
    printf("VAL: %u\n",*((u32*)etageVar0));


    etageVar0Size=rooms->mInfo.uncompressedSize;
    unsigned int cameraDataSize=cams->mInfo.uncompressedSize;
    u32 i;

    if(roomDataTable)
    {
      free(roomDataTable);
      roomDataTable = NULL;
    }

    expectedNumberOfRoom = 0;
    if(gameId >= AITD3)
    {
      printf("ERROR! make something for gameId >= AITD3!\n");
      return false;
    }
    u32 numMax = (((READ_LE_U32(etageVar0))/4));
    for(i=0;i<numMax;i++)
    {
      if(etageVar0Size >= READ_LE_U32(etageVar0 + i * 4))
        expectedNumberOfRoom++;
      else
        break;
    }

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
        roomData = rooms->mDecomprData;
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
      printf("numHardCol: %u\n",currentRoomDataPtr->numHardCol);
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
      printf("numSceZone: %u\n",currentRoomDataPtr->numSceZone);
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
      /*char buffer[256];

      if(gameId == AITD3)
      {
        sprintf(buffer,"CAM%02d",floorNumber);
      }
      else
      {
        sprintf(buffer,"CAMSAL%02d",floorNumber);
      }

      expectedNumberOfCamera = PAK_getNumFiles(buffer);*/
      printf("ERROR! make something for gameId >= AITD3!\n");
      return false;
    }
    else
    {
      expectedNumberOfCamera = ((READ_LE_U32(etageVar1))/4);
    }
    printf("expectedNumberOfCamera: %u\n",expectedNumberOfCamera);

    globalCameraDataTable = (cameraDataStruct*)malloc(sizeof(cameraDataStruct)*expectedNumberOfCamera);

    for(i=0;i<expectedNumberOfCamera;i++)
    {
      u32 k;
      u32 offset;
      u8* currentCameraData;

      if(gameId >= AITD3)
      {
        /*char buffer[256];

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
        currentCameraData = loadPakSafe(buffer,i);*/
        printf("ERROR! make something for gameId >= AITD3!\n");
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

    exportCollada();
    return true;
}

std::string AloneFloor::hardCol2collada(ZVStruct* zvData, int index, float roomX, float roomY, float roomZ)
{
    std::ostringstream oss;
    float x=(zvData->ZVX1)/1000.0+roomX/100.0;
    float y=(zvData->ZVY1)/1000.0+roomY/100.0;
    float z=(zvData->ZVZ1)/1000.0-roomZ/100.0;
    float sx=(zvData->ZVX2-zvData->ZVX1)/1000.0;
    float sy=(zvData->ZVY2-zvData->ZVY1)/1000.0;
    float sz=(zvData->ZVZ2-zvData->ZVZ1)/1000.0;
    oss<<"      <node id=\"Col"<<index<<"\" name=\"Col"<<index<<"\" type=\"NODE\">\n";
    oss<<"        <matrix sid=\"transform\">"<<sx<<" 0 0 "<<x<<" 0 "<<sy<<" 0 "<<y<<" 0 0 "<<sz<<" "<<z<<" 0 0 0 1</matrix>\n";
    oss<<"        <instance_geometry url=\"#Cube-mesh\"/>\n";
    oss<<"      </node>\n";
    //TODO: add parameter and type
    return oss.str();
}

std::string AloneFloor::sceZone2collada(ZVStruct* zvData, int index, float roomX, float roomY, float roomZ)
{
    std::ostringstream oss;
    float x=(zvData->ZVX1)/1000.0+roomX/100.0;
    float y=(zvData->ZVY2)/1000.0+roomY/100.0;
    float z=(zvData->ZVZ1)/1000.0-roomZ/100.0;
    float sx=(zvData->ZVX2-zvData->ZVX1)/1000.0;
    float sy=1;//(zvData->ZVY2-zvData->ZVY1)/100.0;
    float sz=(zvData->ZVZ2-zvData->ZVZ1)/1000.0;
    oss<<"      <node id=\"Sce"<<index<<"\" name=\"Sce"<<index<<"\" type=\"NODE\">\n";
    oss<<"        <matrix sid=\"transform\">"<<sx<<" 0 0 "<<x<<" 0 "<<sy<<" 0 "<<y<<" 0 0 "<<sz<<" "<<z<<" 0 0 0 1</matrix>\n";
    oss<<"        <instance_geometry url=\"#Cube-mesh\"/>\n";
    oss<<"      </node>\n";
    //TODO: add parameter and type
    return oss.str();
}

std::string AloneFloor::cam2collada_lib(cameraDataStruct* cam, int index)
{
    std::ostringstream oss;
    oss<<"    <camera id=\"Camera-camera"<<index<<"\">\n";
    oss<<"      <optics>\n";
    oss<<"        <technique_common>\n";
    oss<<"          <perspective>\n";
    oss<<"            <xfov sid=\"xfov\">49.13434</xfov>\n";
    oss<<"            <aspect_ratio>1.777778</aspect_ratio>\n";
    oss<<"            <znear sid=\"znear\">0.1</znear>\n";
    oss<<"            <zfar sid=\"zfar\">100</zfar>\n";
    oss<<"          </perspective>\n";
    oss<<"        </technique_common>\n";
    oss<<"      </optics>\n";
    oss<<"    </camera>\n";
    return oss.str();
}

std::string AloneFloor::cam2collada_node(cameraDataStruct* cam, int index, float roomX, float roomY, float roomZ)
{
    std::ostringstream oss;
    float x=(cam->x-roomX)/100.0;
    float y=(roomY-cam->y)/100.0;
    float z=(roomZ-cam->z)/100.0;
    oss<<"      <node id=\"Cam"<<index<<"\" name=\"Cam"<<index<<"\" type=\"NODE\">\n";
    oss<<"        <matrix sid=\"transform\">1 0 0 "<<x<<" 0 1 0 "<<y<<" 0 0 1 "<<z<<" 0 0 0 1</matrix>\n";
    oss<<"        <instance_camera url=\"#Camera-camera"<<index<<"\"/>\n";
    oss<<"      </node>\n";

    std::cout<<"Cam "<<index<<": "<<cam->x<<","<<cam->y<<","<<cam->z;
    std::cout<<"  ang: "<<cam->alpha<<","<<cam->beta<<","<<cam->gamma;
    std::cout<<"  foc: "<<cam->focal1<<","<<cam->focal2<<","<<cam->focal3<<"\n";
    return oss.str();
}

void AloneFloor::exportCollada()
{
    std::ofstream outFile;
    char filename[256];
    strcpy(filename,mRooms->mPAKFilename);
    strcat(filename,".dae");
    outFile.open(filename);

    outFile<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    outFile<<"<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">\n";
    outFile<<"  <asset>\n";
    outFile<<"    <contributor>\n";
    outFile<<"      <author>Blender User</author>\n";
    outFile<<"      <authoring_tool>Blender 2.78.0 commit date:2017-02-24, commit time:14:33, hash:e92f2352830</authoring_tool>\n";
    outFile<<"    </contributor>\n";
    outFile<<"    <created>2017-08-21T17:33:59</created>\n";
    outFile<<"    <modified>2017-08-21T17:33:59</modified>\n";
    outFile<<"    <unit name=\"meter\" meter=\"1\"/>\n";
    outFile<<"    <up_axis>Z_UP</up_axis>\n";
    outFile<<"  </asset>\n";
    outFile<<"  <library_cameras>\n";

    for(int i=0;i<expectedNumberOfCamera;i++)
        outFile<<cam2collada_lib(&globalCameraDataTable[i],i);

    outFile<<"  </library_cameras>\n";
    outFile<<"  <library_images/>\n";
    outFile<<"  <library_geometries>\n";
    outFile<<"    <geometry id=\"Cube-mesh\">\n";
    outFile<<"      <mesh>\n";
    outFile<<"        <source id=\"Cube-mesh-positions\">\n";
    outFile<<"          <float_array id=\"Cube-mesh-positions-array\" count=\"24\">1 1 0 1 0 0 0 0 0 0 1 0 1 1 1 1 0 1 0 0 1 0 1 1</float_array>\n";
    outFile<<"          <technique_common>\n";
    outFile<<"            <accessor source=\"#Cube-mesh-positions-array\" count=\"8\" stride=\"3\">\n";
    outFile<<"              <param name=\"X\" type=\"float\"/>\n";
    outFile<<"              <param name=\"Y\" type=\"float\"/>\n";
    outFile<<"              <param name=\"Z\" type=\"float\"/>\n";
    outFile<<"            </accessor>\n";
    outFile<<"          </technique_common>\n";
    outFile<<"        </source>\n";
    outFile<<"        <vertices id=\"Cube-mesh-vertices\">\n";
    outFile<<"          <input semantic=\"POSITION\" source=\"#Cube-mesh-positions\"/>\n";
    outFile<<"        </vertices>\n";
    outFile<<"        <polylist count=\"6\">\n";
    outFile<<"          <input semantic=\"VERTEX\" source=\"#Cube-mesh-vertices\" offset=\"0\"/>\n";
    outFile<<"          <vcount>4 4 4 4 4 4 </vcount>\n";
    outFile<<"          <p>0 1 2 3 4 7 6 5 0 4 5 1 1 5 6 2 2 6 7 3 4 0 3 7</p>\n";
    outFile<<"        </polylist>\n";
    outFile<<"      </mesh>\n";
    outFile<<"    </geometry>\n";
    outFile<<"  </library_geometries>\n";
    outFile<<"  <library_controllers/>\n";
    outFile<<"  <library_visual_scenes>\n";
    outFile<<"    <visual_scene id=\"Scene\" name=\"Scene\">\n";

    for(int i=0;i<expectedNumberOfCamera;i++)
        outFile<<cam2collada_node(&globalCameraDataTable[i],i,0,0,0);//TODO: determine room and give its center

    for (int i=0;i<expectedNumberOfRoom;i++)
    {
        roomDataStruct* currentRoomDataPtr = &roomDataTable[i];
        for (int j=0;j<currentRoomDataPtr->numHardCol;j++)
        {
            ZVStruct* zvData;
            zvData = &currentRoomDataPtr->hardColTable[j].zv;
            outFile<<hardCol2collada(zvData,j,currentRoomDataPtr->worldX,currentRoomDataPtr->worldY,currentRoomDataPtr->worldZ);
        }
        for (int j=0;j<currentRoomDataPtr->numSceZone;j++)
        {
            ZVStruct* zvData;
            zvData = &currentRoomDataPtr->sceZoneTable[j].zv;
            outFile<<sceZone2collada(zvData,j,currentRoomDataPtr->worldX,currentRoomDataPtr->worldY,currentRoomDataPtr->worldZ);
        }
    }
    outFile<<"      <node id=\"Cube0\" name=\"Cube0\" type=\"NODE\">\n";
    outFile<<"        <matrix sid=\"transform\">1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1</matrix>\n";
    outFile<<"        <instance_geometry url=\"#Cube-mesh\"/>\n";
    outFile<<"      </node>\n";

    outFile<<"    </visual_scene>\n";
    outFile<<"  </library_visual_scenes>\n";
    outFile<<"  <scene>\n";
    outFile<<"    <instance_visual_scene url=\"#Scene\"/>\n";
    outFile<<"  </scene>\n";
    outFile<<"</COLLADA>\n";
    outFile.close();
}
