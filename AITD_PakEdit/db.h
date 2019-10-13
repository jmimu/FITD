#ifndef DB_H
#define DB_H

#include <string>
#include <fstream>
#include <vector>
#include <map>

#include "json/json.h"

#define UNKNOWN_COMPR 99

//be sure to initialize DB::mFileTypes with those names
enum class FileType
{
    unknown=0,
    text,
    image,
    rooms,
    cams,
    sound,
    palimage,
    body
};

struct DBFile
{
    DBFile();
    DBFile(const DBFile &o);
    bool set(Json::Value);
    std::string info;
    FileType type;//in mFileTypes
    int default_compr;
};


class DB
{
public:
    DB();
    bool read(std::string filename);
    bool overwrite();
    DBFile& get(std::string namePAK,int numFile);
    void setDefaultCompr(std::string namePAK,int numFile,int _default_compr,bool force=false);
    std::vector<std::string> mFileTypes;
    std::map<std::string,std::vector<DBFile> > mPAKs;

private:
    std::string mFilename;
};

#endif // DB_H
