#ifndef _VARS_
#define _VARS_

#include "common.h"
#include "osystem.h"

#pragma pack(1) //very bad, need to load structs without it (needs -O0)

extern u8 scaledScreen[640*400];

extern s16 cosTable[];

extern int input5;

extern u8* currentFoundBody;
extern int currentFoundBodyIdx;
extern int statusVar1;

extern enum gameTypeEnum gameId;

struct point3dStruct
{
  short int x;
  short int y;
  short int z;
};

typedef struct point3dStruct point3dStruct;

struct textEntryStruct
{
	short int index;
	char* textPtr;
	short int width;
};

typedef struct textEntryStruct textEntryStruct;

struct messageStruct
{
	textEntryStruct* string;
	short int time;
};

typedef struct messageStruct messageStruct;

struct saveEntry
{
	void* ptr;
	unsigned int size;
};

typedef struct saveEntry saveEntry;

struct regularTextEntryStruct
{
	char* textPtr;
	short int width;
};

typedef struct regularTextEntryStruct regularTextEntryStruct;

struct hqrSubEntryStruct
{
	short int key;
	short int size;
    unsigned int lastTimeUsed;
  u8* ptr;
};

typedef struct hqrSubEntryStruct hqrSubEntryStruct;

struct hqrEntryStruct
{
	char string[10];
	unsigned short int maxFreeData;
	unsigned short int sizeFreeData;
	unsigned short int numMaxEntry;
	unsigned short int numUsedEntry;
	hqrSubEntryStruct* entries;
};

typedef struct hqrEntryStruct hqrEntryStruct;

struct ZVStruct //TODO: check that s16 is OK (original FIDT is s32)
{
     s16 ZVX1;
     s16 ZVX2;
     s16 ZVY1;
     s16 ZVY2;
     s16 ZVZ1;
     s16 ZVZ2;
};

typedef struct ZVStruct ZVStruct;

struct hardColStruct
{
  ZVStruct zv;
  s16 parameter;
  s16 type;
};

typedef struct hardColStruct hardColStruct;


struct rotateStruct
{
	short int oldAngle;
	short int newAngle;
	short int param;
	short int timeOfRotate;
};

typedef struct rotateStruct rotateStruct;

struct actorFlags
{
  u16 flag0x0001:1;
  u16 flag0x0002:1;
  u16 flag0x0004:1;
  u16 flag0x0008:1;
  u16 flag0x0010:1;
  u16 flag0x0020:1;
  u16 flag0x0040:1;
  u16 tackable:1;
  u16 flag0x0100:1;
  u16 flag0x0200:1;
  u16 flag0x0400:1;
  u16 flag0x0800:1;
  u16 flag0x1000:1;
  u16 flag0x2000:1;
  u16 flag0x4000:1;
  u16 flag0x8000:1;
};


typedef struct actorFlags actorFlags;

struct actorStruct // used to read data from file too
{
	short int field_0;
	short int bodyNum;
  union
  {
	  short int flags;
    actorFlags bitField;
  };
	short int dynFlags;
	ZVStruct zv;
	short int field_14;
	short int field_16;
	short int field_18;
	short int field_1A;
	short int roomX;
	short int roomY;
	short int roomZ;
	short int worldX;
	short int worldY;
	short int worldZ;
	short int alpha;
	short int beta;
	short int gamma;
	short int stage;
	short int room;
	short int lifeMode;
	short int life;
	unsigned int CHRONO;
	unsigned int ROOM_CHRONO;
	short int ANIM;
	short int field_40;
	short int field_42;
	short int field_44;
	short int field_46;
	short int field_48;
	short int FRAME;
	short int field_4C;
	short int END_FRAME;
	short int END_ANIM;
	short int trackMode;
	short int trackNumber;
	short int MARK;
	short int positionInTrack;

	short int modX;
	short int modY;
	short int modZ;

	rotateStruct field_60;
	short int falling;
	rotateStruct rotate;
	short int field_72;
	short int speed;
	rotateStruct speedChange;
	short int COL[3];
	short int COL_BY;
	short int HARD_DEC;
	short int HARD_COL;
	short int HIT;
	short int HIT_BY;
	short int animActionType;
	short int animActionANIM;
	short int animActionFRAME;
	short int animActionParam;
	short int hitForce;
	short int field_98;
  point3dStruct hotPoint;

  // aitd2
  short int hardMat;
};

typedef struct actorStruct actorStruct;

struct objectStruct
{
	short int ownerIdx;
	short int body;
  union
  {
	  short int flags;
    actorFlags bitField;
  };
  short int field_6;
	short int foundBody;
	short int foundName;
	short int flags2;
	short int foundLife;
	short int x;
	short int y;
	short int z;
	short int alpha;
	short int beta;
	short int gamma;
	short int stage;
	short int room;
	short int lifeMode;
	short int life;
	short int field_24;
	short int anim;
	short int frame;
	short int animType;
	short int animInfo;
	short int trackMode;
	short int trackNumber;
	short int positionInTrack;

  // AITD2
  short int mark;
};

typedef struct objectStruct objectStruct;

struct boxStruct
{
	short int var0;
	short int var1;
	short int var2;
	short int var3;
};

typedef struct boxStruct boxStruct;

struct roomDefStruct
{
	short int offsetToCameraDef; // 0
	short int offsetToPosDef; // 2
	short int worldX;//4
	short int worldY;//6
	short int worldZ;//8
	short int numCameraInRoom;//0xA
};

typedef struct roomDefStruct roomDefStruct;

extern hqrEntryStruct* hqrUnk;

extern hqrEntryStruct* listMus;
extern hqrEntryStruct* listSamp;

extern int videoMode;
extern int musicConfigured;
extern int musicEnabled;

