#ifndef _BMP_
#define _BMP_

#include "common.h"

//info from http://www.dragonwins.com/domains/getteched/bmp/bmpfileformat.htm
struct BMPHeader
{
    char bfType[2];
    u32 bfSize;//The size of the file in bytes
    u16 bfReserved1;//Unused - must be zero
    u16 bfReserved2;//Unused - must be zero
    u32 bfOffBits;//Offset to start of Pixel Data
    u32 biSize;//Header Size - Must be at least 40
    u32 biWidth;//Image width in pixels
    u32 biHeight;//Image height in pixels
    u16 biPlanes;//Must be 1
    u16 biBitCount;//Bits per pixel - 1, 4, 8, 16, 24, or 32
    u32 biCompression;//Compression type (0 = uncompressed)
    u32 biSizeImage;//Image Size - may be zero for uncompressed images
    u32 biXPelsPerMeter;//Preferred resolution in pixels per meter
    u32 biYPelsPerMeter;//Preferred resolution in pixels per meter
    u32 biClrUsed;//Number Color Map entries that are actually used
    u32 biClrImportant;//Number of significant colors
};


void saveBMP(char* name, u8* pix, u8* pal, u32 w, u32 h);

#endif
