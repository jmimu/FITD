#ifndef ALONEFLOOR_H
#define ALONEFLOOR_H


#include "alonefile.h"
extern "C" {
#include "../src/vars.h"
#include "../src/room.h"
#include "../src/floor.h"
#include "../src/hqr.h"
}


class AloneFloor
{
public:
    AloneFloor();
    ~AloneFloor();
    bool load(AloneFile *rooms,AloneFile *cams);

    roomDataStruct* roomDataTable;

};

#endif // ALONEFLOOR_H
