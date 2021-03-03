#ifndef VEDIO_MANAGER_h
#define VEDIO_MANAGER_h

#ifndef STB_LIB
#define STB_LIB
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif // !STB_LIB


#include"stb_image.h"
#include"stb_image_write.h"

struct ScreenShoot
{
    unsigned char* pictureData;
    
	unsigned char* stbi_buffer;

	int out_len;

    //TODO: read file from local and turn to bitStream
    void pictureToData();
    
    //TODO: display the picture from local
    void display();
    
    //TODO: ture bitSteam to a picture
    void dataToPicture();
    
};

#endif

