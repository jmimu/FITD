#include "common.h"

enum gameTypeEnum gameId;

char* currentFoundBody;
int currentFoundBodyIdx;
int statusVar1;

hqrEntryStruct* hqrUnk;

int videoMode;
int musicConfigured;
int musicEnabled;

char* aux;
char* aux2;
char* bufferAnim;

char* screen;

int screenBufferSize;
int unkScreenVar2;

int numCVars;
short int* CVars;

char* priority;

char* fontData;

char* aitdBoxGfx;

char palette[256*3];

//OSystem osystem;

char unkScreenVar[320*200];

char rgbaBuffer[320*200*4];

unsigned int timer;
volatile unsigned int timeGlobal;

int currentMenuLeft;
int currentMenuTop;
int currentMenuRight;
int currentMenuBottom;

textEntryStruct* tabTextes;
char* systemTextes;

char inputKey = 0;
char input1 = 0;
char input2 = 0;
char input3;
char input4;
char button;

char* languageNameTable[] =
{
  "FRANCAIS",
  "ITALIANO",
  "ENGLISH",
  "ESPAGNOL",
  "DEUTSCH",
  NULL,
};

char languageNameString[20] = "";

regularTextEntryStruct textTable[NUM_MAX_TEXT];

int readVar;

int hqrKeyGen = 0;

char* screenSm1;
char* screenSm2;
char* screenSm3;
char* screenSm4;
char* screenSm5;

actorStruct actorTable[NUM_MAX_ACTOR];

short int currentCameraTarget;

int fileSize;

hqrEntryStruct* listBody;
hqrEntryStruct* listAnim;
hqrEntryStruct* listLife;
hqrEntryStruct* listTrack;
hqrEntryStruct* listMatrix;

short int maxObjects;

objectStruct* objectTable;

short int* vars;

int varSize;

messageStruct messageTable[NUM_MAX_MESSAGE];

short int currentMusic;
int action;

boxStruct genVar2[15]; // recheckSize
boxStruct genVar4[50];
boxStruct *genVar1;
boxStruct *genVar3;

int genVar5;
int genVar6;
int genVar7;
int nextMusic;
short int genVar9;
short int giveUp;
short int inHand;
short int lightVar1;
int lightVar2;
short int numObjInInventory;
int soundVar1;
int soundVar2;
short int statusScreenAllowed;

char* etageVar0 = NULL;
char* etageVar1 = NULL;

int changeFloor;
short int currentCamera;
short int currentEtage;
int needChangeRoom;

char* cameraPtr;
roomDefStruct* pCurrentRoomData;

short int currentDisplayedRoom;
int mainVar1;
int numCameraInRoom;
int numCameraZone;
char* cameraZoneData;
int numRoomZone;
char* roomZoneData;
char* roomVar5[NUM_MAX_CAMERA_IN_ROOM];
int startGameVar1;

int transformX;
int transformY;
int transformZ;
int transformXCos;
int transformXSin;
int transformYCos;
int transformYSin;
int transformZCos;
int transformZSin;
bool transformUseX;
bool transformUseY;
bool transformUseZ;

int translateX;
int translateY;
int translateZ;

int cameraCenterX;
int cameraCenterY;
int cameraX;
int cameraY;
int cameraZ;

char currentCameraVisibilityList[30];

int actorTurnedToObj = 0;

int currentProcessedActorIdx;
actorStruct* currentProcessedActorPtr;

int currentLifeActorIdx;
actorStruct* currentLifeActorPtr;
int currentLifeNum;

char* currentLifePtr;

int setupCameraVar1;

#ifdef USE_GL
float renderPointList[6400];
#else
short int renderPointList[6400];
#endif

int numActorInList;
int sortedActorTable[NUM_MAX_ACTOR];

int angleCompX;
int angleCompZ;
int angleCompBeta;

int bufferAnimCounter = 0;

int animCurrentTime;
int animKeyframeLength;
int animMoveX;
int animMoveY;
int animMoveZ;
int animRot1;
int animRot2;
int animRot3;
char* animVar1;
char* animVar3;
char* animVar4;

short int newFloor;

int paletteVar;

char cameraBuffer[256];
char cameraBuffer2[256];
char cameraBuffer3[400];
char cameraBuffer4[400];

char* cameraBufferPtr = cameraBuffer;
char* cameraBuffer2Ptr = cameraBuffer2;
char* cameraBuffer3Ptr = cameraBuffer3;

int overlaySize1;
int overlaySize2;

int bgOverlayVar1;

short int newRoom;

short int inventory[INVENTORY_SIZE];

short int shakeVar1;
short int shakingAmplitude;
unsigned int timerFreeze1;

hardColStruct* hardColTable[10];

short int hardColVar1;
short int hardColVar2;

ZVStruct hardClip;

char* listBodySelect[] = {
  "LISTBODY",
  "LISTBOD2",
};

char* listAnimSelect[] = {
  "LISTANIM",
  "LISTANI2",
};

saveEntry saveTable[40];

int hqrVar1 = 0;
int mainVar3 = 4000;
int mainVar2 = -2000;

int clipLeft = 0;
int clipTop = 0;
int clipRight = 319;
int clipBottom = 119;

hqrEntryStruct* listMus;
hqrEntryStruct* listSamp;

#ifdef INTERNAL_DEBUGGER
backgroundModeEnum backgroundMode = backgroundModeEnum_2D;
#endif
