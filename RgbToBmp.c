#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned char  	BYTE;
typedef unsigned short  WORD;
typedef unsigned int  	DWORD;
typedef int  		LONG;


#define BITS_PER_PIXCEL 24
#define FORMAT_RGBA 4
#define FORMAT_RGB  3

/** must needed. pack */
#pragma pack(1)

typedef struct
{
    WORD    bfType;
    DWORD   bfSize;
    WORD    bfReserved1;
    WORD    bfReserved2;
    DWORD   bfOffBits;
} BMP_FILE_HEADER;
 
typedef struct{
    DWORD      biSize;
    LONG       biWidth;
    LONG       biHeight;
    WORD       biPlanes;
    WORD       biBitCount;
    DWORD      biCompression;
    DWORD      biSizeImage;
    LONG       biXPelsPerMeter;
    LONG       biYPelsPerMeter;
    DWORD      biClrUsed;
    DWORD      biClrImportant;
} BMP_INFO_HEADER;

#pragma pack()


int rgbaToBmpFile(char *pFileName, unsigned char* pRgbaData, int nWidth , int nHeight)
{
    BMP_FILE_HEADER bmpHeader;
    BMP_INFO_HEADER bmpInfo;

    FILE* fp         = NULL;
    char* pBmpSource = NULL;
    char* pBmpData   = NULL;

    int i = 0, j=0;

    //4 bytes pack. must be 4 times per line。
    int bytesPerLine = (nWidth*BITS_PER_PIXCEL+31)/32*4;
    int pixcelBytes  = bytesPerLine*nHeight;

    bmpHeader.bfType        = 0x4D42;
    bmpHeader.bfReserved1   = 0;
    bmpHeader.bfReserved2   = 0;
    bmpHeader.bfOffBits     = sizeof(BMP_FILE_HEADER) + sizeof(BMP_INFO_HEADER);
    bmpHeader.bfSize        = bmpHeader.bfOffBits     + pixcelBytes;
  
    bmpInfo.biSize          = sizeof(BMP_INFO_HEADER);
    bmpInfo.biWidth         = nWidth;
    /** 这样图片才不会倒置 */
    bmpInfo.biHeight        = -nHeight; 
    bmpInfo.biPlanes        = 1;
    bmpInfo.biBitCount      = BITS_PER_PIXCEL;
    bmpInfo.biCompression   = 0;
    bmpInfo.biSizeImage     = pixcelBytes;
    bmpInfo.biXPelsPerMeter = 100;
    bmpInfo.biYPelsPerMeter = 100;
    bmpInfo.biClrUsed       = 0;
    bmpInfo.biClrImportant  = 0;
 

    /** convert in memort, then write to file. */
    pBmpSource = malloc(pixcelBytes);
    if (!pBmpSource)
    {
        return -1;
    }

    /** open file */
    fp = fopen(pFileName,"wb+");
    if (!fp)
    {
        return -1;
    }

    fwrite(&bmpHeader, sizeof(BMP_FILE_HEADER), 1, fp);
    fwrite(&bmpInfo,   sizeof(BMP_INFO_HEADER), 1, fp);
    /** Here you should consider color format. RGBA ? RGB? BGR?
        Param format is RGBA, format for file is BGR */
    pBmpData = pBmpSource;
    for (i=0; i<nHeight; i++)
    {
        for (j=0; j<nWidth; j++)
        {
           pBmpData[0] = pRgbaData[2];
           pBmpData[1] = pRgbaData[1];
           pBmpData[2] = pRgbaData[0];
	       pRgbaData += FORMAT_RGBA;
           pBmpData  += FORMAT_RGB;
        }
        //pack for 4 bytes
        pBmpData +=(bytesPerLine - nWidth*3);
    }
    fwrite(pBmpSource, pixcelBytes, 1, fp);

    /** close and release。 */
    fclose(fp);  
    free(pBmpSource);

    return 0;
}


int clipRgbaToBmpFile(char *pFileName, unsigned char* pRgbaData, int nWidth , int nHeight,
    int nClipLeft, int nClipTop, int nClipWidth, int nClipHeight)
{
    char* pClipSource     = NULL;
    char* pClipData       = NULL;
    int pixcelBytes       = nClipWidth*nClipHeight*4;
    int i = 0;

    pClipSource = malloc(pixcelBytes);
    if (!pClipSource)
    {
        return -1;
    }

    //move to right position
    pRgbaData += nClipTop * nWidth * 4;
    pRgbaData += nClipLeft * 4;

    pClipData = pClipSource;
    for (i=0; i<nClipHeight; i++)
    {
        memcpy(pClipData, pRgbaData, nClipWidth*4);
        pRgbaData += nWidth    * 4;
        pClipData += nClipWidth* 4;
    }

    rgbaToBmpFile(pFileName, pClipSource, nClipWidth, nClipHeight);

    //release
    free(pClipSource);

    return 0;
}
