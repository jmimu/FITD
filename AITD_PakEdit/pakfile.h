#ifndef PAKFILE_H
#define PAKFILE_H

#include <vector>

#include "alonefile.h"

class PakFile
{
public:
    PakFile();
    bool read(const char* filepath, std::string filename);
    std::vector<AloneFile> & getAllFiles(){return mAllFiles;}
    const char* getPaKPath(){return mPAKPath;}
    bool overwrite(bool forceUncompressed);
protected:
    std::vector<AloneFile> mAllFiles;
    char mPAKPath[256];
    std::string mPAKFilename;
};

#endif // PAKFILE_H
