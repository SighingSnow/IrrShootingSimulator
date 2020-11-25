//
//  common.h
//  demo
//
//  Created by ËÎÍ¥í² on 2020/11/21.
//
#pragma once
#ifndef common_h
#define common_h

#include <iostream>
#include "engine/irrlicht.h"
//#include "netManager.h"
//#include "eventManager.h"

const float moveSpeed = 5.0f;
const unsigned int windowHeight = 800;
const unsigned int windowWidth = 600;
const irr::f32 reTime = 200, upTime = 200, jumpTime = 800;

using namespace irr;
using namespace gui;
using namespace core;
using namespace video;
using namespace scene;

// device choose
irr::video::E_DRIVER_TYPE driverChoiceConsole();


// TODO: The main frame will be trasplanted here
class GameData {
public:
	IrrlichtDevice* Device;
	//eventManager eventer;
	//NetManager netManager;
	irr::s32 guiActive;
	irr::s32 guiInputActive;
	irr::s32 roomActive;
	irr::s32 roomInputActive;

	//irr::s32 netEnable;

	GameData() {
		guiActive = 1;
		guiInputActive = 0;
	}

	/* including light and enviroment */
	// void loadScene();

	/* render */
	// void render();

	/* animate */
	// void animate();

	// TODO: more parameters will be added here to make the game more realistic
	// gravity
};

#endif /* common_h */
