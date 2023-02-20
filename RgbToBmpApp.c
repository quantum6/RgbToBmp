#include "YuvLibFile.h"
#include "YuvLibBmp.h"

#define FRAME_WIDTH     1280
#define FRAME_HEIGHT     720

int main(const int argc, const char** argv)
{
    char rgbFile[FILE_NAME_SIZE] = {0};
    char bmpFile[FILE_NAME_SIZE] = {0};
    
    int dataCount = (int)(FRAME_WIDTH*FRAME_HEIGHT*3);
    char* pRgb = (char*)malloc(dataCount);

    YUV_CreateFileNameFrame(rgbFile, "../RgbCreater/Rgb", FRAME_WIDTH, FRAME_HEIGHT);
    YUV_ReadFromFile(rgbFile, pRgb, dataCount);

    YUV_CreateFileName(bmpFile, "Rgb", "bmp", FRAME_WIDTH, FRAME_HEIGHT);
    RgbToBmpFile(bmpFile, pRgb, FRAME_WIDTH, FRAME_HEIGHT);

    free(pRgb);
}

