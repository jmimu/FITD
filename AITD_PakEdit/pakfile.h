#ifndef PAKFILE_H
#define PAKFILE_H

#include <vector>

#include "alonefile.h"

class PakFile
{
public:
    PakFile();
    bool read(const char* filename);
    std::vector<AloneFile> & getAllFiles(){return mAllFiles;}
    const char* getPaKFilename(){return mPAKFilename;}
    bool overwrite(bool forceUncompressed);
protected:
    std::vector<AloneFile> mAllFiles;
    char mPAKFilename[256];
};

#endif // PAKFILE_H
