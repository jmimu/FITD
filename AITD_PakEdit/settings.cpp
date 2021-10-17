#include "settings.h"

Settings Settings::current;

Settings::Settings():
    currentDB(""), currentPAK("")
{
    QSettings settings("jmimu", "AITD_PakEdit");

    currentDB = settings.value("currentDB", "").toString();
    currentPAK = settings.value("currentPAK", "").toString();
    dosboxEXE = settings.value("dosboxEXE", "dosbox").toString();
    pkzipDir = settings.value("pkzipEXE", "").toString();
}

Settings::~Settings()
{
    QSettings settings("jmimu", "AITD_PakEdit");
    settings.setValue("currentDB", currentDB);
    settings.setValue("currentPAK", currentPAK);
    settings.setValue("dosboxEXE", dosboxEXE);
    settings.setValue("pkzipEXE", pkzipDir);
}
