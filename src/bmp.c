#include "bmp.h"

#include "common.h"

void saveBMP(char* name, u8* pix, u8* pal, u32 w, u32 h)
{
    FILE* fHandle;
    struct BMPHeader header={2+sizeof(struct BMPHeader)+256*4+w*h,0,0,2+sizeof(struct BMPHeader)+256*4,40,w,h,1,8,0,0,1,1,256,256};
    u32 i;

/*    char bfType[2];
    u32 bfSize=0;//The size of the file in bytes
    u16 bfReserved1=0//Unused - must be zero
    u16 bfReserved2=0//Unused - must be zero
    u32 bfOffBits=0;//Offset to start of Pixel Data
    u32 biSize=40;//Header Size - Must be at least 40
    u32 biWidth=0;//Image width in pixels
    u32 biHeight=0;//Image height in pixels
    u16 biPlanes=1;//Must be 1
    u16 biBitCount=8;//Bits per pixel - 1, 4, 8, 16, 24, or 32
    u32 biCompression=0;//Compression type (0 = uncompressed)
    u32 biSizeImage=0;//Image Size - may be zero for uncompressed images
    u32 biXPelsPerMeter=1;//Preferred resolution in pixels per meter
    u32 biYPelsPerMeter=1;//Preferred resolution in pixels per meter
    u32 biClrUsed=256;//Number Color Map entries that are actually used
    u32 biClrImportant=256;//Number of significant colors
    * */
    
    u8 palBMP[256*4];
    for (i=0;i<256;i++)
    {
        palBMP[i*4]=pal[i*3+2];
        palBMP[i*4+1]=pal[i*3+1];
        palBMP[i*4+2]=pal[i*3+0];
        palBMP[i*4+3]=0;
    }
    
    u8 *pixBMP=(u8 *)malloc(w*h);
    for (i=0;i<h;i++)
        memcpy(pixBMP+i*w,pix+(h-i-1)*w,w);

    fHandle = fopen(name,"wb");
    fwrite("BM",2,1,fHandle);
    fwrite(&header,1,sizeof(header),fHandle); 
    fwrite(palBMP,1,256*4,fHandle);
    fwrite(pixBMP,1,header.biWidth*header.biHeight,fHandle);
    fclose(fHandle);


    /*fHandle = fopen("debug/palette.dat","wb");
    fwrite(pal,1,256*3,fHandle);
    fclose(fHandle);*/
}
