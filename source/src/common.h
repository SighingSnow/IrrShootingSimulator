//
//  common.h
//  demo
//
//  Created by 宋庭聿 on 2020/11/21.
//

#ifndef common_h
#define common_h

#include <iostream>
#include <fstream>
#include "include/irrlicht.h"
#include <vector>
#include <queue>

//#include "eventManager.h"

const float moveSpeed = 5.0f;
const unsigned int windowHeight = 800;
const unsigned int windowWidth = 600;
const irr::f32 reTime = 200, upTime = 200, jumpTime = 800,scshotTime=20;


///const unsigned short SERVER_PORT = 60000;
const unsigned int MAX_CLIENT = 10;
const unsigned short SERVER_PORT=60000;

using namespace irr;
using namespace gui;
using namespace core;
using namespace video;
using namespace scene;

// device choose
irr::video::E_DRIVER_TYPE driverChoiceConsole();

void byteToFloat(unsigned char * charBuf, float* out);
void floatToBytes(float value, unsigned char*cSendBuff, int pos);

// TODO: The main frame will be trasplanted here
class GameData{
public:
    SIrrlichtCreationParameters deviceParam;
    IrrlichtDevice* Device;
    
    
    irr::s32 guiActive;
    irr::s32 guiInputActive;
    irr::s32 roomActive;
    irr::s32 roomInputActive;
    irr::s32 netEnable;
    
    int playersNum;
    
    void setDefault();
    
    /* including light and enviroment */
    //void loadScene();
    
    /* render */
    // void render();
    
    /* animate */
    // void animate();
    
    // TODO: more parameters will be added here to make the game more realistic
    // gravity
private:
    ICameraSceneNode* camera;
    
};

#endif /* common_h */
