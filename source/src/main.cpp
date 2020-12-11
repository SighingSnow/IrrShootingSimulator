#ifdef _MSC_VER
#pragma comment(lib,"Irrlicht.lib")
#endif

#include "eventManager.h"

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
static int hitIndex = 10;
scene::IMeshSceneNode* tgtnode[MAX_CLIENT];


void DrawCur(IrrlichtDevice* device,video::IVideoDriver* driver);
void createtgt(scene::ISceneManager* smgr, video::IVideoDriver* driver, scene::IMetaTriangleSelector* selectors,std::vector<PlayerInfo> &clientsInfo);

int main(void)
{
    eventManager eventer;
    srand((int)time(0));
#ifdef _MSC_VER
    IrrlichtDevice* device = createDevice(EDT_DIRECT3D9,core::dimension2d<u32>(windowHeight,windowWidth),16,false,false,false,&eventer);
#else
    IrrlichtDevice* device = createDevice(EDT_OPENGL,core::dimension2d<u32>(windowHeight,windowWidth),16,false,false,false,&eventer);
#endif
    
    if(device == 0)
        std::cerr<<"Failed when creating device"<<std::endl;
    eventer.device = device;
    device->setWindowCaption(L"Demo");
    
    eventer.CreateGUI();
    eventer.SetGUIActive(1);
    
    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* smgr = device->getSceneManager();
    
    scene::IMetaTriangleSelector* selectors = smgr->createMetaTriangleSelector();
    // create surrounding
    scene::IAnimatedMesh *smesh = smgr->getMesh("map.obj");
    //scene::IMesh* test = smesh->getMesh(0);
    //scope png
    video::ITexture* scopeImg = driver->getTexture(windowHeight != 800U ? "scope1366.png" : "scope.png");
    bulletImg = driver->getTexture("fireball.bmp");
    driver->makeColorKeyTexture(scopeImg, core::position2d<s32>(windowHeight/3, windowWidth/3));
    /*load targets*/
    std::fstream fs("tgtinfo.txt", std::ios::in);
    
    fs >> tgtnum;
    selector = 0;
    tmpmesh = smgr->getMesh("sydney.md2");
    bhmesh = smgr->getMesh("bullethole.obj");
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
    video::ITexture* top = driver->getTexture("skydome.jpg");
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

    //weapon node
    c8 animation[64];
    scene::IAnimatedMeshMD2* weaponMesh = (scene::IAnimatedMeshMD2*)smgr->getMesh("gun.md2");
    if (weaponMesh->getMeshType() == irr::scene::E_ANIMATED_MESH_TYPE::EAMT_MD2)
    {
        s32 count = weaponMesh->getAnimationCount();
//        for (s32 i = 0; i != count; ++i)
//        {
//            printf("Animation: %s\n", weaponMesh->getAnimationName(i));
//        }
    }
    WeaponNode = smgr->addAnimatedMeshSceneNode(
        weaponMesh,
        camera,
        6,
        core::vector3df(0, 0, 0),
        core::vector3df(-90, -90, 90)
    );
    WeaponNode->setMaterialFlag(video::EMF_LIGHTING,true);
    WeaponNode->setMaterialTexture(0, driver->getTexture("gun.jpg"));
    WeaponNode->setLoopMode(false);
    //end weapon node

    //initialize some device related and changed in-run variations
    device->getCursorControl()->setVisible(true);
    f32 defaultFOV = camera->getFOV();
    int lastFPS = -1;
    u32 then = device->getTimer()->getTime();
    int tgtcount[10];
    for (int i = 0; i < 10; i++)tgtcount[i] = 0;
    f32 formerAnimSpeed = WeaponNode->getAnimationSpeed();

    
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
#ifdef _MSC_VER
            PlaySound(TEXT("in.wav"), NULL, SND_FILENAME | SND_ASYNC);
#endif
            reload = 0;
            ammo = full;
        }
        else if (reload == reloadTime) {
#ifdef _MSC_VER
            PlaySound(TEXT("out.wav"), NULL, SND_FILENAME | SND_ASYNC);
#endif
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
        
//        if (spawn)
//        {
//            for (int i = 0; i < 10; i++) {
//                if (tgtnode[i] != 0) {
//                    selectors->removeTriangleSelector(tgtnode[i]->getTriangleSelector());
//                    dropElement(tgtnode[i]);
//                }
//            }
//            createtgt(smgr, driver, selectors);
//            spawn = 0;
//        }

        if(eventer.netMgr != nullptr){
            /* send our data */
            eventer.netMgr->playerInfo.x = camera->getPosition().X;
            eventer.netMgr->playerInfo.y = camera->getPosition().Y;
            eventer.netMgr->playerInfo.z = camera->getPosition().Z;
            if(eventer.netMgr->isServer){
                eventer.netMgr->serverInfo = eventer.netMgr->playerInfo;
            }
            else{
                eventer.netMgr->clientInfo = eventer.netMgr->playerInfo;
            }
            //printf("%f %f %f\n",camera->getPosition().X,camera->getPosition().Y,camera->getPosition().Z);
            eventer.netMgr->updateNet();

            /* draw the collected data */
            if(eventer.netMgr->clientsInfo.size() > 0){
                createtgt(smgr, driver, selectors, eventer.netMgr->clientsInfo);
            }
        }
        
        
        driver->beginScene();
        smgr->drawAll();
        if(eventer.Game->guiActive || eventer.Game->roomActive) device->getGUIEnvironment()->drawAll();
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

void createtgt(scene::ISceneManager* smgr, video::IVideoDriver* driver, scene::IMetaTriangleSelector* selectors,std::vector<PlayerInfo> &clientsInfo)
{
    //printf("%d\n",clientsInfo.size());
    for (int i = 0; i < clientsInfo.size(); i++) {
        tgtnode[i] = smgr->addMeshSceneNode(tmpmesh->getMesh(0), 0, 2 * 10 + i);
        tgtnode[i]->setMaterialTexture(0, driver->getTexture("sydney.jpg"));
        tgtnode[i]->setScale(core::vector3df(1.5f));
        tgtnode[i]->setRotation(core::vector3df(0, i * 60, 0));
        tgtnode[i]->setPosition(core::vector3df(clientsInfo[i].x, clientsInfo[i].y, clientsInfo[i].z));
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
