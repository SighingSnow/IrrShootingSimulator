#ifdef _MSC_VER
#pragma comment(lib,"Irrlicht.lib")
#endif

#include "../include/irrlicht.h"
#include"api.h"
#include"eventManager.h"
#include"sceneManager.h"
#include<iostream>
#include<fstream>
#include<string>

using namespace irr;
enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be highlighted.  In this example, the
	// homonids can be highlighted, but the level mesh can't.
	IDFlag_IsHighlightable = 1 << 1
};
int main(void)
{
    eventManager receiver;
    sceneManager scene;

    // choose device
    video::E_DRIVER_TYPE driverType= driverChoiceConsole();
    
    // create device
    IrrlichtDevice* device = createDevice(driverType,core::dimension2d<u32>(windowHeight,windowWidth),16,false,false,false,&receiver);
	//IrrlichtDevice* device = createDevice(driverType, core::dimension2d<u32>(windowHeight, windowWidth), 16, false);
    if(device == 0)
        std::cerr<<"Failed when creating device"<<std::endl;
    receiver.device = &device;
    device->setWindowCaption(L"Demo");

    // receive information about the material of a hit triangle
    //const bool separateMeshBuffers = true;

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
	
    // create surrounding
	scene::IAnimatedMesh *smesh = smgr->getMesh("scene.obj");
	//scene::IMesh* test = smesh->getMesh(0);
	
	/*load targets*/
	std::fstream fs("tgtinfo.txt", std::ios::in);
	int tgtnum;
	fs >> tgtnum;
	scene::ITriangleSelector* selector = 0;
	scene::IAnimatedMesh *tmpmesh = smgr->getMesh("tgt.obj");
	for (int i = 0; i < tgtnum; i++) {
		scene::IMeshSceneNode* tgtnode = smgr->addOctreeSceneNode(tmpmesh->getMesh(0),0,IDFlag_IsHighlightable);
		tgtnode->setScale(core::vector3df(5.0f));
		int posi[3];
		fs >> posi[0] >> posi[1] >> posi[2];
		tgtnode->setPosition(core::vector3df(posi[0], posi[1], posi[2]));
		tgtnode->setMaterialFlag(video::EMF_LIGHTING, false);
		selector = smgr->createTriangleSelector(tgtnode->getMesh(), tgtnode);
		tgtnode->setTriangleSelector(selector);
	}

	/*end load targets*/
	
	scene::IMeshSceneNode* snode = 0;
	if (smesh) {
		snode = smgr->addOctreeSceneNode(smesh->getMesh(0), 0, IDFlag_IsPickable);
	}
	
	if (snode) {
		snode->setScale(core::vector3df(20.0f));
		snode->setPosition(core::vector3df(-70, 30, -60));
		snode->setMaterialFlag(video::EMF_LIGHTING, true);//
		
		selector = smgr->createTriangleSelector(snode->getMesh(), snode);
		snode->setTriangleSelector(selector);
	}
	scene::ILightSceneNode* light = smgr->addLightSceneNode(0, core::vector3df(-70, 200, -60),video::SColorf(1.0f,1.0f,1.0f),(f32)10000.0f, ID_IsNotPickable);
	
    // camera
	SKeyMap keyMap[10];
	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_UP;
	keyMap[1].Action = EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = KEY_KEY_W;

	keyMap[2].Action = EKA_MOVE_BACKWARD;
	keyMap[2].KeyCode = KEY_DOWN;
	keyMap[3].Action = EKA_MOVE_BACKWARD;
	keyMap[3].KeyCode = KEY_KEY_S;

	keyMap[4].Action = EKA_STRAFE_LEFT;
	keyMap[4].KeyCode = KEY_LEFT;
	keyMap[5].Action = EKA_STRAFE_LEFT;
	keyMap[5].KeyCode = KEY_KEY_A;

	keyMap[6].Action = EKA_STRAFE_RIGHT;
	keyMap[6].KeyCode = KEY_RIGHT;
	keyMap[7].Action = EKA_STRAFE_RIGHT;
	keyMap[7].KeyCode = KEY_KEY_D;

	keyMap[8].Action = EKA_JUMP_UP;
	keyMap[8].KeyCode = KEY_SPACE;
	keyMap[9].Action = EKA_CROUCH;
	keyMap[9].KeyCode = KEY_KEY_C;

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f,.08f, ID_IsNotPickable,keyMap,10 , true, .3f);
	camera->setPosition(core::vector3df(-20, 80, 60));
	camera->setTarget(core::vector3df(-70, 30, -60));

	if (selector) {
		scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selector, camera, core::vector3df(1, 5, 1),
			core::vector3df(0, -1.0/*gravity*/, 0), core::vector3df(0, 20, 0));
		selector->drop(); // As soon as we're done with the selector, drop it.
		camera->addAnimator(anim);
		anim->drop();  // And likewise, drop the animator when we're done referring to it.
	}
    
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
            //scene.dropSceneNode(index);
            if(index == 2) score++;
        }
		if(index!=-1)
			std::cout << index << std::endl;

        
		if (receiver.IsKeyDown(irr::KEY_KEY_R))
		{
			camera->setPosition(core::vector3df(-20, 80, 60));
			camera->setTarget(core::vector3df(-70, 30, -60));
		}
        
        core::stringw str = L"demo ";
        str+="score:";
        str+=score;
        device->setWindowCaption(str.c_str());
        
        // switch case
        driver->beginScene();
        smgr->drawAll();
        driver->endScene();
    }
    // drop device

    device->drop();

    return 0;
}
