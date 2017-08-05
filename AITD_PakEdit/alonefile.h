#ifndef ALONEFILE_H
#define ALONEFILE_H

#include <iostream>
#include <cstdint>

extern "C" {
#include "../src/pak.h"
#include "../src/unpack.h"

}

class AloneFile
{
public:
    AloneFile();
    ~AloneFile();
    bool read(FILE* pakfile, const char *filename, unsigned int index);
    void print();


//protected:
    pakInfoStruct mInfo;
    u32 mFileOffset;
    u32 mAdditionalDescriptorSize;
    char mNameBuffer[256];

    const char* mPAKFilename;
    int mIndex;

    u8* mComprData;
    u8* mDecomprData;

    u8 mTailingBytes[0x10];
};

#endif // ALONEFILE_H
