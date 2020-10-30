#ifdef _MSC_VER
#pragma comment(lib,"Irrlicht.lib")
#endif

#include "engine/irrlicht.h"
#include "api.h"
#include "eventManager.h"
#include "sceneManager.h"
#include<iostream>
#include<fstream>
#include<string>
#include<queue>
#include<ctime>
#include<cstdlib>

using namespace irr;
#define dropElement(x)	if (x) { x->remove(); x = 0; }
enum
{
	ID_IsNotPickable = 0,
	IDFlag_IsPickable = 1 << 0,
	IDFlag_IsHighlightable = 1 << 1
};

scene::IAnimatedMesh* tmpmesh;
scene::IAnimatedMesh* bhmesh;
scene::ITriangleSelector* selector;
f32 reTimer, upTimer ,jumpTimer;
int tgtnum;
core::vector3df upvec,jumpvec;
f32 frameDeltaTime;
int scope = 0;
int reload = 0, reloadTime = 1700;
int ammo=20 ,full=20;
bool spawn = false;
video::ITexture* bulletImg;
scene::IMeshSceneNode* lastbullet;
scene::IAnimatedMeshSceneNode* WeaponNode;
scene::IMeshSceneNode* tgtnode[10];
void DrawCur(IrrlichtDevice* device,video::IVideoDriver* driver);
void createtgt(scene::ISceneManager* smgr, video::IVideoDriver* driver, scene::IMetaTriangleSelector* selectors);


