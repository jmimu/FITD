#include "alonefloor.h"

#include <fstream>
#include <sstream>
#include <QGenericMatrix>

extern s16 cosTable[];

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
    if (!globalCameraDataTable)
    {
        printf("Impossible to allocate globalCameraDataTable (%lu bytes)!\n",sizeof(cameraDataStruct)*expectedNumberOfCamera);
        return false;
    }

    for(i=0;i<expectedNumberOfCamera;i++)
    {
      printf("Camera %d\n",i);
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
    float fovX=2.0*atan(160.0/cam->focal2)*180.0/M_PI;
    float fovY=2.0*atan(160.0/cam->focal3)*180.0/M_PI;
    oss<<"    <camera id=\"Camera-camera"<<index<<"\">\n";
    oss<<"      <optics>\n";
    oss<<"        <technique_common>\n";
    oss<<"          <perspective>\n";
    oss<<"            <xfov sid=\"xfov\">"<<fovX<<"</xfov>\n";
    oss<<"            <yfov sid=\"yfov\">"<<fovY<<"</yfov>\n";
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

/*  for collada:
    a=alpha*pi/512
    b=beta*pi/512
    c=gamma*pi/512
    rx=[-1 0 0;0 cos(a) -sin(a);0 sin(a) cos(a)]
    ry=[cos(b) 0 -sin(b);0 1 0;sin(b) 0 cos(b)]
    rz=[cos(c) -sin(c) 0;sin(c) cos(c) 0; 0 0 1]
    r=(-rz*rx*ry)'

    dcam=R*[0;0;1]*focal1/1000
  */

    float camX=(cam->x-roomX)/100.0;
    float camY=(roomY-cam->y)/100.0;
    float camZ=(roomZ-cam->z)/100.0;

    double rotX_[]={
        -1,0,0,
        0,cosTable[(cam->alpha+0x100)&0x3FF]/32768.0,-cosTable[cam->alpha&0x3FF]/32768.0,
        0,cosTable[cam->alpha&0x3FF]/32768.0,cosTable[(cam->alpha+0x100)&0x3FF]/32768.0
    };
    double rotY_[]={
        cosTable[(cam->beta+0x100)&0x3FF]/32768.0,0,-cosTable[cam->beta&0x3FF]/32768.0,
        0,1,0,
        cosTable[cam->beta&0x3FF]/32768.0,0,cosTable[(cam->beta+0x100)&0x3FF]/32768.0
    };
    double rotZ_[]={
        cosTable[(cam->gamma+0x100)&0x3FF]/32768.0,-cosTable[cam->gamma&0x3FF]/32768.0,0,
        cosTable[cam->gamma&0x3FF]/32768.0,cosTable[(cam->gamma+0x100)&0x3FF]/32768.0,0,
        0,0,1
    };
    QGenericMatrix<3,3,double> rotX(rotX_);
    QGenericMatrix<3,3,double> rotY(rotY_);
    QGenericMatrix<3,3,double> rotZ(rotZ_);

    QGenericMatrix<3,3,double> rot=-(rotZ*rotX*rotY).transposed();

    double dcamX=rot(0,2)*cam->focal1/1000.0;
    double dcamY=rot(1,2)*cam->focal1/1000.0;
    double dcamZ=rot(2,2)*cam->focal1/1000.0;

    oss<<"      <node id=\"Cam"<<index<<"\" name=\"Cam"<<index<<"\" type=\"NODE\">\n";
    oss<<"        <matrix sid=\"transform\">"<<rot(0,0)<<" "<<rot(0,1)<<" "<<rot(0,2)<<" " <<camX+dcamX
       <<" "<<rot(1,0)<<" "<<rot(1,1)<<" "<<rot(1,2)<<" " <<camY+dcamY
       <<" "<<rot(2,0)<<" "<<rot(2,1)<<" "<<rot(2,2)<<" " <<camZ+dcamZ
       <<" 0 0 0 1</matrix>\n";
    oss<<"        <instance_camera url=\"#Camera-camera"<<index<<"\"/>\n";
    oss<<"      </node>\n";

    std::cout<<"Cam "<<index<<": "<<cam->x<<","<<cam->y<<","<<cam->z;
    std::cout<<"  ang: "<<cam->alpha<<","<<cam->beta<<","<<cam->gamma;
    std::cout<<"  foc: "<<cam->focal1<<","<<cam->focal2<<","<<cam->focal3<<"\n";
    return oss.str();
}

