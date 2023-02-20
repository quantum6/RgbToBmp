
EXE_FILE=RgbToBmp
if [ -f ${EXE_FILE} ]; then
    rm ${EXE_FILE}
fi

JPG_FILE=Rgb-1280x720.bmp
if [ -f ${JPG_FILE} ]; then
    rm ${JPG_FILE}
fi

gcc \
    ../YuvLib/YuvLibFile.c ../YuvLibBmp/YuvLibBmp.c \
    RgbToBmpApp.c \
    -o ${EXE_FILE} \
    -I ../YuvLib \
    -I ../YuvLibBmp

cd ../RgbCreater
./RgbToFile.sh

cd ../RgbToBmp
./${EXE_FILE}

