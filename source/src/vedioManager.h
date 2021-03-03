#ifndef VEDIO_MANAGER_h
#define VEDIO_MANAGER_h

#include "stb_image.h"

struct ScreenShoot
{
    unsigned char* pictureData;
    
    //TODO: read file from local and turn to bitStream
    void pictureToData();
    
    //TODO: display the picture from local
    void display();
    
    //TODO: ture bitSteam to a picture
    void dataToPicture();
    
};

#endif

