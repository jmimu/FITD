#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QComboBox>
#include <cstdint>

#include "alonefloor.h"

extern "C" {
#include "../src/pak.h"
#include "../src/types.h"
}


u8 palette[256*3];


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mPAKPath("."),mPakFile(),mDB()
{
    setbuf(stdout, NULL);
    ui->setupUi(this);
    connect(this->ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(this->ui->actionOpenPAK,SIGNAL(triggered()),this,SLOT(openPAK()));
    connect(this->ui->actionSavePAK,SIGNAL(triggered()),this,SLOT(savePAK()));
    connect(this->ui->actionOverwrite_PAK_uncompressed,SIGNAL(triggered()),this,SLOT(savePAKUncompressed()));
    connect(this->ui->actionExport_all_as_BMP,SIGNAL(triggered()),this,SLOT(exportAllAsBmp()));
    connect(this->ui->action_Export,SIGNAL(triggered()),this,SLOT(exportFile()));
    connect(this->ui->action_Import,SIGNAL(triggered()),this,SLOT(importFile()));
    connect(this->ui->action_WriteDB,SIGNAL(triggered()),this,SLOT(writeDB()));
    connect(this->ui->action_OpenDB,SIGNAL(triggered()),this,SLOT(openDB()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::openDB()
{
    QString tmpfilename;
    tmpfilename=QFileDialog::getOpenFileName(this, tr("Open database file"),
                                                 "",
                                                 tr("json (*.json)"));
    if (tmpfilename=="")
        return false;

    if (mDB.read(tmpfilename.toStdString()))
    {
        updateTable();
        ui->actionOpenPAK->setEnabled(true);
        ui->action_WriteDB->setEnabled(true);
        return true;
    }
    return false;
}
void MainWindow::updateDB()
{
    //read data from tableWidget
    for (unsigned int i=0;i<mPakFile.getAllFiles().size();i++)
    {
        DBFile &file=mDB.get(mPAKname.toStdString(),i);

        QLineEdit* lineEdit=(QLineEdit*)ui->tableWidget->cellWidget(i,6);
        if (!lineEdit) return;//table not full
        file.info=lineEdit->text().toStdString();

        QComboBox* cbox=(QComboBox*)ui->tableWidget->cellWidget(i,2);
        if (!cbox) return;//table not full
        file.type=(FileType)cbox->currentIndex();
    }
    std::cout<<"DB updated."<<std::endl;
}

bool MainWindow::writeDB()
{
    updateDB();
    return mDB.overwrite();
}

void MainWindow::updateTable()
{
    ui->lineEditPAKName->setText(mPAKPath);
    ui->tableWidget->setRowCount(mPakFile.getAllFiles().size());
    ui->tableWidget->setColumnCount(7);
    QStringList hzlabels={"Offset","Name","Type","Compr","diskSize","realSize","InfoDB"};
    QStringList vertlabels;
    for (unsigned int i=0;i<mPakFile.getAllFiles().size();i++)
    {
        mDB.setDefaultCompr(mPAKname.toStdString(),i,mPakFile.getAllFiles()[i].mInfo.compressionFlag);

        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(mPakFile.getAllFiles()[i].mFileOffset, 16)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(mPakFile.getAllFiles()[i].mNameBuffer));

        DBFile &file=mDB.get(mPAKname.toStdString(),i);

        QComboBox * cbox=new QComboBox(ui->tableWidget);
        for (unsigned int j=0;j<mDB.mFileTypes.size();j++)
            cbox->addItem(mDB.mFileTypes[j].c_str());
        cbox->setCurrentIndex((int)file.type);
        //ui->tableWidget->setItem(i,2,new QTableWidgetItem(mDB.mFileTypes[file.type].c_str()));
        ui->tableWidget->setCellWidget(i,2,cbox);
        connect(cbox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateDB()));

        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(mPakFile.getAllFiles()[i].mInfo.compressionFlag, 16)));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(mPakFile.getAllFiles()[i].mInfo.discSize, 16)));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(mPakFile.getAllFiles()[i].mInfo.uncompressedSize, 16)));

        //ui->tableWidget->setItem(i,6,new QTableWidgetItem(file.info.c_str()));

        QLineEdit *lineEdit=new QLineEdit(file.info.c_str(),ui->tableWidget);
        ui->tableWidget->setCellWidget(i, 6, lineEdit);
        connect(lineEdit,SIGNAL(editingFinished()),this,SLOT(updateDB()));

        vertlabels.push_back(QString("%1").arg(i));
    }

    ui->tableWidget->setHorizontalHeaderLabels(hzlabels);
    ui->tableWidget->setVerticalHeaderLabels(vertlabels);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

bool MainWindow::openPAK()
{
    QString tmpfilename;
    tmpfilename=QFileDialog::getOpenFileName(this, tr("Open PAK file"),
                                                 mPAKPath,
                                                 tr("PAK (*.PAK)"));
    if (tmpfilename=="")
        return false;

    mPAKPath=tmpfilename;

    std::cout<<"Opening "<<mPAKPath.toStdString()<<std::endl;

    QFileInfo fileInfo(mPAKPath);
    mPAKname=fileInfo.fileName();
    std::cout<<"PAK name: "<<mPAKname.toStdString()<<std::endl;

    if (!mPakFile.read(mPAKPath.toStdString().c_str(),mPAKname.toStdString()))
        return false;

    updateTable();
    ui->actionOverwrite_PAK_uncompressed->setEnabled(true);
    ui->actionSavePAK->setEnabled(true);
    ui->actionExport_all_as_BMP->setEnabled(true);

    return true;
}

