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


bool PakFile::saveUncompressed()
{
    char bufferNameBak[256+4];
    strcpy(bufferNameBak, mPAKFilename);
    strcat(bufferNameBak,".BAK");
    printf("Copy %s into %s\n",mPAKFilename, bufferNameBak);
    QFile::copy(mPAKFilename, bufferNameBak);

    return true;
}
