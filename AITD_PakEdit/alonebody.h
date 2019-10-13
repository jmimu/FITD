#ifndef ALONEBODY_H
#define ALONEBODY_H

extern "C" {
#include "../src/vars.h"
#include "../src/pak.h"
#include "../src/types.h"
}

#include "alonefile.h"

struct Pt3d
{
	s16 x;
	s16 y;
	s16 z;
};

struct Bone
{
	s16 data;//?
};

class AloneBody
{
public:
	AloneBody(AloneFile *file);
	bool load();
	bool exportPly(char* filename);
protected:
	AloneFile *file;
	s16 modelFlags;
	s16 numOfPoints;
	Pt3d * allPoints;
	s16 numOfBones;
	Bone * allBones;
};

#endif // ALONEBODY_H
