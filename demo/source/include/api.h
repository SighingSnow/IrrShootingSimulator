#ifndef API_H
#define API_H

#include "engine/irrlicht.h"

const irr::io::path mediaPath = "../../resources";
const unsigned int windowHeight = 800;
const unsigned int windowWidth = 600;
const float moveSpeed = 5.0f;

static int score = 0;

// device choose
irr::video::E_DRIVER_TYPE driverChoiceConsole();

#endif