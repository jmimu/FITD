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

public slots:
    bool openPAK();
    bool savePAK();
    bool savePAKUncompressed();
    bool exportAllAsBmp();

    bool importFile();
    bool exportFile();

    bool openDB();
    bool writeDB();
};

#endif // MAINWINDOW_H