void AloneFloor::exportCollada()
{
    printf("exportCollada\n");
    char filename[256];
    strcpy(filename,mRooms->mPAKFilename);
    strcat(filename,".dae");
    std::cout<<"outname: "<<filename<<std::endl;
    std::ostringstream oss;

    oss<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    oss<<"<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">\n";
    oss<<"  <asset>\n";
    oss<<"    <contributor>\n";
    oss<<"      <author>Blender User</author>\n";
    oss<<"      <authoring_tool>Blender 2.78.0 commit date:2017-02-24, commit time:14:33, hash:e92f2352830</authoring_tool>\n";
    oss<<"    </contributor>\n";
    oss<<"    <created>2017-08-21T17:33:59</created>\n";
    oss<<"    <modified>2017-08-21T17:33:59</modified>\n";
    oss<<"    <unit name=\"meter\" meter=\"1\"/>\n";
    oss<<"    <up_axis>Z_UP</up_axis>\n";
    oss<<"  </asset>\n";
    oss<<"  <library_cameras>\n";

    for(unsigned int i=0;i<expectedNumberOfCamera;i++)
        oss<<cam2collada_lib(&globalCameraDataTable[i],i);
    printf("exportCollada cam assets\n");

    oss<<"  </library_cameras>\n";
    oss<<"  <library_images/>\n";
    oss<<"  <library_geometries>\n";
    oss<<"    <geometry id=\"Cube-mesh\">\n";
    oss<<"      <mesh>\n";
    oss<<"        <source id=\"Cube-mesh-positions\">\n";
    oss<<"          <float_array id=\"Cube-mesh-positions-array\" count=\"24\">1 1 0 1 0 0 0 0 0 0 1 0 1 1 1 1 0 1 0 0 1 0 1 1</float_array>\n";
    oss<<"          <technique_common>\n";
    oss<<"            <accessor source=\"#Cube-mesh-positions-array\" count=\"8\" stride=\"3\">\n";
    oss<<"              <param name=\"X\" type=\"float\"/>\n";
    oss<<"              <param name=\"Y\" type=\"float\"/>\n";
    oss<<"              <param name=\"Z\" type=\"float\"/>\n";
    oss<<"            </accessor>\n";
    oss<<"          </technique_common>\n";
    oss<<"        </source>\n";
    oss<<"        <vertices id=\"Cube-mesh-vertices\">\n";
    oss<<"          <input semantic=\"POSITION\" source=\"#Cube-mesh-positions\"/>\n";
    oss<<"        </vertices>\n";
    oss<<"        <polylist count=\"6\">\n";
    oss<<"          <input semantic=\"VERTEX\" source=\"#Cube-mesh-vertices\" offset=\"0\"/>\n";
    oss<<"          <vcount>4 4 4 4 4 4 </vcount>\n";
    oss<<"          <p>0 1 2 3 4 7 6 5 0 4 5 1 1 5 6 2 2 6 7 3 4 0 3 7</p>\n";
    oss<<"        </polylist>\n";
    oss<<"      </mesh>\n";
    oss<<"    </geometry>\n";
    oss<<"  </library_geometries>\n";
    oss<<"  <library_controllers/>\n";
    oss<<"  <library_visual_scenes>\n";
    oss<<"    <visual_scene id=\"Scene\" name=\"Scene\">\n";

    for(unsigned int i=0;i<expectedNumberOfCamera;i++)
        oss<<cam2collada_node(&globalCameraDataTable[i],i,0,0,0);//TODO: determine room and give its center
    printf("exportCollada cam nodes\n");

    for (unsigned int i=0;i<expectedNumberOfRoom;i++)
    {
        printf("exportCollada room %d\n",i);
        roomDataStruct* currentRoomDataPtr = &roomDataTable[i];
        for (unsigned int j=0;j<currentRoomDataPtr->numHardCol;j++)
        {
            ZVStruct* zvData;
            zvData = &currentRoomDataPtr->hardColTable[j].zv;
            oss<<hardCol2collada(zvData,j,currentRoomDataPtr->worldX,currentRoomDataPtr->worldY,currentRoomDataPtr->worldZ);
        }
        for (unsigned int j=0;j<currentRoomDataPtr->numSceZone;j++)
        {
            ZVStruct* zvData;
            zvData = &currentRoomDataPtr->sceZoneTable[j].zv;
            oss<<sceZone2collada(zvData,j,currentRoomDataPtr->worldX,currentRoomDataPtr->worldY,currentRoomDataPtr->worldZ);
        }
    }
    oss<<"      <node id=\"Cube0\" name=\"Cube0\" type=\"NODE\">\n";
    oss<<"        <matrix sid=\"transform\">1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1</matrix>\n";
    oss<<"        <instance_geometry url=\"#Cube-mesh\"/>\n";
    oss<<"      </node>\n";

    oss<<"    </visual_scene>\n";
    oss<<"  </library_visual_scenes>\n";
    oss<<"  <scene>\n";
    oss<<"    <instance_visual_scene url=\"#Scene\"/>\n";
    oss<<"  </scene>\n";
    oss<<"</COLLADA>\n";

   FILE *F;
   F=fopen(filename,"w");
   fprintf(F,"%s",oss.str().c_str());
   fclose(F);
   //std::ofstream fileout; //crash on windows??
   //fileout << oss.str()<<std::endl;
   //fileout.close();
}