int main(void)
{
	eventManager eventer;
    sceneManager scene;
	srand((int)time(0));
    // choose device
    video::E_DRIVER_TYPE driverType= driverChoiceConsole();
    
    // create device
    IrrlichtDevice* device = createDevice(driverType,core::dimension2d<u32>(windowHeight,windowWidth),16,false,false,false,&eventer);
	//IrrlichtDevice* device = createDevice(driverType, core::dimension2d<u32>(windowHeight, windowWidth), 16, false);
    if(device == 0)
        std::cerr<<"Failed when creating device"<<std::endl;
    eventer.device = &device;
    device->setWindowCaption(L"Demo");

    // receive information about the material of a hit triangle
    //const bool separateMeshBuffers = true;

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();

	scene::IMetaTriangleSelector* selectors = smgr->createMetaTriangleSelector();
    // create surrounding
	scene::IAnimatedMesh *smesh = smgr->getMesh("media/map.obj");
	//scene::IMesh* test = smesh->getMesh(0);
	//scope png
	video::ITexture* scopeImg = driver->getTexture(windowHeight != 800U ? "media/scope1366.png" : "media/scope.png");
	bulletImg = driver->getTexture("media/fireball.bmp");
	driver->makeColorKeyTexture(scopeImg, core::position2d<s32>(windowHeight/3, windowWidth/3));
	/*load targets*/
	std::fstream fs("tgtinfo.txt", std::ios::in);
	
	fs >> tgtnum;
	selector = 0;
	tmpmesh = smgr->getMesh("media/sydney.md2");
	bhmesh = smgr->getMesh("media/bullethole.obj");
	/*
	scene::IMeshSceneNode* tgtnode[10];
	for (int i = 0; i < tgtnum; i++){
		tgtnode[i] = smgr->addMeshSceneNode(tmpmesh->getMesh(0),0,2*10+i);
		tgtnode[i]->setMaterialTexture(0, driver->getTexture("media/sydney.jpg"));
		tgtnode[i]->setScale(core::vector3df(1.5f));
		tgtnode[i]->setRotation(core::vector3df(0, i*60, 0));
		int posi[3];
		fs >> posi[0] >> posi[1] >> posi[2];
		tgtnode[i]->setPosition(core::vector3df(posi[0], posi[1], posi[2]));
		tgtnode[i]->setMaterialFlag(video::EMF_LIGHTING, true);
		selector = smgr->createTriangleSelector(tgtnode[i]->getMesh(), tgtnode[i]);
		selectors->addTriangleSelector(selector);
		tgtnode[i]->setTriangleSelector(selector);
	}
	*/
	createtgt(smgr, driver, selectors);
	/*end load targets*/	

	scene::IMeshSceneNode* snode = 0;
	if (smesh) {
		snode = smgr->addMeshSceneNode(smesh->getMesh(0), 0, IDFlag_IsPickable);
	}
	
	if (snode) {
		snode->setScale(core::vector3df(50.0f));
		snode->setPosition(core::vector3df(-70, 30, -60));
		snode->setMaterialFlag(video::EMF_LIGHTING, true);//
		
		selector = smgr->createOctreeTriangleSelector(snode->getMesh(), snode,128);
		snode->setTriangleSelector(selector);
		selectors->addTriangleSelector(selector);
	}
	scene::ILightSceneNode* light1 = smgr->addLightSceneNode(0, core::vector3df(-70, 95, -89),video::SColorf(1.0f,1.0f,1.0f),(f32)30000.0f, ID_IsNotPickable);
	scene::ILightSceneNode* light2 = smgr->addLightSceneNode(0, core::vector3df(-100, 95, -3500), video::SColorf(1.0f, 1.0f, 1.0f), (f32)30000.0f, ID_IsNotPickable);
	smgr->setAmbientLight(video::SColorf(0.2f, 0.2f, 0.2f, 0.3f));
    // camera
	SKeyMap keyMap[10];
	eventer.initKeyMap(keyMap);
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f,.3f, ID_IsNotPickable,keyMap,10 , true, 1.5f);
	camera->setPosition(core::vector3df(-10, 0, -10));
	camera->setTarget(core::vector3df(-70, 30, -60));
	video::ITexture* top = driver->getTexture("../media/skydome.jpg");
	smgr->addSkyDomeSceneNode(top);
	if (selector) {
		// const core::aabbox3d<f32>& box = camera->getBoundingBox();
		//core::vector3df radius = box.MaxEdge - box.getCenter
		scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
			selectors, camera, core::vector3df(15, 40, 15),
			core::vector3df(0, -5.5/*-10gravity*/, 0), core::vector3df(0, 21, 0));
		selector->drop(); 
		camera->addAnimator(anim);
		anim->drop(); 
	}

	///weapon node
	c8 animation[64];
	scene::IAnimatedMeshMD2* weaponMesh = (scene::IAnimatedMeshMD2*)smgr->getMesh("media/gun.md2");
	if (weaponMesh->getMeshType() == irr::scene::E_ANIMATED_MESH_TYPE::EAMT_MD2)
	{
		s32 count = weaponMesh->getAnimationCount();
		for (s32 i = 0; i != count; ++i)
		{
			printf("Animation: %s\n", weaponMesh->getAnimationName(i));
		}
	}
	WeaponNode = smgr->addAnimatedMeshSceneNode(
		weaponMesh,
		camera,
		6,
		core::vector3df(0, 0, 0),
		core::vector3df(-90, -90, 90)
	);
	WeaponNode->setMaterialFlag(video::EMF_LIGHTING,true);
	WeaponNode->setMaterialTexture(0, driver->getTexture("media/gun.jpg"));
	WeaponNode->setLoopMode(false);
	///end weapon node


    //initialize some device related and changed in-run variations
    device->getCursorControl()->setVisible(false);
	f32 defaultFOV = camera->getFOV();
    int lastFPS = -1;
    u32 then = device->getTimer()->getTime();
	int tgtcount[10];
	for (int i = 0; i < 10; i++)tgtcount[i] = 0;
	f32 formerAnimSpeed = WeaponNode->getAnimationSpeed();

	class attacher {
	private:
		queue< scene::IMeshSceneNode* > bullets;
		void popu() {      
			scene::IMeshSceneNode* oldbullet;
			oldbullet = bullets.front();
			scene::ISceneNodeAnimator* anim = smg->createDeleteAnimator(0);
			oldbullet->addAnimator(anim);
			anim->drop();
			bullets.pop();
		}
	public:
		scene::ISceneManager* smg;
		attacher() {
		}
		attacher(scene::ISceneManager* smgr) {
			smg = smgr;
		}
		void add(scene::IMeshSceneNode* newbullet) {
			if (bullets.size() >= 10) 
				popu();
			bullets.push(newbullet);
		}
		void clear() {
			int size = bullets.size();
			for (int i = 0; i < size; i++)
				popu();
		}

	};
	attacher tgtatcr[10];
	for(int i = 0; i < 10; i++) {
		tgtatcr[i].smg = smgr;
	}

    while(device->run())
    {
		//initialize
		if (lastFPS == -1) {			
			WeaponNode->setMD2Animation("pow");
		}
        const u32 now = device->getTimer()->getTime();
        frameDeltaTime = (f32)(now-then);
        then = now;
        int index = eventer.collideObject();
        if(index != -1){
            //scene.dropSceneNode(index);
				if (index >=20&&index<=29) { //hit target
					int i = index - 20;
					tgtcount[i]++;
					tgtatcr[i].add(lastbullet);
					if (tgtcount[i] == 10) {
						tgtatcr[i].clear();
						selectors->removeTriangleSelector(tgtnode[i]->getTriangleSelector());
						dropElement(tgtnode[i]);
						tgtcount[i] = -1;
					
					}
				}
			std::cout << tgtcount[0] << " " << tgtcount[1]<<std::endl;
			std::cout <<"index:"<< index << std::endl << std::endl;
        }
		if (upTimer > now) {
			core::vector3df speed;
			speed = (upvec / upTime) * (1 - (now + upTime - upTimer)*2 / upTime);
			camera->setRotation(camera->getRotation() + speed*frameDeltaTime);
		}
		if (jumpTimer > now+350) {			
			core::vector3df jspeed;
			jspeed = (jumpvec / jumpTime) * (1 - (now + jumpTime - jumpTimer) * 2 / jumpTime);
			camera->setPosition(camera->getPosition() + jspeed * frameDeltaTime);
		}
		//reload
		if (reload == 0)
			WeaponNode->setAnimationSpeed(formerAnimSpeed);
		else if (reload < 0) {
			PlaySound(TEXT("media/in.wav"), NULL, SND_FILENAME | SND_ASYNC);
			reload = 0;
			ammo = full;
		}
		else if (reload == reloadTime) {
			PlaySound(TEXT("media/out.wav"), NULL, SND_FILENAME | SND_ASYNC);
			WeaponNode->setMD2Animation("idle");
			WeaponNode->setAnimationSpeed(90);
			reload -= (int)frameDeltaTime;
		}
		else if (reload) {
			reload -= (int)frameDeltaTime;
			if (reload == 0)reload--;
		}

		if (eventer.IsKeyDown(irr::KEY_KEY_F)) {
			//tgtatcr.clear();
		}
		if (spawn)
		{
			for (int i = 0; i < 10; i++) {
				if (tgtnode[i] != 0) {
					selectors->removeTriangleSelector(tgtnode[i]->getTriangleSelector());
					dropElement(tgtnode[i]);
				}
			}
			createtgt(smgr, driver, selectors);
			spawn = 0;
		}
		if (eventer.IsKeyDown(irr::KEY_KEY_T))
		{	
			camera->setPosition(core::vector3df(0,0,0));
			//camera->setTarget(core::vector3df(-70, 30, -60));
		}
        driver->beginScene();
		smgr->drawAll();
		camera->setFOV(defaultFOV / (f32)pow(2, scope));
		//sniper cover
		if (scope) {
			driver->draw2DImage(scopeImg, core::rect<s32>(0, 0, windowHeight, windowWidth),
				core::rect<s32>(0, 0, windowHeight, windowWidth), 0, 0, true);
		}
		else DrawCur(device, driver);
		driver->endScene();

		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw tmp(L"IrrShootingSimulator - Irrlicht Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
    }
    // drop device

    device->drop();

    return 0;
}

