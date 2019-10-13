#include "db.h"

#include <iostream>
#include <ctime>
#include <sstream>

DBFile defaultDBFile;


DBFile::DBFile():info("?"),type(FileType::unknown),default_compr(UNKNOWN_COMPR)
{

}

DBFile::DBFile(const DBFile &o):info(o.info),type(o.type),default_compr(o.default_compr)
{

}


bool DBFile::set(Json::Value v)
{
    try
    {
        info=v.get("info", "UTF-8").asString();
        type=(FileType)v.get("type", "UTF-8").asInt();
        default_compr=v.get("default_compr", "UTF-8").asInt();
    }
    catch (std::exception& e)
    {
        std::cout<<"While reading file in DB."<<std::endl;
        std::cout<<"Exception: "<<e.what()<<std::endl;
        return false;
    }
    return true;
}

DB::DB():mFileTypes({"Unknown","Text","Image","Rooms","Cameras","Sound","Palette+Image","Body"}),mFilename("noname.json")
{
}

//from http://insanecoding.blogspot.fr/2011/11/how-to-read-in-file-in-c.html
std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

bool DB::overwrite()
{
    Json::Value js_root;
    time_t t = time(0);
    struct tm * now = localtime(&t);
    std::ostringstream oss;
    oss<<(now->tm_year + 1900)<<'/'<<(now->tm_mon+1)<<'/'<<now->tm_mday;
    js_root["date"]=oss.str();

    for (int i = 0; i < (int)mFileTypes.size(); i++)
        js_root["file_types"].append(mFileTypes[i]);

    for (auto & itPAK:mPAKs)
    {
        for (int i=0; i<(int)itPAK.second.size(); ++i)
        {
            js_root["all_PAKs"][itPAK.first][std::to_string(i)]["info"]=itPAK.second[i].info;
            js_root["all_PAKs"][itPAK.first][std::to_string(i)]["type"]=(int)itPAK.second[i].type;
            js_root["all_PAKs"][itPAK.first][std::to_string(i)]["default_compr"]=itPAK.second[i].default_compr;
        }
    }


    std::ofstream fileout(mFilename);
    fileout << js_root<<std::endl;
    fileout.close();
    return true;
}

void DB::setDefaultCompr(std::string namePAK,int numFile,int _default_compr,bool force)
{
    DBFile& file=get(namePAK,numFile);
    if ((file.default_compr==UNKNOWN_COMPR)||force)
        file.default_compr=_default_compr;
}

bool DB::read(std::string filename)
{
    mFilename=filename;
    //mFileTypes.clear();
    mPAKs.clear();

    Json::Value root;
    Json::Reader reader;
    std::string jsondata;

    try
    {
        jsondata=get_file_contents(filename.c_str());
    }
    catch (std::exception& e)
    {
        std::cout<<"While reading DB file \""<<filename<<"\""<<std::endl;
        std::cout<<"Exception: "<<e.what()<<std::endl;
        return false;
    }
    catch (int e)
    {
        std::cout<<"Error num "<<e<<std::endl;
        std::cout<<"Impossible to open DB file \""<<filename<<"\""<<std::endl;
        return false;
    }


    bool parsingSuccessful = reader.parse( jsondata, root );
    if ( !parsingSuccessful )
    {
        std::cout<<"Failed to parse DB.\n"
                 <<reader.getFormattedErrorMessages()<<std::endl;
        return false;
    }

    std::cout<<"Database read."<<std::endl;


    bool conversionOK=true;
    Json::Value json_NotFound;//to test if there is an error
    Json::Value js_date;
    try
    {
        js_date=root["date"];
    }catch (std::exception& e){
        std::cout<<"Exception: "<<e.what()<<std::endl;
        std::cout<<"Error reading \"date\" node."<<std::endl;
        conversionOK=false;
    }
    std::cout<<"Date DB: "<<js_date.asString()<<std::endl;

    /*Json::Value js_types;
    try
    {
        js_types=root["file_types"];
        for (int i = 0; i < (int)js_types.size(); i++)
            mFileTypes.push_back(js_types[i].asString());
    }catch (std::exception& e){
        std::cout<<"Exception: "<<e.what()<<std::endl;
        std::cout<<"Error reading \"file_types\" node."<<std::endl;
        conversionOK=false;
    }*/

    Json::Value js_PAKs;
    try
    {
        js_PAKs=root["all_PAKs"];
        std::vector<std::string> keys = js_PAKs.getMemberNames();
        for (size_t i=0; i<keys.size(); i++)
        {
            const std::string& key = keys[i];
            //std::cout<<"Reading DB for "<<key<<"..."<<std::endl;
            Json::Value js_PAK=js_PAKs.get(key,json_NotFound);
            //std::cout<<"PAK "<<key<<" with "<<js_PAK.size()<<" files."<<std::endl;

            std::vector<DBFile> files;
            std::vector<std::string> files_keys = js_PAK.getMemberNames();
            for (size_t j=0; j<files_keys.size(); j++)
            {
                const std::string& file_key = files_keys[j];
                //std::cout<<"Reading DB for "<<key<<"/"<<file_key<<"..."<<std::endl;
                int file_key_int=atoi(file_key.c_str());
                if (file_key_int+1>(int)files.size()) files.resize(file_key_int+1);
                files[file_key_int].set(js_PAK.get(file_key,json_NotFound));
            }
            mPAKs.insert ( std::pair<std::string,std::vector<DBFile> >(key,files) );

        }

    }catch (std::exception& e){
        std::cout<<"Exception: "<<e.what()<<std::endl;
        std::cout<<"Error reading \"all_PAKs\" node."<<std::endl;
        conversionOK=false;
    }

    return conversionOK;
}

DBFile& DB::get(std::string namePAK,int numFile)
{
    auto FileIt = mPAKs.find(namePAK);
    if (FileIt!=mPAKs.end())
    {
        if ((int)FileIt->second.size()<=numFile)
        {
            //file not in DB
            FileIt->second.resize(numFile+1);
        }
        return FileIt->second.at(numFile);
    }
    //PAK not in DB
    std::vector<DBFile> files;
    files.resize(numFile+1);
    mPAKs.insert ( std::pair<std::string,std::vector<DBFile> >(namePAK,files) );
    return get(namePAK,numFile);
}
