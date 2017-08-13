#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <cstdint>

extern "C" {
#include "../src/pak.h"
#include "../src/types.h"
}


u8 palette[256*3];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mPAKPath("."),mPakFile(),mDB("AITD_PAK_DB.json")
{
    setbuf(stdout, NULL);
    ui->setupUi(this);
    connect(this->ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(this->ui->actionOpenPAK,SIGNAL(triggered()),this,SLOT(openPAK()));
    connect(this->ui->actionOverwrite_PAK,SIGNAL(triggered()),this,SLOT(overwritePAK()));
    connect(this->ui->actionOverwrite_PAK_uncompressed,SIGNAL(triggered()),this,SLOT(overwritePAKUncompressed()));
    connect(this->ui->actionExport_all_as_BMP,SIGNAL(triggered()),this,SLOT(exportAllAsBmp()));
    connect(this->ui->action_Import,SIGNAL(triggered()),this,SLOT(importFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openPAK()
{
    QString tmpfilename;
    tmpfilename=QFileDialog::getOpenFileName(this, tr("Open PAK file"),
                                                 mPAKPath+".PAK",
                                                 tr("PAK (*.PAK)"));
    if (tmpfilename=="")
        return false;

    mPAKPath=tmpfilename;

    std::cout<<"Opening "<<mPAKPath.toStdString()<<std::endl;

    QFileInfo fileInfo(mPAKPath);
    mPAKname=fileInfo.fileName();
    std::cout<<"PAK name: "<<mPAKname.toStdString()<<std::endl;


    mPAKPath.remove(mPAKPath.size()-4,4);//TODO: change file name usage (with .PAK or not)

    if (!mPakFile.read(mPAKPath.toStdString().c_str()))
        return false;

    ui->lineEditPAKName->setText(mPAKPath+".PAK");
    ui->tableWidget->setRowCount(mPakFile.getAllFiles().size());
    ui->tableWidget->setColumnCount(6);
    QStringList hzlabels={"Offset","Name","InfoDB","Compr","diskSize","realSize"};
    QStringList vertlabels;
    for (unsigned int i=0;i<mPakFile.getAllFiles().size();i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(mPakFile.getAllFiles()[i].mFileOffset, 16)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(mPakFile.getAllFiles()[i].mNameBuffer));

        DBFile file=mDB.get(mPAKname.toStdString(),i);
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(file.info.c_str()));

        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(mPakFile.getAllFiles()[i].mInfo.compressionFlag, 16)));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(mPakFile.getAllFiles()[i].mInfo.discSize, 16)));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(mPakFile.getAllFiles()[i].mInfo.uncompressedSize, 16)));
        vertlabels.push_back(QString("%1").arg(i));
    }

    ui->tableWidget->setHorizontalHeaderLabels(hzlabels);
    ui->tableWidget->setVerticalHeaderLabels(vertlabels);
    ui->tableWidget->setColumnWidth(1,150);
    ui->tableWidget->setColumnWidth(2,150);
    ui->tableWidget->setColumnWidth(3,50);


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


bool MainWindow::exportAllAsBmp()
{
    /*    QString outDir;
        outDir=QFileDialog::getExistingDirectory(this, tr("Output directory"),mPAKFilename);
        if (outDir=="")
            return false;*/

    for (unsigned int i=0;i<mPakFile.getAllFiles().size();i++)
        mPakFile.getAllFiles().at(i).exportAsBMP(0,320,AloneFile::palette);

    return true;
}

bool MainWindow::importFile()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();

    if (select->selectedRows().size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText("Please select a row!");
        msgBox.exec();
        return false;
        return false;
    }

    int index=select->selectedRows()[0].row();

    printf("Import file %d\n",index);
    QString filename;
    filename=QFileDialog::getOpenFileName(this, tr("Open BMP file"),
                                                 mPAKPath,
                                                 tr("BMP (*.BMP)"));
    if (filename=="")
        return false;

    QImage img(filename.toStdString().c_str());
    if (img.isNull())
    {
        QMessageBox msgBox;
        msgBox.setText("Problem loading image.");
        msgBox.exec();
        return false;
    }

    printf("count: %d\n",img.colorCount());
    if (img.colorCount()!=256)
    {
        QMessageBox msgBox;
        msgBox.setText("The picture must be in 256 indexed colors.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    if (img.width()*img.height()!=(int)mPakFile.getAllFiles().at(index).mInfo.uncompressedSize)
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Picture size is %1, but file size is %2...")
                       .arg(img.width()*img.height())
                       .arg(mPakFile.getAllFiles().at(index).mInfo.uncompressedSize));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    //copy values into mDecomprData
    u8* ptr=mPakFile.getAllFiles().at(index).mDecomprData;
    for (int y=0;y<img.height();y++)
        for (int x=0;x<img.width();x++)
        {
            *ptr=img.pixelIndex(x,y);
            ptr++;
        }

    QMessageBox msgBox;
    msgBox.setText("Done!");
    msgBox.exec();

    return true;
}

