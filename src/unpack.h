#ifndef _UNPACK_DAMIEN_H_
#define _UNPACK_DAMIEN_H_

int PAK_deflate(u8 * srcBuffer, u8 * dstBuffer, u32 compressedSize, u32 uncompressedSize);
int PAK_explode(u8 *srcBuffer, u8 *dstBuffer, u32 compressedSize, u32 uncompressedSize, u16 flags);

#endif
