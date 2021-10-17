#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>

class Settings
{
public:
    Settings(); //load settings
    ~Settings(); //store settings
    static Settings current;
    //data
    QString currentDB;
    QString currentPAK;
};

#endif // SETTINGS_H
