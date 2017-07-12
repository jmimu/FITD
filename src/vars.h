#ifndef _VARS_
#define _VARS_

#include "osystem.h"

#pragma pack(1)

extern char* currentFoundBody;
extern int currentFoundBodyIdx;
extern int statusVar1;

enum gameTypeEnum
{
  AITD1,
  JACK,
  AITD2,
  AITD3,
  TIMEGATE,
};

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
  char* ptr;
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

struct ZVStruct
{
	 s32 ZVX1;
	 s32 ZVX2;
	 s32 ZVY1;
	 s32 ZVY2;
	 s32 ZVZ1;
	 s32 ZVZ2;
};

typedef struct ZVStruct ZVStruct;

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

extern char* aux;
extern char* aux2;
extern char* bufferAnim;

extern char* screen;

extern int screenBufferSize;
extern int unkScreenVar2;

extern int numCVars;
extern short int* CVars;

extern char* priority;

extern char* fontData;

extern char* aitdBoxGfx;

extern char palette[0x300];

//extern OSystem osystem;

extern char unkScreenVar[320*200];

extern char rgbaBuffer[320*200*4];

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

extern char* screenSm1;
extern char* screenSm2;
extern char* screenSm3;
extern char* screenSm4;
extern char* screenSm5;

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

extern short int currentMusic;
extern int action;

extern boxStruct genVar2[15]; // recheckSize
extern boxStruct genVar4[50];
extern boxStruct *genVar1;
extern boxStruct *genVar3;

extern int genVar5;
extern int genVar6;
extern int genVar7;
extern int nextMusic;
extern short int genVar9;
extern short int giveUp;
extern short int inHand;
extern short int lightVar1;
extern int lightVar2;
extern short int numObjInInventory;
extern int soundVar1;
extern int soundVar2;
extern short int statusScreenAllowed;

extern char* etageVar0;
extern char* etageVar1;

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
extern char* cameraZoneData;
extern int numRoomZone;
extern char* roomZoneData;
extern char* roomVar5[NUM_MAX_CAMERA_IN_ROOM];
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

extern char* currentLifePtr;

extern int setupCameraVar1;

#ifdef USE_GL
extern float renderPointList[6400];
#else
extern short int renderPointList[6400];
#endif

extern int numActorInList;
extern int sortedActorTable[NUM_MAX_ACTOR];

extern int angleCompX;
extern int angleCompZ;
extern int angleCompBeta;

extern int bufferAnimCounter;

extern int animCurrentTime;
extern int animKeyframeLength;
extern int animMoveX;
extern int animMoveY;
extern int animMoveZ;
extern int animRot1;
extern int animRot2;
extern int animRot3;
extern char* animVar1;
extern char* animVar3;
extern char* animVar4;

extern short int newFloor;

extern int paletteVar;

extern char cameraBuffer[256];
extern char cameraBuffer2[256];
extern char cameraBuffer3[400];
extern char cameraBuffer4[400];

extern char* cameraBufferPtr;
extern char* cameraBuffer2Ptr;
extern char* cameraBuffer3Ptr;

extern int overlaySize1;
extern int overlaySize2;

extern int bgOverlayVar1;

extern short int newRoom;

extern char* listBodySelect[];
extern char* listAnimSelect[];

extern short int inventory[30];

extern short int shakeVar1;
extern short int shakingAmplitude;
extern unsigned int timerFreeze1;

extern hardColStruct* hardColTable[10];

extern short int hardColVar1;
extern short int hardColVar2;

extern ZVStruct hardClip;

extern saveEntry saveTable[];

extern int hqrVar1;
extern int mainVar3;
extern int mainVar2;

extern int clipLeft;
extern int clipTop;
extern int clipRight;
extern int clipBottom;

#ifdef INTERNAL_DEBUGGER
enum backgroundModeEnum
{
  backgroundModeEnum_2D,
  backgroundModeEnum_3D,
};

typedef enum backgroundModeEnum backgroundModeEnum;

extern backgroundModeEnum backgroundMode;
#endif

#endif
