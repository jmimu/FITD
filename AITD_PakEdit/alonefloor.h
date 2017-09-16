#ifndef ALONEFLOOR_H
#define ALONEFLOOR_H

#include <vector>
#include <QtXml>

#include "alonefile.h"
extern "C" {
#include "../src/vars.h"
#include "../src/room.h"
#include "../src/floor.h"
#include "../src/hqr.h"
}

struct AloneRoom
{
    s16 worldX;
    s16 worldY;
    s16 worldZ;
    u16 numCameraInRoom;
    std::vector<hardColStruct*> hardCols;
    std::vector<sceZoneStruct*> sceZones;

    u32 computeSize();
};

class AloneFloor
{
public:
    AloneFloor();
    ~AloneFloor();
    bool load(AloneFile *rooms,AloneFile *cams);
    void exportCollada();

    bool importCollada(const char *filename, AloneFile *roomsFile, AloneFile *camsFile);
    bool xml2struct(QDomNode &n);

    roomDataStruct* roomDataTable;
    AloneFile *mRooms;
    AloneFile *mCams;
    u32 expectedNumberOfRoom;
    u32 expectedNumberOfCamera;

    std::string hardCol2collada(hardColStruct &hardcol, int index, int roomNum);
    std::string sceZone2collada(sceZoneStruct & sceZone, int index, int roomNum);
    std::string cam2collada_lib(cameraDataStruct* cam, int index);
    std::string cam2collada_node(cameraDataStruct* cam, int index, int roomNum);

    //for import
    std::vector<AloneRoom> rooms;
};

#endif // ALONEFLOOR_H
