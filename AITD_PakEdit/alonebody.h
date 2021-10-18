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

struct Prim
{
	u8 primtype;
	u8 color;
    virtual void toPly(FILE* f);
};

struct PrimLine: public Prim
{
	s16 ptA_index;
	s16 ptB_index;
    virtual void toPly(FILE* f);
};

struct PrimPoly: public Prim
{
	u8 nbPts;
	u8 polytype;
	s16 * allPt_index;
    virtual void toPly(FILE* f);
};

struct PrimSphere: public Prim
{
	u8 type;
	u8 nbPts;
	s16 pt_index;
    //virtual void toPly(FILE* f);
};

struct PrimSqr: public Prim
{
	s16 pt_index;
    //virtual void toPly(FILE* f);
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
	s16 numOfPrim;
	Prim ** allPrims;
};

#endif // ALONEBODY_H
