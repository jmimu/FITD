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
    mPAKFilename("."),numberOfFiles(0)
{
    setbuf(stdout, NULL);
    ui->setupUi(this);
    connect(this->ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(this->ui->actionOpenPAK,SIGNAL(triggered()),this,SLOT(openPAK()));

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


    mPAKFilename.remove(mPAKFilename.size()-4,4);
    numberOfFiles=PAK_getNumFiles(mPAKFilename.toStdString().c_str());
    printf("Num files: %d\n",numberOfFiles);


    for (unsigned int index=0;index<numberOfFiles;index++)
    {
        char nameBuffer[256]="Yooo";

        FILE* fileHandle;
        u32 fileOffset;
        u32 additionalDescriptorSize;
        pakInfoStruct pakInfo;
        char* ptr=0;
        u32 size=0;

        char bufferName[256];
        strcpy(bufferName, mPAKFilename.toStdString().c_str());
        strcat(bufferName,".PAK");
        fileHandle = fopen(bufferName,"rb");

        if(fileHandle) // a bit stupid, should return NULL right away
        {
          fseek(fileHandle,(index+1)*4,SEEK_SET);

          if (fread(&fileOffset,4,1,fileHandle)!=1)
            printf("Error reading fileOffset!\n");
      #ifdef BIG_ENDIAN_
          fileOffset = READ_LE_U32(&fileOffset);
      #endif
          fseek(fileHandle,fileOffset,SEEK_SET);

          if (fread(&additionalDescriptorSize,4,1,fileHandle)!=1)
            printf("Error reading additionalDescriptorSize!\n");
      #ifdef BIG_ENDIAN_
          additionalDescriptorSize = READ_LE_U32(&additionalDescriptorSize);
      #endif

          readPakInfo(&pakInfo,fileHandle);


          if(pakInfo.offset)
          {
            //ASSERT(pakInfo.offset<256);

            if (fread(nameBuffer,pakInfo.offset,1,fileHandle)!=1)
              printf("Error reading nameBuffer!\n");
      #ifdef INTERNAL_DEBUGGER
            printf("Loading %s/%s\n", name,nameBuffer+2);
      #endif
          }
          else
          {
            fseek(fileHandle,pakInfo.offset,SEEK_CUR);
          }


          /*fseek(fileHandle,pakInfo.offset,SEEK_CUR);

          if(pakInfo.compressionFlag == 0) // uncompressed
          {
            size = pakInfo.discSize;
          }
          else if(pakInfo.compressionFlag == 1) // compressed
          {
            size = pakInfo.uncompressedSize;
          }
          else if(pakInfo.compressionFlag == 4)
          {
            size = pakInfo.uncompressedSize;
          }*/

          fclose(fileHandle);
          printf("%d ",index);
          printf("fileOffset: %X ",fileOffset);
          printf("additionalDescriptorSize: %X ",additionalDescriptorSize);
          printf("discSize: %X ",pakInfo.discSize);
          printf("uncompressedSize: %X ",pakInfo.uncompressedSize);
          printf("compressionFlag: %X ",pakInfo.compressionFlag);
          printf("info5: %X ",pakInfo.info5);
          printf("offset: %X\n",pakInfo.offset);


        }
    }
    return true;
}
