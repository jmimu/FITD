#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "pakfile.h"

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
    QString mPAKFilename;
    PakFile mPakFile;

public slots:
    bool openPAK();
    bool overwritePAK();
    bool overwritePAKUncompressed();

};

#endif // MAINWINDOW_H
