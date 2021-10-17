#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

#define DOSBOX_CONF_FILE "dosbox_pakedit.conf"

class Settings
{
public:
    Settings(); //load settings
    ~Settings(); //store settings
    static Settings current;
    //data
    QString currentDB;
    QString currentPAK;
    QString dosboxEXE;
    QString pkzipDir;
};

#endif // SETTINGS_H