bool MainWindow::savePAK()
{
    return mPakFile.overwrite(false);
}

bool MainWindow::savePAKUncompressed()
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

bool MainWindow::exportFile()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();

    if (select->selectedRows().size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText("Please select a row!");
        msgBox.exec();
        return false;
    }

    int index=select->selectedRows()[0].row();

    //use DB type
    DBFile &file=mDB.get(mPAKname.toStdString(),index);
    std::cout<<"Export "<<mPAKname.toStdString()<<":"<<index
       <<" as "<<mDB.mFileTypes[(int)file.type]<<std::endl;


    char bufferNameOut[256+9];
    bool result=true;

    switch (file.type)
    {
    case FileType::unknown:
        //export raw
        sprintf(bufferNameOut,"%s_%d.dat",mPAKPath.toStdString().c_str(),index);
        result=mPakFile.getAllFiles().at(index).exportUncompressed(bufferNameOut);
        break;
    case FileType::text:
        //export txt
        sprintf(bufferNameOut,"%s_%d.txt",mPAKPath.toStdString().c_str(),index);
        result=mPakFile.getAllFiles().at(index).exportUncompressed(bufferNameOut);
        break;
    case FileType::image:
        //export bmp
        result=mPakFile.getAllFiles().at(index).exportAsBMP(0,320,AloneFile::palette);
        break;
    case FileType::rooms:
    case FileType::cams:
        //check that we have a rooms for file 0, and a cams for file 1
        if ((mDB.get(mPAKname.toStdString(),0).type!=FileType::rooms)
                ||(mDB.get(mPAKname.toStdString(),1).type!=FileType::cams))
        {
            QMessageBox msgBox;
            msgBox.setText("ERROR! For a floor, file 0 must be a \"Rooms\", and file 1 a \"Cameras\"!");
            msgBox.exec();
            result=false;
        }else{
            AloneFloor floor;
            floor.load(&mPakFile.getAllFiles().at(0),&mPakFile.getAllFiles().at(1));
            //TODO: save COLLADA
            result=true;
        }
        break;
    case FileType::sounds:
        //export VOX
        sprintf(bufferNameOut,"%s_%d.VOX",mPAKPath.toStdString().c_str(),index);
        result=mPakFile.getAllFiles().at(index).exportUncompressed(bufferNameOut);
        break;
    default:
        QMessageBox msgBox;
        msgBox.setText(QString("File type %1 export not implemented!").arg(mDB.mFileTypes[(int)file.type].c_str()));
        msgBox.exec();
        result=false;
        break;
    }

    if (result)
    {
        QMessageBox msgBox;
        msgBox.setText("Done!");
        msgBox.exec();
    }

    return result;
}

bool MainWindow::importFile()
{
    bool result;
    QItemSelectionModel *select = ui->tableWidget->selectionModel();

    if (select->selectedRows().size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText("Please select a row!");
        msgBox.exec();
        return false;
    }

    int index=select->selectedRows()[0].row();

    //use DB type
    DBFile &file=mDB.get(mPAKname.toStdString(),index);
    switch (file.type)
    {
    case FileType::unknown:
        //import raw
        result=importRaw(index);
        break;
    case FileType::text:
        //import txt
        result=importRaw(index);
        break;
    case FileType::image:
        //import bmp
        result=importBMP(index);
        break;
    default:
        QMessageBox msgBox;
        msgBox.setText(QString("File type %1 import not implemented!").arg(mDB.mFileTypes[(int)file.type].c_str()));
        msgBox.exec();
        return false;

    }

    if (!result) return false;

    QMessageBox msgBox;
    msgBox.setText("Done!");
    msgBox.exec();

    updateTable();

    return true;
}

bool MainWindow::importBMP(int index)
{
    printf("Import file %d\n",index);
    QString filename;
    filename=QFileDialog::getOpenFileName(this, tr("Select BMP file"),
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

    //tell to use decompressed data
    AloneFile &file=mPakFile.getAllFiles().at(index);
    pakInfoStruct &info=file.mInfo;

    delete file.mComprData;
    file.mComprData = (u8*)malloc(info.uncompressedSize);
    strncpy((char*)file.mComprData,(char*)file.mDecomprData,info.uncompressedSize);
    info.discSize=info.uncompressedSize;
    info.compressionFlag=0;

    return true;
}



bool MainWindow::importRaw(int index)
{
    printf("Import file %d\n",index);
    QString filename;
    filename=QFileDialog::getOpenFileName(this, tr("Select raw file"),
                                                 mPAKPath,
                                                 tr("RAW (*.txt *.dat)"));

    if (filename=="")
        return false;

    FILE* fileHandle=fopen(filename.toStdString().c_str(),"rb");
    fseek(fileHandle,0L,SEEK_END);
    u32 size = ftell(fileHandle);
    fseek(fileHandle,0L,SEEK_SET);

    AloneFile &file=mPakFile.getAllFiles().at(index);
    pakInfoStruct &info=file.mInfo;

    delete file.mDecomprData;
    info.uncompressedSize=size;
    file.mDecomprData = (u8*)malloc(info.uncompressedSize);
    fread(file.mDecomprData,size,1,fileHandle);

    //tell to use decompressed data

    delete file.mComprData;
    file.mComprData = (u8*)malloc(info.uncompressedSize);
    strncpy((char*)file.mComprData,(char*)file.mDecomprData,info.uncompressedSize);
    info.discSize=info.uncompressedSize;
    info.compressionFlag=0;

    fclose(fileHandle);
    return true;
}