extern u8* aux;
extern u8* aux2;
extern u8* bufferAnim;

extern u8* screen;

extern int screenBufferSize;
extern int unkScreenVar2;

extern int numCVars;
extern short int* CVars;

extern u8* priority;

extern u8* fontData;

extern u8* aitdBoxGfx;

extern u8 palette[0x300];

//extern OSystem osystem;

extern u8 unkScreenVar[320*200];

extern u8 rgbaBuffer[320*200*4];

extern unsigned int timer;
extern volatile unsigned int timeGlobal;

extern int currentMenuLeft;
extern int currentMenuTop;
extern int currentMenuRight;
extern int currentMenuBottom;

extern textEntryStruct* tabTextes;
extern char* systemTextes;

extern char inputKey;
extern char input1;
extern char input2;
extern char input3;
extern char input4;
extern char button;

extern char languageNameString[];
extern char* languageNameTable[];

extern regularTextEntryStruct textTable[40];

extern int readVar;

extern int hqrKeyGen;

extern u8* screenSm1;
extern u8* screenSm2;
extern u8* screenSm3;
extern u8* screenSm4;
extern u8* screenSm5;

extern actorStruct actorTable[NUM_MAX_ACTOR];

extern short int currentCameraTarget;

extern int fileSize;

extern hqrEntryStruct* listBody;
extern hqrEntryStruct* listAnim;
extern hqrEntryStruct* listLife;
extern hqrEntryStruct* listTrack;
extern hqrEntryStruct* listMatrix;

extern short int maxObjects;

extern objectStruct* objectTable; // may be less

extern short int* vars;

extern int varSize;

extern messageStruct messageTable[5];

extern int action;

extern boxStruct genVar2[15]; // recheckSize
extern boxStruct genVar4[50];
extern boxStruct *genVar1;
extern boxStruct *genVar3;

extern int genVar5;
extern int genVar6;
extern int genVar7;
extern short int genVar9;
extern short int giveUp;
extern short int inHand;
extern short int lightVar1;
extern int lightVar2;
extern short int numObjInInventory;
extern int soundVar1;
extern int soundVar2;
extern short int statusScreenAllowed;

extern u8* etageVar0;
extern u8* etageVar1;

extern int changeFloor;
extern short int currentCamera;
extern short int currentEtage;
extern int needChangeRoom;

extern char* cameraPtr;
extern roomDefStruct* pCurrentRoomData;
extern short int currentDisplayedRoom;
extern int mainVar1;
extern int numCameraInRoom;
extern int numCameraZone;
extern u8* cameraZoneData;
extern int numRoomZone;
extern u8* roomZoneData;
extern u8* roomVar5[NUM_MAX_CAMERA_IN_ROOM];
extern int startGameVar1;

extern int transformX;
extern int transformY;
extern int transformZ;
extern int transformXCos;
extern int transformXSin;
extern int transformYCos;
extern int transformYSin;
extern int transformZCos;
extern int transformZSin;
extern bool transformUseX;
extern bool transformUseY;
extern bool transformUseZ;

extern int translateX;
extern int translateY;
extern int translateZ;

extern int cameraCenterX;
extern int cameraCenterY;
extern int cameraX;
extern int cameraY;
extern int cameraZ;

extern char currentCameraVisibilityList[30];

extern int actorTurnedToObj;

extern int currentProcessedActorIdx;
extern actorStruct* currentProcessedActorPtr;

extern int currentLifeActorIdx;
extern actorStruct* currentLifeActorPtr;
extern int currentLifeNum;

extern u8* currentLifePtr;

extern int setupCameraVar1;

#ifdef USE_GL
extern float renderPointList[6400];
#else
extern short int renderPointList[6400];
#endif

extern s32 numActorInList;
extern s32 sortedActorTable[NUM_MAX_ACTOR];

extern s32 angleCompX;
extern s32 angleCompZ;
extern s32 angleCompBeta;

extern s32 bufferAnimCounter;

extern s32 animCurrentTime;
extern s32 animKeyframeLength;
extern s32 animMoveX;
extern s32 animMoveY;
extern s32 animMoveZ;
extern s32 animRot1;
extern s32 animRot2;
extern s32 animRot3;
extern u8* animVar1;
extern u8* animVar3;
extern u8* animVar4;

extern s16 newFloor;

extern s32 paletteVar;

extern u8 cameraBuffer[256];
extern u8 cameraBuffer2[256];
extern u8 cameraBuffer3[400];
extern u8 cameraBuffer4[400];

extern u8* cameraBufferPtr;
extern u8* cameraBuffer2Ptr;
extern u8* cameraBuffer3Ptr;

extern s32 overlaySize1;
extern s32 overlaySize2;

extern s32 bgOverlayVar1;

extern s16 newRoom;

extern char* listBodySelect[];
extern char* listAnimSelect[];

extern s16 inventory[30];

extern s16 shakeVar1;
extern s16 shakingAmplitude;
extern u32 timerFreeze1;

extern hardColStruct* hardColTable[10];

extern s16 hardColVar1;
extern s16 hardColVar2;

extern ZVStruct hardClip;

extern saveEntry saveTable[];

extern s32 hqrVar1;
extern s32 mainVar3;
extern s32 mainVar2;

extern s32 clipLeft;
extern s32 clipTop;
extern s32 clipRight;
extern s32 clipBottom;

#ifdef INTERNAL_DEBUGGER
enum backgroundModeEnum
{
  backgroundModeEnum_2D,
  backgroundModeEnum_3D,
};

typedef enum backgroundModeEnum backgroundModeEnum;

extern backgroundModeEnum backgroundMode;
#endif

void freeAll(void);


#endif
