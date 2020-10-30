#include "api.h"

irr::video::E_DRIVER_TYPE driverChoiceConsole() {
	video::E_DRIVER_TYPE driverType = video::EDT_SOFTWARE;

	printf("Please select the driver you want for this example:\n"\
		" (a) OpenGL 1.5\n (b) Direct3D 9.0c\n (c) Direct3D 8.1\n"\
		" (d) Burning's Software Renderer\n (e) Software Renderer\n"\
		" (f) NullDevice\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;

	switch (i)
	{
	case 'a': driverType = video::EDT_OPENGL;   break;
	case 'b': driverType = video::EDT_DIRECT3D9; break;
		//case 'c': driverType = video::EDT_DIRECT3D8; break;
	case 'd': driverType = video::EDT_BURNINGSVIDEO; break;
	case 'e': driverType = video::EDT_SOFTWARE; break;
	case 'f': driverType = video::EDT_NULL;     break;
	default: return driverType;
	}
	return driverType;
	// create device and exit if creation failed
}

