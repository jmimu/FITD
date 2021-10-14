#include "pakfile.h"

#include <cstdio>
#include <cstring>
#include <QFile>

PakFile::PakFile() : mAllFiles(),mPAKPath()
{

}

bool PakFile::read(const char* filepath, std::string filename)
{
    mPAKFilename=filename;
    mAllFiles.clear();

    strcpy(mPAKPath, filepath);

    FILE* fileHandle = fopen(filepath,"rb");

    if(!fileHandle)
    {
        printf("Error opening file %s\n",filepath);
        return false;
    }

    unsigned int numberOfFiles=PAK_getNumFiles(filepath);
    printf("Num files: %d\n",numberOfFiles);

    mAllFiles.resize(numberOfFiles);
    for (unsigned int index=0;index<numberOfFiles;index++)
    {
        if (!mAllFiles[index].read(fileHandle,mPAKPath,index))
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
    strcpy(bufferNameBak, mPAKPath);
    strcat(bufferNameBak,".BAK");
    printf("Copy %s into %s\n",mPAKPath, bufferNameBak);
    QFile::copy(mPAKPath, bufferNameBak);

    //compute offsets
    u32 currentOffset=4*(mAllFiles.size()+1);
    for (unsigned int index=0;index<mAllFiles.size();index++)
    {
        mAllFiles[index].mFileOffset=currentOffset;
        mAllFiles[index].print();
        currentOffset+=sizeof(mAllFiles[index].mAdditionalDescriptorSize);
        currentOffset+=sizeof(mAllFiles[index].mInfo);

        if ((forceUncompressed)||(mAllFiles[index].mInfo.compressionFlag==0))
            currentOffset+=mAllFiles[index].mInfo.uncompressedSize;
        else
            currentOffset+=mAllFiles[index].mInfo.discSize;

        currentOffset+=mAllFiles[index].mInfo.offset;//TODO: how is it working?

        currentOffset+=sizeof(mAllFiles[index].mTailingBytes);//tailing bytes
    }

    //write offsets
    FILE* fileHandle;
    fileHandle = fopen(mPAKPath,"wb");
    if (!fileHandle)
    {
        std::cout<<"Error! Impossible to open "<<mPAKPath<<" for writing!"<<std::endl;
        return false;
    }
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
        if ((forceUncompressed)||(mAllFiles[index].mInfo.compressionFlag==0))
        {
            newInfo.compressionFlag=0;
            newInfo.discSize=newInfo.uncompressedSize;
        }
        fwrite(&newInfo,sizeof(newInfo),1,fileHandle);

        fwrite(mAllFiles[index].mNameBuffer,newInfo.offset,1,fileHandle);

        if ((forceUncompressed)||(mAllFiles[index].mInfo.compressionFlag==0))
            fwrite(mAllFiles[index].mDecomprData,newInfo.uncompressedSize,1,fileHandle);
        else
            fwrite(mAllFiles[index].mComprData,newInfo.discSize,1,fileHandle);

        //TODO: add AdditionalDescriptor?

        fwrite(mAllFiles[index].mTailingBytes,sizeof(mAllFiles[index].mTailingBytes),1,fileHandle);
    }
    fclose(fileHandle);

    return true;
}
