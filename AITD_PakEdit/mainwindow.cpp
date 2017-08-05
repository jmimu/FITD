#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <cstdint>

extern "C" {
#include "../src/pak.h"
}

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
u8 palette[256*3];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mPAKFilename("."),mPakFile()
{
    setbuf(stdout, NULL);
    ui->setupUi(this);
    connect(this->ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(this->ui->actionOpenPAK,SIGNAL(triggered()),this,SLOT(openPAK()));
    connect(this->ui->actionOverwrite_PAK,SIGNAL(triggered()),this,SLOT(overwritePAK()));
    connect(this->ui->actionOverwrite_PAK_uncompressed,SIGNAL(triggered()),this,SLOT(overwritePAKUncompressed()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openPAK()
{
    QString tmpfilename;
    tmpfilename=QFileDialog::getOpenFileName(this, tr("Open PAK file"),
                                                 mPAKFilename+".PAK",
                                                 tr("PAK (*.PAK)"));
    if (tmpfilename=="")
        return false;

    mPAKFilename=tmpfilename;

    std::cout<<"Opening "<<mPAKFilename.toStdString()<<std::endl;


    mPAKFilename.remove(mPAKFilename.size()-4,4);//TODO: change file name usage (with .PAK or not)

    if (!mPakFile.read(mPAKFilename.toStdString().c_str()))
        return false;

    ui->lineEditPAKName->setText(mPAKFilename+".PAK");
    ui->tableWidget->setRowCount(mPakFile.getAllFiles().size());
    ui->tableWidget->setColumnCount(8);
    QStringList hzlabels={"Offset","Name","NameDB","Compression","diskSize","realSize","Export","Import"};
    QStringList vertlabels;
    for (unsigned int i=0;i<mPakFile.getAllFiles().size();i++)
        vertlabels.push_back(QString("%1").arg(i));
    ui->tableWidget->setHorizontalHeaderLabels(hzlabels);
    ui->tableWidget->setVerticalHeaderLabels(vertlabels);

    return true;
}

bool MainWindow::overwritePAK()
{
    return mPakFile.overwrite(false);
}

bool MainWindow::overwritePAKUncompressed()
{
    return mPakFile.overwrite(true);
}
