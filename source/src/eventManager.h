#pragma once
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H


#include "engine/irrlicht.h"
//#include "sceneManager.h"
#include "api.h"
#include "engine/ICameraSceneNode.h"
#include<cmath>

/// <sound>

#include<windows.h>
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
/// </sound>

extern scene::IAnimatedMesh* bhmesh;
extern scene::ITriangleSelector* selector;
extern f32 reTimer, upTimer, jumpTimer;
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

using namespace irr;
class eventManager : public irr::IEventReceiver
{
public:
	IrrlichtDevice** device;

	struct SMouseState {
		irr::core::position2di Position;
		bool LeftButtonDown, RightButtonDown;
		SMouseState() :LeftButtonDown(false), RightButtonDown(false) {}
	}MouseState;

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
	eventManager() {
		for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
			KeyIsDown[i] = false;
		canfire = true;
		semiauto = true;
		//device = 0;
	}

private:
	bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
	bool canfire, semiauto;
	s32 fireTimer;

	core::vector3df getvector(core::triangle3df tri) {
		core::vector3df N = tri.getNormal();
		N.normalize();
		f32 theta, phie;
		theta = atan2(N.Y, N.X);
		phie = atan2(N.Z, sqrt(N.Y*N.Y + N.X*N.X));
	}
};


#endif