void createtgt(scene::ISceneManager* smgr, video::IVideoDriver* driver, scene::IMetaTriangleSelector* selectors) {
	std::fstream fs("tgtinfo.txt", std::ios::in);

	fs >> tgtnum;

	for (int i = 0; i < tgtnum; i++) {
		tgtnode[i] = smgr->addMeshSceneNode(tmpmesh->getMesh(0), 0, 2 * 10 + i);
		tgtnode[i]->setMaterialTexture(0, driver->getTexture("media/sydney.jpg"));
		tgtnode[i]->setScale(core::vector3df(1.5f));
		tgtnode[i]->setRotation(core::vector3df(0, i * 60, 0));
		int posi[3];
		fs >> posi[0] >> posi[1] >> posi[2];
		tgtnode[i]->setPosition(core::vector3df(posi[0], posi[1], posi[2]));
		tgtnode[i]->setMaterialFlag(video::EMF_LIGHTING, true);
		selector = smgr->createTriangleSelector(tgtnode[i]->getMesh(), tgtnode[i]);
		selectors->addTriangleSelector(selector);
		tgtnode[i]->setTriangleSelector(selector);
	}
}

void DrawCur(IrrlichtDevice* device, video::IVideoDriver* driver) {
	s32 nowTimer = device->getTimer()->getTime();
	s32 deltaTime;
	deltaTime = ((reTimer >= nowTimer) ? (reTimer - nowTimer) : 0);//0-1000
	u32 h1 = windowHeight / 80, h2 = windowHeight / 160, hr = windowHeight / 100;
	/*L*/driver->draw2DLine(core::vector2di((windowHeight / 2 - h1) - (hr * deltaTime / reTime), windowWidth / 2),
		core::vector2di((windowHeight / 2 - h2) - (hr * deltaTime / reTime), windowWidth / 2),
		video::SColor(255, 0, 255, 0));
	/*R*/driver->draw2DLine(core::vector2di((windowHeight / 2 + h1) + (hr * deltaTime / reTime), windowWidth / 2),
		core::vector2di((windowHeight / 2 + h2) + (hr * deltaTime / reTime), windowWidth / 2),
		video::SColor(255, 0, 255, 0));
	/*S*/driver->draw2DLine(core::vector2di(windowHeight / 2, (windowWidth / 2 - h1) - (hr * deltaTime / reTime)),
		core::vector2di(windowHeight / 2, (windowWidth / 2 - h2) - (hr * deltaTime / reTime)),
		video::SColor(255, 0, 255, 0));
	/*N*/driver->draw2DLine(core::vector2di(windowHeight / 2, (windowWidth / 2 + h1) + (hr * deltaTime / reTime)),
		core::vector2di(windowHeight / 2, (windowWidth / 2 + h2) + (hr * deltaTime / reTime)),
		video::SColor(255, 0, 255, 0));
	/*C*/if (!deltaTime)
		driver->draw2DPolygon(core::vector2di(windowHeight / 2, windowWidth / 2), 1, video::SColor(255, 0, 255, 0), 8);
}
