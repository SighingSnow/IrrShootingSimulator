#pragma once
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H


#include "common.h"
#include "netManager.h"
#include "include/ICameraSceneNode.h"
#include<ctime>
#include<cmath>
#include<queue>

/* sound */
#ifdef _WINDOWS_
#include<windows.h>
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")
#endif
/* sound> */

extern scene::IAnimatedMesh* bhmesh;
extern scene::ITriangleSelector* selector;
extern f32 reTimer, upTimer, jumpTimer,scTimer;
extern const f32 reTime, upTime, jumpTime;
extern core::vector3df upvec, jumpvec;
extern int scope;
extern int ammo, full;
extern f32 frameDeltaTime;
extern video::ITexture* bulletImg;
extern scene::IMeshSceneNode* lastbullet;
extern scene::IAnimatedMeshSceneNode* WeaponNode;
extern bool spawn;
extern int reload, reloadTime;

#define dropElement(x)    if (x) { x->remove(); x = 0; }

class attacher {
private:
    std::queue< scene::IMeshSceneNode* > bullets;
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

class GUIX{
public:
    GUIX(){
        memset(this, 0 ,sizeof(this));
    }

    void drop()
    {
        dropElement ( Window );
        dropElement ( logo );
        dropElement ( cover );
    }
    
    IGUIWindow* Window;
    
    IGUIButton* singlePlayer;
    IGUIButton* multiPlayer;
    
    IGUIImage* logo;
    IGUIImage* cover;
    
    /* Please notice that the below botton are for decorated*/
    IGUIScrollBar* soundVolumn;
    IGUIScrollBar* lightVolumn;
};

class Room{
public:
    Room(){
        memset(this,0,sizeof(this));
    }
    
    void drop(){
        dropElement(Window);
    }
    
    IGUIWindow* Window;
    IGUIButton* Play;
    IGUIButton* EnterRoom;
    
    IGUIButton* createRoom;
    IGUIButton* enterRoom;
    IGUIEditBox* test;
};


class eventManager : public IEventReceiver
{
public:
    IrrlichtDevice* device;
    GUIX guix;
    Room room;
    GameData* Game;
    NetManager* netMgr;
    Server* server;
    
    struct SMouseState {
        irr::core::position2di Position;
        bool LeftButtonDown, RightButtonDown;
        SMouseState() :LeftButtonDown(false), RightButtonDown(false) {}
    }MouseState;

    eventManager() {
        for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
            KeyIsDown[i] = false;
        canfire = true;
        semiauto = true;
        //device = 0;
        Game = new GameData();
        Game->setDefault();
        netMgr= nullptr;
    }
    
    virtual bool OnEvent(const irr::SEvent& event);

    // Keyboard
    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const {
        return KeyIsDown[keyCode];
    }

    // Mouse
    const SMouseState & GetMouseState(void) const {
        return MouseState;
    }

    // return the hitted object's index by sceneManager, return -1 if no objects are hit,
    int collideObject();
    bool allowfire();
    void initKeyMap(SKeyMap * keyMap);
    // Construct
    void CreateGUI();
    void CreateRoom();
    void SetGUIActive(s32 command);
    void SetRoomActive(s32 command);
    void render();
    
    
private:
    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
    bool canfire, semiauto;
    s32 fireTimer;
    
};


#endif
