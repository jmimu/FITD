#include "alonebody.h"

AloneBody::AloneBody(AloneFile *file) : file(file),modelFlags(0),numOfPoints(0),allPoints(nullptr),numOfBones(0),allBones(nullptr)
{
}

bool AloneBody::load()
{
	printf("Load ing file %s\n",file->mPAKFilename);
	u8 * ptr = file->mDecomprData;

	//from renderModel
	modelFlags = READ_LE_S16(ptr);
	ptr+=2;
	ptr+=12;
	ptr+=READ_LE_S16(ptr) + 2;

	//from computeModel
	numOfPoints = *(short int*)ptr;
	ptr+=2;

	ASSERT(numOfPoints<NUM_MAX_POINT_IN_POINT_BUFFER);
	allPoints=new Pt3d[numOfPoints];
	printf("sizeof Pt3d: %lu\n",sizeof(Pt3d));

	memcpy(allPoints,ptr,numOfPoints*3*2);
	ptr+=numOfPoints*3*2;

	numOfBones = *(short int*)ptr;
	ptr+=2;

	ASSERT(numOfBones<NUM_MAX_BONES);
	allBones=new Bone[numOfBones];

	memcpy(allBones,ptr,numOfBones*2);
	ptr+=numOfBones*2;

	char fname[256+9]="toto.ply";
	sprintf(fname,"%s_%05d.ply",file->mPAKFilename,file->mIndex);

	exportPly(fname);
	return true;
}

bool AloneBody::exportPly(char* filename)
{
	printf("Export ply into %s\n",filename);
	FILE* fileHandle;
	fileHandle = fopen(filename,"w");
	fprintf(fileHandle,"ply\n");
	fprintf(fileHandle,"format ascii 1.0\n");
	fprintf(fileHandle,"element vertex %d\n",numOfPoints);
	fprintf(fileHandle,"property float32 x\n");
	fprintf(fileHandle,"property float32 y\n");
	fprintf(fileHandle,"property float32 z\n");
	fprintf(fileHandle,"element face 0\n");
	fprintf(fileHandle,"end_header\n");
	for (int i=0;i<numOfPoints;i++)
	{
		fprintf(fileHandle,"%d %d %d\n", allPoints[i].x, allPoints[i].y, allPoints[i].z);
	}
	fclose(fileHandle);
	return true;
}
