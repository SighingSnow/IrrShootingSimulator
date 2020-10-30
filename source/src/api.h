#pragma once
#ifndef API_H
#define API_H

#include "engine/irrlicht.h"
#include<iostream>
#include<cstdio>
#include<fstream>

using namespace irr;
const unsigned int windowHeight = 1366;
const unsigned int windowWidth = 768;
const float moveSpeed = 5.0f;

const f32 reTime = 200, upTime = 200, jumpTime = 800;

static int score = 0;

// device choose
irr::video::E_DRIVER_TYPE driverChoiceConsole();


#endif