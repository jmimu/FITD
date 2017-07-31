#include "bmp.h"

void saveBMP(char* name, u8* pix, u8* pal, u32 w, u32 h)
{
    struct BMPHeader header;
    
    header.biWidth=w;
    header.biHeight=h;
    header.bfSize=sizeof(header)+w*h+256*3;
    fHandle = fopen(name,"wb");
    fwrite(header,1,sizeof(header),fHandle); 
    fwrite(pix,1,heander.biWidth*heander.biHeight,fHandle);
    fwrite(pal,1,256*3,fHandle);
    fclose(fHandle);
}
