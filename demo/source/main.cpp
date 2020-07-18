#ifdef _MSC_VER
#pragma comment(lib,"Irrlicht.lib");
#endif

#include "include/engine/irrlicht.h"
#include "include/api.h"
#include "include/sceneManager.h"
#include "include/eventManager.h"
#include<iostream>

using namespace irr;

int main(void)
{
    eventManager receiver;
    sceneManager scene;

    // choose device
    video::E_DRIVER_TYPE driverType= driverChoiceConsole();
    
    // create device
    IrrlichtDevice* device = createDevice(driverType,core::dimension2d<u32>(windowHeight,windowWidth),16,false,false,false,&receiver);
    if(device == 0)
        std::cerr<<"Failed when create device"<<std::endl;

    device->setWindowCaption(L"Demo");

    // receive information about the material of a hit triangle
    const bool separateMeshBuffers = true;

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

    // create surrounding
    scene::IAnimatedMeshSceneNode* sceneNode = scene.makeSceneNode();
    scene.createSceneNodes(device);
    receiver.scene = &scene;

    // camera
    smgr->addCameraSceneNodeFPS();
    device->getCursorControl()->setVisible(true);
    
    int lastFPS = -1;
    u32 then = device->getTimer()->getTime();

    while(device->run())
    {
        const u32 now = device->getTimer()->getTime();
        const f32 frameDeltaTime = (f32)(now-then) / 1000.0f;
        then = now;
        
        int index = receiver.collideObject();
        if(index != -1){
            scene.dropSceneNode(index);
        }

        // switch case
        driver->beginScene();
        smgr->drawAll();
        driver->endScene();
    }
    // drop device

    device->drop();

    return 0;
}