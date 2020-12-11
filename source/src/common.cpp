//
//  common.cpp
//  demo
//
//  Created by 宋庭聿 on 2020/11/21.
//

#include "common.h"

irr::video::E_DRIVER_TYPE driverChoiceConsole() {
    video::E_DRIVER_TYPE driverType= video::EDT_SOFTWARE;

    printf("Please select the driver you want for this example:\n"\
        " (a) OpenGL 1.5\n (b) Direct3D 9.0c\n (c) Direct3D 8.1\n"\
        " (d) Burning's Software Renderer\n (e) Software Renderer\n"\
        " (f) NullDevice\n (otherKey) exit\n\n");

    char i;
    std::cin >> i;

    switch (i)
    {
        case 'a': driverType = video::EDT_OPENGL;   break;
        case 'b': driverType = video::EDT_DIRECT3D9; break;
        //case 'c': driverType = video::EDT_DIRECT3D8; break;
        case 'd': driverType = video::EDT_BURNINGSVIDEO; break;
        case 'e': driverType = video::EDT_SOFTWARE; break;
        case 'f': driverType = video::EDT_NULL;     break;
        default: return driverType;
    }
    return driverType;
    // create device and exit if creation failed
}

void GameData::setDefault()
{
    
#if defined ( _IRR_WINDOWS_ )
    deviceParam.DriverType = EDT_DIRECT3D9;
#else
    deviceParam.DriverType = EDT_OPENGL;
#endif
    
    deviceParam.WindowSize.Width = 800;
    deviceParam.WindowSize.Height = 600;
    deviceParam.Fullscreen = false;
    deviceParam.Bits = 24;
    deviceParam.ZBufferBits = 16;
    deviceParam.Vsync = false;
    deviceParam.AntiAlias = false;
    
    guiActive = 1;
    guiInputActive = 0;
    roomActive=0;
}

void floatToBytesLittle(float value, unsigned char*cSendBuff, int pos)
{
    unsigned short i = 0;
    float floatVariable = value;
    unsigned char *pdata = (unsigned char *)&floatVariable;
    for (i = 0; i < 4; i++)
    {
        cSendBuff[i + pos] = *pdata++;//float转BYTE
    }
}

void byteToFloat(unsigned char * charBuf, float* out) {
    
    unsigned char  i;
    void *pf = out;
    unsigned char * px = charBuf;

    for (i = 0; i < 4; i++){
        *((unsigned char*)pf + i) = *(px + i);
    }
}
