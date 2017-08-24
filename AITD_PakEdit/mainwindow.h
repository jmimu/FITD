#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "pakfile.h"
#include "db.h"

namespace Ui {
class MainWindow;
}


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
    bool exportFile(int index);

public slots:
    bool openPAK();
    bool savePAK();
    bool savePAKUncompressed();
    void exportAll();
    bool exportAllAsBmp();

    bool importFile();
    bool exportSelectedFile();

    bool openDB();
    void updateDB();
    bool writeDB();
};

#endif // MAINWINDOW_H
