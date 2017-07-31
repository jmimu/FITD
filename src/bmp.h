#ifndef _PAK_
#define _PAK_

//info from http://www.dragonwins.com/domains/getteched/bmp/bmpfileformat.htm
struct BMPHeader
{
    char bfType[2]="BM";
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
};


void saveBMP(char* name, u8* pix, u8* pal, u32 w, u32 h);

#endif
