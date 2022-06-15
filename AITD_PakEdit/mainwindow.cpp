#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QComboBox>
#include <cstdint>

#include "alonefloor.h"
#include "alonebody.h"

extern "C" {
#include "../src/pak.h"
#include "../src/types.h"
}

#include "settings.h"
#include "version.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mPAKPath("."),mPakFile(),mDB()
{
    setbuf(stdout, NULL);
    ui->setupUi(this);
    setWindowTitle("AITD PAK Editor v" EDITOR_VERSION );
    connect(this->ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(this->ui->actionOpenPAK,SIGNAL(triggered()),this,SLOT(openPAK()));
    connect(this->ui->actionSavePAK,SIGNAL(triggered()),this,SLOT(savePAK()));
    connect(this->ui->actionOverwrite_PAK_uncompressed,SIGNAL(triggered()),this,SLOT(savePAKUncompressed()));
    connect(this->ui->actionExport_All_Compressed,SIGNAL(triggered()),this,SLOT(exportAll_Compressed()));
    connect(this->ui->actionExport_All_Decompressed,SIGNAL(triggered()),this,SLOT(exportAll_Decompressed()));
    connect(this->ui->actionExport_All_Interpreted,SIGNAL(triggered()),this,SLOT(exportAll_Interpreted()));
    connect(this->ui->actionExport_Compressed,SIGNAL(triggered()),this,SLOT(exportSelectedFile_Compressed()));
    connect(this->ui->actionExport_Decompressed,SIGNAL(triggered()),this,SLOT(exportSelectedFile_Decompressed()));
    connect(this->ui->actionExport_Interpreted,SIGNAL(triggered()),this,SLOT(exportSelectedFile_Interpreted()));
    connect(this->ui->action_Import,SIGNAL(triggered()),this,SLOT(importFile()));
    connect(this->ui->action_ImportRaw,SIGNAL(triggered()),this,SLOT(importRawFile()));
    connect(this->ui->action_WriteDB,SIGNAL(triggered()),this,SLOT(writeDB()));
    connect(this->ui->action_OpenDB,SIGNAL(triggered()),this,SLOT(openDB()));

    connect(this->ui->actionSet_Dosbox_path,SIGNAL(triggered()),this,SLOT(setDosboxPath()));
    connect(this->ui->actionSet_PKZip_path,SIGNAL(triggered()),this,SLOT(setPkZipPath()));

    if (openDB(true))
        openPAK(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::setDosboxPath()
{
    QString tmpfilename;
    tmpfilename=QFileDialog::getOpenFileName(this, tr("Dosbox executable"),
                                                 Settings::current.dosboxEXE,
                                                 tr("executable (*)"));
    if (tmpfilename=="")
        return false;
    Settings::current.dosboxEXE = tmpfilename;
    return true;
}

bool MainWindow::setPkZipPath()
{
    QString tmpfilename;
    tmpfilename=QFileDialog::getExistingDirectory(this, tr("PKZip directory"),
                                                 Settings::current.pkzipDir);
    if (tmpfilename=="")
        return false;
    Settings::current.pkzipDir = tmpfilename;

    //create dosbox config
    QFile tmp(DOSBOX_CONF_FILE);
    tmp.setPermissions((QFileDevice::Permission)0x6666);
    QFile::remove(DOSBOX_CONF_FILE);
    QFile::copy("dosbox_pakedit_template.conf", DOSBOX_CONF_FILE);
    QFile tmp2(DOSBOX_CONF_FILE);
    if (tmp2.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QString toAdd = QString("mount c %1\nc:\ndel tmp.zip\npkzip -ei tmp.zip tmp.dat\nexit\n").arg(Settings::current.pkzipDir);
        tmp2.write(toAdd.toUtf8());
        tmp2.close();
    }
    return true;
}

bool MainWindow::openDB(bool init)
{
    QString tmpfilename;
    if (!init) {
        tmpfilename=QFileDialog::getOpenFileName(this, tr("Open database file"),
                                                 "",
                                                 tr("json (*.json)"));
    } else {
         tmpfilename = Settings::current.currentDB;
    }
    if (tmpfilename=="")
        return false;

    if (mDB.read(tmpfilename.toStdString()))
    {
        updateTable();
        ui->actionOpenPAK->setEnabled(true);
        ui->action_WriteDB->setEnabled(true);
        ui->lineEditDBName->setText(tmpfilename);
        Settings::current.currentDB = tmpfilename;
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
        std::cout<<"updateTable file "<<i<<std::endl;
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

bool MainWindow::openPAK(bool init)
{
    QString tmpfilename;
    if (!init) {
        tmpfilename=QFileDialog::getOpenFileName(this, tr("Open PAK file"),
                                                     mPAKPath,
                                                     tr("PAK (*.PAK)"));
    } else {
         tmpfilename = Settings::current.currentPAK;
    }

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
    ui->actionExport_All_Compressed->setEnabled(true);
    ui->actionExport_All_Decompressed->setEnabled(true);
    ui->actionExport_All_Interpreted->setEnabled(true);
    ui->actionExport_all_as_BMP->setEnabled(true);


    Settings::current.currentPAK = tmpfilename;
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

void MainWindow::exportAll_Compressed()
{
    for (unsigned int i=0;i<mPakFile.getAllFiles().size();i++)
    {
        if (mDB.get(mPAKname.toStdString(),i).type==FileType::cams)
            continue;//export only rooms, cams will go with it
        exportFile(i,IO_COMPRESSED);
    }
    QMessageBox msgBox;
    msgBox.setText("Done!");
    msgBox.exec();
}

void MainWindow::exportAll_Decompressed()
{
    for (unsigned int i=0;i<mPakFile.getAllFiles().size();i++)
    {
        if (mDB.get(mPAKname.toStdString(),i).type==FileType::cams)
            continue;//export only rooms, cams will go with it
        exportFile(i,IO_DECOMPRESSED);
    }
    QMessageBox msgBox;
    msgBox.setText("Done!");
    msgBox.exec();
}

void MainWindow::exportAll_Interpreted()
{
    for (unsigned int i=0;i<mPakFile.getAllFiles().size();i++)
    {
        if (mDB.get(mPAKname.toStdString(),i).type==FileType::cams)
            continue;//export only rooms, cams will go with it
        exportFile(i,IO_INTERPRETED);
    }
    QMessageBox msgBox;
    msgBox.setText("Done!");
    msgBox.exec();
}

int MainWindow::getSelectedIndex()
{
    QItemSelectionModel *select = ui->tableWidget->selectionModel();

    if (select->selectedRows().size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText("Please select a row!");
        msgBox.exec();
        return -1;
    }

    int index=select->selectedRows()[0].row();
    return index;
}

bool MainWindow::exportSelectedFile_Compressed()
{
    int index=getSelectedIndex();
    if (exportFile(index,IO_COMPRESSED))
    {
        QMessageBox msgBox;
        msgBox.setText("Done!");
        msgBox.exec();
        return true;
    }
    return false;
}
bool MainWindow::exportSelectedFile_Decompressed()
{
    int index=getSelectedIndex();
    if (exportFile(index,IO_DECOMPRESSED))
    {
        QMessageBox msgBox;
        msgBox.setText("Done!");
        msgBox.exec();
        return true;
    }
    return false;
}
bool MainWindow::exportSelectedFile_Interpreted()
{
    int index=getSelectedIndex();
    if (exportFile(index,IO_INTERPRETED))
    {
        QMessageBox msgBox;
        msgBox.setText("Done!");
        msgBox.exec();
        return true;
    }
    return false;
}

bool MainWindow::exportFile(int index, IOType type)
{
    //use DB type
    DBFile &file=mDB.get(mPAKname.toStdString(),index);
    std::cout<<"Export "<<mPAKname.toStdString()<<":"<<index
       <<" as "<<mDB.mFileTypes[(int)file.type]<<std::endl;


    char bufferNameOut[256+9];
    bool result=true;
    AloneFile &alonefile=mPakFile.getAllFiles().at(index);

    switch (type) {
    case IO_COMPRESSED:
        sprintf(bufferNameOut,"%s_%d.arc",mPAKPath.toStdString().c_str(),index);
        result=alonefile.exportCompressed(bufferNameOut);
        break;
    case IO_DECOMPRESSED:
        sprintf(bufferNameOut,"%s_%d.dat",mPAKPath.toStdString().c_str(),index);
        result=alonefile.exportUncompressed(bufferNameOut);
        break;
    case IO_INTERPRETED:
        switch (file.type)
        {
        case FileType::unknown:
            //export raw
            sprintf(bufferNameOut,"%s_%d.dat",mPAKPath.toStdString().c_str(),index);
            result=alonefile.exportUncompressed(bufferNameOut);
            break;
        case FileType::text:
            //export txt
            sprintf(bufferNameOut,"%s_%d.txt",mPAKPath.toStdString().c_str(),index);
            result=alonefile.exportUncompressed(bufferNameOut);
            break;
        case FileType::image:
            //export bmp
            result=alonefile.exportAsBMP(0,320,AloneFile::palette);
            break;
        case FileType::palimage:
            //export bmp with own palette
            printf("Two first bytes: %X %X\n",alonefile.mDecomprData[0],alonefile.mDecomprData[1]);
            result=alonefile.exportAsBMP(770,320,alonefile.mDecomprData+2);
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
                result=floor.load(&mPakFile.getAllFiles().at(0),&mPakFile.getAllFiles().at(1));
            }
            break;
        case FileType::sound:
            //export VOX
            sprintf(bufferNameOut,"%s_%d.VOX",mPAKPath.toStdString().c_str(),index);
            result=alonefile.exportUncompressed(bufferNameOut);
            break;
        case FileType::body:
        //export ply
        {
            AloneBody body(&alonefile);
            result=body.load();
        }
        break;
        default:
            QMessageBox msgBox;
            msgBox.setText(QString("File type %1 export not implemented!").arg(mDB.mFileTypes[(int)file.type].c_str()));
            msgBox.exec();
            result=false;
            break;
        }

        break;
    }


    return result;
}



bool MainWindow::importRawFile()
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
    result=importRaw(index);

    if (!result) return false;

    QMessageBox msgBox;
    msgBox.setText("Done!");
    msgBox.exec();

    updateTable();

    return true;
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
    case FileType::text:
    case FileType::sound:
        //import raw
        result=importRaw(index);
        break;
    case FileType::image:
        //import bmp
        result=importBMP(index);
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
            result=importFloor();
        }
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

bool MainWindow::importFloor()
{
    printf("Import file\n");
    QString filename;
    filename=QFileDialog::getOpenFileName(this, tr("Select Collada file"),
                                                 mPAKPath,
                                                 tr("Collada (*.dae)"));
    if (filename=="")
        return false;

    AloneFloor floor;
    if (floor.load(&mPakFile.getAllFiles().at(0),&mPakFile.getAllFiles().at(1)))
        return floor.importCollada(filename.toStdString().c_str(),&mPakFile.getAllFiles().at(0),&mPakFile.getAllFiles().at(1));
    else
        return false;
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
    memcpy((char*)file.mComprData,(char*)file.mDecomprData,info.uncompressedSize);
    info.discSize=info.uncompressedSize;
    info.compressionFlag=0;

    return true;
}



bool MainWindow::importRaw(int index)
{
    printf("Import file %d\n",index);
    QString filename;
    filename=QFileDialog::getOpenFileName(this, tr("Select file"),
                                                 mPAKPath,
                                                 tr("RAW (*.txt *.dat *.VOX)"));

    if (filename=="")
        return false;

    DBFile &dbfile=mDB.get(mPAKname.toStdString(),index);
    AloneFile &file=mPakFile.getAllFiles().at(index);
    pakInfoStruct &info=file.mInfo;

    bool compress=(dbfile.default_compr==1);
    if (compress)
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Compress?", "Do you want to compress file?",
                                    QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) compress = false;
    }

    FILE* fileHandle=fopen(filename.toStdString().c_str(),"rb");
    fseek(fileHandle,0L,SEEK_END);
    u32 size = ftell(fileHandle);
    fseek(fileHandle,0L,SEEK_SET);

    char* data=(char*)malloc(size);
    fread(data,size,1,fileHandle);
    fclose(fileHandle);

    if (dbfile.type==FileType::sound)
    {
        //check VOX header
        if (strncmp("Creative Voice File",data,19)!=0)
        {
            QMessageBox msgBox;
            msgBox.setText("Sound file must be in VOX format!");
            msgBox.exec();
            free(data);
            return false;
        }
    }

    delete file.mDecomprData;
    info.uncompressedSize=size;
    file.mDecomprData = (u8*)malloc(info.uncompressedSize);
    memcpy(file.mDecomprData,(u8*)data,size);

    if (compress)
    {
        compress_status res = file.compress_dosbox_pkzip();
        QMessageBox msgBox;
        switch (res) {
        case ok:
        //    msgBox.setText("Data compression successful!");
        //    msgBox.exec();
            break;
        case nodosbox:
            msgBox.setText("Dosbox not found. Please set dosbox path.");
            msgBox.exec();
            free(data);
            return false;
            break;
        case nozip:
            msgBox.setText("No zip file created. Please check PKZip path.");
            msgBox.exec();
            free(data);
            return false;
            break;
        case notimplode:
            msgBox.setText("Wrong zip file compression (file too small?).");
            msgBox.exec();
            free(data);
            return false;
            break;
        default:
            msgBox.setText("Data compression: unknown error");
            msgBox.exec();
            free(data);
            return false;
        }
    } else {
        //tell to use decompressed data
        delete file.mComprData;
        file.mComprData = (u8*)malloc(info.uncompressedSize);
        memcpy((char*)file.mComprData,(char*)file.mDecomprData,info.uncompressedSize);
        info.discSize=info.uncompressedSize;
        info.compressionFlag=0;
    }

    free(data);
    return true;
}
