#ifndef _LIFE2_
#define _LIFE2_

#define NUM_MAX_SEQUENCE_PARAM 30

struct sequenceParamStruct
{
  unsigned int frame;
  unsigned int sample;
};

typedef struct sequenceParamStruct sequenceParamStruct;

extern short int numObjInInventoryTable[2];
extern short int inHandTable[2];
extern short int currentInHand;

extern int numSequenceParam;

extern sequenceParamStruct sequenceParams[NUM_MAX_SEQUENCE_PARAM];

void processLife2(int lifeNum);
void convertPaletteIfRequired(unsigned char* lpalette);

extern short int numObjInInventoryTable[2];
extern short int inHandTable[2];
extern short int currentInHand;

#endif

