#include "pakfile.h"

#include <cstdio>
#include <cstring>
#include <QFile>

PakFile::PakFile() : mAllFiles(),mPAKFilename()
{

}

bool PakFile::read(const char* filename)
{
    mAllFiles.clear();

    char bufferName[256];
    strcpy(bufferName, filename);
    strcat(bufferName,".PAK");
    strcpy(mPAKFilename, bufferName);

    FILE* fileHandle = fopen(bufferName,"rb");

    if(!fileHandle)
    {
        printf("Error opening file %s\n",bufferName);
        return false;
    }

    unsigned int numberOfFiles=PAK_getNumFiles(filename);
    printf("Num files: %d\n",numberOfFiles);

    mAllFiles.resize(numberOfFiles);
    for (unsigned int index=0;index<numberOfFiles;index++)
    {
        if (!mAllFiles[index].read(fileHandle,mPAKFilename,index))
        {
            printf("Error reading file %d!\n",index);
            mAllFiles.resize(index);
            return false;
        }
        mAllFiles[index].print();
    }
    return true;
}


bool PakFile::overwrite(bool forceUncompressed)
{
    char bufferNameBak[256+4];
    strcpy(bufferNameBak, mPAKFilename);
    strcat(bufferNameBak,".BAK");
    printf("Copy %s into %s\n",mPAKFilename, bufferNameBak);
    QFile::copy(mPAKFilename, bufferNameBak);

    //compute offsets
    u32 currentOffset=4*(mAllFiles.size()+1);
    for (unsigned int index=0;index<mAllFiles.size();index++)
    {
        mAllFiles[index].mFileOffset=currentOffset;
        currentOffset+=sizeof(mAllFiles[index].mAdditionalDescriptorSize);
        currentOffset+=sizeof(mAllFiles[index].mInfo);

        if ((forceUncompressed)||(mAllFiles[index].mInfo.compressionFlag==0))
            currentOffset+=mAllFiles[index].mInfo.uncompressedSize;
        else
            currentOffset+=mAllFiles[index].mInfo.discSize;

        currentOffset+=mAllFiles[index].mAdditionalDescriptorSize;//TODO: how is it working?
        currentOffset+=mAllFiles[index].mInfo.offset;//TODO: how is it working?

        currentOffset+=sizeof(mAllFiles[index].mTailingBytes);//tailing bytes
    }

    //write offsets
    FILE* fileHandle;
    fileHandle = fopen(mPAKFilename,"wb");
    u32 dummy=0;
    fwrite(&dummy,4,1,fileHandle);
    for (unsigned int index=0;index<mAllFiles.size();index++)
    {
        fwrite(&mAllFiles[index].mFileOffset,4,1,fileHandle);
    }

    //write info and uncompressed data
    for (unsigned int index=0;index<mAllFiles.size();index++)
    {
        fwrite(&mAllFiles[index].mAdditionalDescriptorSize,4,1,fileHandle);
        pakInfoStruct newInfo=mAllFiles[index].mInfo;
        if (forceUncompressed)
        {
            newInfo.compressionFlag=0;
            newInfo.discSize=newInfo.uncompressedSize;
        }
        fwrite(&newInfo,sizeof(newInfo),1,fileHandle);

        if ((forceUncompressed)||(mAllFiles[index].mInfo.compressionFlag==0))
            fwrite(mAllFiles[index].mDecomprData,newInfo.uncompressedSize,1,fileHandle);
        else
            fwrite(mAllFiles[index].mComprData,newInfo.discSize,1,fileHandle);

        //TODO: add AdditionalDescriptor?
        fwrite(mAllFiles[index].mNameBuffer,newInfo.offset,1,fileHandle);

        fwrite(mAllFiles[index].mTailingBytes,sizeof(mAllFiles[index].mTailingBytes),1,fileHandle);
    }

    return true;
}
