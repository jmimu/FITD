#include "alonefile.h"

#include <cstring>

AloneFile::AloneFile():
    mInfo({0,0,0,0,0}),mFileOffset(0),mAdditionalDescriptorSize(0),mNameBuffer("?"),
    mPAKFilename(0),mIndex(-1),mComprData(0),mDecomprData(0)
{

}

AloneFile::~AloneFile()
{
    if (mComprData) delete mComprData;
    if (mDecomprData) delete mDecomprData;
}

bool AloneFile::read(FILE* pakfile,const char* filename,unsigned int index)
{
    mPAKFilename=filename;
    mIndex=index;

    if (mComprData) delete mComprData;
    if (mDecomprData) delete mDecomprData;

    rewind(pakfile);
    fseek(pakfile,(index+1)*4,SEEK_SET);

    if (fread(&mFileOffset,4,1,pakfile)!=1)
        printf("Error reading fileOffset!\n");
  #ifdef BIG_ENDIAN_
    fileOffset = READ_LE_U32(&fileOffset);
  #endif
    fseek(pakfile,mFileOffset,SEEK_SET);

    if (fread(&mAdditionalDescriptorSize,4,1,pakfile)!=1)
        printf("Error reading additionalDescriptorSize!\n");
  #ifdef BIG_ENDIAN_ //TODO: remove ifdef and give ref to READ_LE_U32
    mAdditionalDescriptorSize = READ_LE_U32(&mAdditionalDescriptorSize);
  #endif

    readPakInfo(&mInfo,pakfile);


    if(mInfo.offset)
    {
        if (mInfo.offset>=255)
        {
            printf("Error, nameBuffer must be < 256!\n");
            return false;
        }
        //ASSERT(pakInfo.offset<256);

        if (fread(mNameBuffer,mInfo.offset,1,pakfile)!=1)
            printf("Error reading nameBuffer!\n");
  #ifdef INTERNAL_DEBUGGER
        printf("Loading %s/%s\n", filename,mNameBuffer+2);
  #endif
    }
    /*else  //useless?
    {
        fseek(pakfile,pakInfo.offset,SEEK_CUR);
    }*/

    mComprData = (u8*)malloc(mInfo.discSize);
    if (fread(mComprData,mInfo.discSize,1,pakfile)!=1)
        printf("Error reading data!\n");

    switch(mInfo.compressionFlag)
    {
    case 0:
      {
        mInfo.uncompressedSize=mInfo.discSize;
        mDecomprData = (u8 *) malloc(mInfo.uncompressedSize);
        memcpy (mDecomprData,mComprData,mInfo.uncompressedSize);

        break;
      }
    case 1:
      {
        mDecomprData = (u8 *) malloc(mInfo.uncompressedSize);
        PAK_explode(mComprData, mDecomprData, mInfo.discSize, mInfo.uncompressedSize, mInfo.info5);
        break;
      }
    case 4:
      {
        mDecomprData = (u8 *) malloc(mInfo.uncompressedSize);
        PAK_deflate(mComprData, mDecomprData, mInfo.discSize, mInfo.uncompressedSize);
        break;
      }
    }

    return true;
}

void AloneFile::print()
{
    printf("File %s:%d ",mPAKFilename,mIndex);
    printf("fileOffset: %X ",mFileOffset);
    printf("additionalDescriptorSize: %X ",mAdditionalDescriptorSize);
    printf("discSize: %X ",mInfo.discSize);
    printf("uncompressedSize: %X ",mInfo.uncompressedSize);
    printf("compressionFlag: %X ",mInfo.compressionFlag);
    printf("info5: %X ",mInfo.info5);
    printf("offset: %X ",mInfo.offset);
    printf("nameBuffer: %s\n",mNameBuffer);
}
