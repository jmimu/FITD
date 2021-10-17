#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "pakfile.h"
#include "db.h"

namespace Ui {
class MainWindow;
}

enum IOType
{
    IO_COMPRESSED,
    IO_DECOMPRESSED,
    IO_INTERPRETED
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString mPAKPath;
    QString mPAKname;
    PakFile mPakFile;
    DB mDB;
    void updateTable();
    bool importBMP(int index);
    bool importRaw(int index);
    bool importFloor();
    bool exportFile(int index, IOType type);
    int getSelectedIndex();

public slots:
    bool openPAK(bool init=false);
    bool savePAK();
    bool savePAKUncompressed();
    void exportAll_Compressed();
    void exportAll_Decompressed();
    void exportAll_Interpreted();

    bool importRawFile();
    bool importFile();
    bool exportSelectedFile_Compressed();
    bool exportSelectedFile_Decompressed();
    bool exportSelectedFile_Interpreted();

    bool openDB(bool init=false);
    void updateDB();
    bool writeDB();

    bool setDosboxPath();
    bool setPkZipPath();
};

#endif // MAINWINDOW_H
