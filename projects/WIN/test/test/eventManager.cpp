#include "eventManager.h"

void eventManager::render()
{
	IVideoDriver* driver = Game->Device->getVideoDriver();
	if (!driver) {
		std::cerr << "error when render" << std::endl;
	}
	driver->beginScene();
	Game->Device->getSceneManager()->drawAll();
	Game->Device->getGUIEnvironment()->drawAll();
	driver->endScene();
}

void eventManager::CreateGUI()
{
	IGUIEnvironment *env = device->getGUIEnvironment();
	irr::video::IVideoDriver * driver = device->getVideoDriver();

	IGUIFont* font = env->getFont("fontlucida.png");
	if (font)
		env->getSkin()->setFont(font);
	env->getSkin()->setColor(EGDC_BUTTON_TEXT, video::SColor(240, 0xAA, 0xAA, 0xAA));
	env->getSkin()->setColor(EGDC_3D_HIGH_LIGHT, video::SColor(240, 0x22, 0x22, 0x22));
	env->getSkin()->setColor(EGDC_3D_FACE, video::SColor(240, 0x44, 0x44, 0x44));
	env->getSkin()->setColor(EGDC_EDITABLE, video::SColor(240, 0x44, 0x44, 0x44));
	env->getSkin()->setColor(EGDC_FOCUSED_EDITABLE, video::SColor(240, 0x54, 0x54, 0x54));
	env->getSkin()->setColor(EGDC_WINDOW, video::SColor(240, 0x66, 0x66, 0x66));

	// minimal gui size 800x600
	dimension2d<u32> dim(800, 600);
	dimension2d<u32> vdim(device->getVideoDriver()->getScreenSize());

	guix.Window = env->addWindow(rect<s32>(0, 0, dim.Width, dim.Height), false, L"IrrShootingSim");
	guix.Window->setToolTipText(L"Just have fun here");
	//guix.Window->getCloseButton()->setToolTipText ( L"Quit Quake3 Explorer" );

	guix.cover = env->addImage(driver->getTexture("media/cover.jpg"), position2d<s32>(0, 0), true, 0);

	guix.singlePlayer = env->addButton(rect<s32>(dim.Width - 500, dim.Height - 400, dim.Width - 200, dim.Height - 300), guix.Window, -1, L"Single Player");
	guix.singlePlayer->setToolTipText(L"Play immediately with AI");

	guix.multiPlayer = env->addButton(rect<s32>(dim.Width - 500, dim.Height - 200, dim.Width - 200, dim.Height - 100), guix.Window, -1, L"Multi Player");
	guix.multiPlayer->setToolTipText(L"Play with human when connected");

	env->addStaticText(L"Volumn :", rect<s32>(dim.Width - 300, 64, dim.Width - 200, 80), false, false, guix.Window, -1, false);
	guix.soundVolumn = env->addScrollBar(true, rect<s32>(dim.Width - 180, 64, dim.Width - 70, 80), guix.Window, -1);

	env->addStaticText(L"Brightness :", rect<s32>(dim.Width - 300, 30, dim.Width - 200, 45), false, false, guix.Window, -1, false);
	guix.lightVolumn = env->addScrollBar(true, rect<s32>(dim.Width - 180, 30, dim.Width - 70, 45), guix.Window, -1);
}

void eventManager::CreateRoom()
{
	IGUIEnvironment *env = device->getGUIEnvironment();
	irr::video::IVideoDriver * driver = device->getVideoDriver();

	IGUIFont* font = env->getFont("fontlucida.png");
	if (font)
		env->getSkin()->setFont(font);
	env->getSkin()->setColor(EGDC_BUTTON_TEXT, video::SColor(240, 0xAA, 0xAA, 0xAA));
	env->getSkin()->setColor(EGDC_3D_HIGH_LIGHT, video::SColor(240, 0x22, 0x22, 0x22));
	env->getSkin()->setColor(EGDC_3D_FACE, video::SColor(240, 0x44, 0x44, 0x44));
	env->getSkin()->setColor(EGDC_EDITABLE, video::SColor(240, 0x44, 0x44, 0x44));
	env->getSkin()->setColor(EGDC_FOCUSED_EDITABLE, video::SColor(240, 0x54, 0x54, 0x54));
	env->getSkin()->setColor(EGDC_WINDOW, video::SColor(240, 0x66, 0x66, 0x66));

	dimension2d<u32> dim(800, 600);
	dimension2d<u32> vdim(device->getVideoDriver()->getScreenSize());

	room.Window = env->addWindow(rect<s32>(0, 0, dim.Width, dim.Height), false, L"IrrShootingSim", 0, 1);
	room.Window->setToolTipText(L"Just have fun here");

	room.Play = env->addButton(rect<s32>(dim.Width - 250, dim.Height - 350, dim.Width - 50, dim.Height - 300), room.Window, 1, L"Play");
	room.Play->setToolTipText(L"Double Clicked Please");

}

void eventManager::SetGUIActive(s32 command)
{
	bool inputState = false;

	ICameraSceneNode * camera = device->getSceneManager()->getActiveCamera();

	switch (command)
	{
	case 0: Game->guiActive = 0; inputState = !Game->guiActive; break;
	case 1: Game->guiActive = 1; inputState = !Game->guiActive;; break;
	case 2: Game->guiActive ^= 1; inputState = !Game->guiActive; break;
	case 3:
		if (camera)
			inputState = !camera->isInputReceiverEnabled();
		break;
	}

	if (camera)
	{
		camera->setInputReceiverEnabled(inputState);
		device->getCursorControl()->setVisible(!inputState);
	}

	if (guix.Window)
	{
		guix.Window->setVisible(Game->guiActive != 0);
	}

	device->getGUIEnvironment()->setFocus(Game->guiActive ? guix.Window : 0);
}

void eventManager::SetRoomActive(s32 command)
{
	bool inputState = false;

	ICameraSceneNode * camera = device->getSceneManager()->getActiveCamera();

	switch (command)
	{
	case 0: Game->roomActive = 0; inputState = !Game->roomActive; break;
	case 1: Game->roomActive = 1; inputState = !Game->roomActive; break;
	case 2: Game->roomActive ^= 1; inputState = !Game->roomActive; break;
	case 3:
		if (camera)
			inputState = !camera->isInputReceiverEnabled();
		break;
	}

	if (camera)
	{
		camera->setInputReceiverEnabled(inputState);
		//printf("Room: inputState: %d\n",inputState);
		device->getCursorControl()->setVisible(!inputState);
	}

	if (room.Window)
	{
		room.Window->setVisible(Game->roomActive != 0);
	}

	device->getGUIEnvironment()->setFocus(Game->roomActive ? room.Window : 0);
}

void eventManager::initKeyMap(SKeyMap  *keyMap)
{
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
}

bool eventManager::OnEvent(const irr::SEvent& event) {
	if (event.EventType == EET_GUI_EVENT) {
		if (Game->guiActive) {
			if (event.GUIEvent.Caller == guix.singlePlayer && event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED) {
				SetGUIActive(0);
			}
			if (event.GUIEvent.Caller == guix.multiPlayer && event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED) {
				//TODO: open net interface here
				SetGUIActive(0);
				CreateRoom();
				SetRoomActive(1);
			}
		}
		else if (Game->roomActive) {
			if (event.GUIEvent.Caller == room.Play && event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED) {
				SetRoomActive(0);
			}
		}
	}
	else
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !Game->guiActive) {
			if (event.KeyInput.Key == KEY_KEY_Z && !KeyIsDown[KEY_KEY_Z])
				semiauto = !semiauto;
			if (event.KeyInput.Key == KEY_KEY_C && !KeyIsDown[KEY_KEY_C])
				spawn = 1;
			if (event.KeyInput.Key == KEY_KEY_R && !KeyIsDown[KEY_KEY_R] && reload == 0 && ammo != full)
				reload = reloadTime;
			if (event.KeyInput.Key == KEY_LSHIFT && !KeyIsDown[KEY_LSHIFT]) {
				u32 now = device->getTimer()->getTime();
				if (now >= jumpTimer - 350) {
					jumpTimer = now + jumpTime;
					jumpvec = core::vector3df(0, 500, 0);
				}
			}
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}
		else
			if (event.EventType == irr::EET_MOUSE_INPUT_EVENT && !Game->guiActive) {
				switch (event.MouseInput.Event)
				{
				case EMIE_LMOUSE_PRESSED_DOWN: {
					if (MouseState.LeftButtonDown == false)
						canfire = true;
					MouseState.LeftButtonDown = true;
				}
											   break;
				case EMIE_LMOUSE_LEFT_UP:
					MouseState.LeftButtonDown = false;
					break;
				case EMIE_RMOUSE_PRESSED_DOWN: {
					if (!MouseState.RightButtonDown)//on click
						scope = (scope + 1) % 3;
					MouseState.RightButtonDown = true;
				}
				case EMIE_RMOUSE_LEFT_UP:
					MouseState.RightButtonDown = false;
					break;
				default:
					break;
				}
			}
			else
				if (event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_ESCAPE && Game->guiActive) {
					SetGUIActive(3);
				}
	return false;
}

bool eventManager::allowfire() {
	s32 nowTimer = device->getTimer()->getTime();
	s32 fireRate = 125;
	if (reload)return false;
	if (!ammo) {
		reload = reloadTime;
		return false;
	}
	if (semiauto) {
		if (MouseState.LeftButtonDown && canfire) {
			canfire = false;
			return true;
		}
	}
	else {
		if (MouseState.LeftButtonDown) {
			if (canfire) {//just pressed
				//start count
				fireTimer = nowTimer + fireRate;
				canfire = false;
				return true;
			}
			else
				if (nowTimer >= fireTimer) {
					fireTimer += fireRate;
					return true;
				}
		}
	}
	return false;
}

int eventManager::collideObject() {
	core::line3d<f32> ray;
	scene::ISceneManager* smgr = device->getSceneManager();
	scene::ICameraSceneNode* camera = smgr->getActiveCamera();
	ray.start = camera->getPosition();
	ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 10000.0f;
	core::triangle3df hitTriangle;
	core::vector3df intersection;
	scene::ISceneCollisionManager* collMan = (smgr)->getSceneCollisionManager();
	scene::ISceneNode* selectedSceneNode;
	selectedSceneNode =
		collMan->getSceneNodeAndCollisionPointFromRay(
			ray,
			intersection,
			hitTriangle,
			0,
			0);
	while (selectedSceneNode && selectedSceneNode->getID() == 3) { //bypass bullethole
		ray.start = intersection;
		selectedSceneNode =
			collMan->getSceneNodeAndCollisionPointFromRay(
				ray,
				intersection,
				hitTriangle,
				0,
				0);
	}
	if (allowfire()) {
		//cout << intersection.X << " " << intersection.Y << " " << intersection.Z << endl;
		ammo--;
		//pow anim
		WeaponNode->setMD2Animation("pow");
		//sound
#ifdef _MSC_VER
		PlaySound(TEXT("media/ak.wav"), NULL, SND_FILENAME | SND_ASYNC);
#endif
		//fireline//////////////
		scene::ISceneNode* fireline = 0;
		scene::ISceneNodeAnimator* anim = 0;
		core::vector3df start = camera->getPosition();

		core::vector3df end = (camera->getTarget() - start);
		end.normalize();
		start += end * 0.25f;////out of face
		if (selectedSceneNode)
			end = intersection;
		else
			end = start + (end * camera->getFarValue());
		f32 length = (f32)(end - start).getLength();
		const f32 speed = 8.0f;
		u32 timu = (u32)(length / speed);

		///*if (player->WeaponNode)    ///weapon shift
		{
			start.Y += -5.f;
			start += (end - start).normalize() * 5;
		}
		fireline = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(5, 5), start);
		// set material flag
		fireline->setMaterialTexture(0, bulletImg);
		fireline->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		fireline->setMaterialFlag(video::EMF_LIGHTING, false);
		//anim
		anim = smgr->createFlyStraightAnimator(start, end, timu);
		fireline->addAnimator(anim);
		anim->drop();


		anim = smgr->createDeleteAnimator(timu);
		fireline->addAnimator(anim);
		anim->drop();
		/////////////////////uplift
		f32 randy;
		randy = ((((f32)(rand() % 10)) / (f32)10) - 0.5f) * 2;
		core::vector3df camrotate = camera->getRotation();
		upvec = core::vector3df((camrotate.X - 2.0f > 272.1f || camrotate.X < 271.9f) ? -2.0f : 0, 1.0f*randy, 0);
		upvec *= 2;
		reTimer = device->getTimer()->getTime() + reTime;
		upTimer = device->getTimer()->getTime() + upTime;
		////////////////////////
		if (selectedSceneNode) {
			//on shot
			scene::IMeshSceneNode* bltnode = smgr->addMeshSceneNode(bhmesh->getMesh(0), 0, 3);
			bltnode->setScale(core::vector3df(5.f));
			core::vector3df N = hitTriangle.getNormal();
			N.normalize();
			bltnode->setPosition((intersection)+N * 0.5f);

			bltnode->setMaterialFlag(video::EMF_LIGHTING, false);
			selector = smgr->createTriangleSelector(bltnode->getMesh(), bltnode);
			bltnode->setTriangleSelector(selector);

			f32 theta, phie;
			theta = atan2(N.X, N.Y) * 180.0f / 3.14159265f;
			phie = atan2(N.Z, sqrt(N.X*N.X + N.Y*N.Y)) * 180.0f / 3.14159265f;
			bltnode->setRotation(core::vector3df(phie, 0, -theta));
			scene::ISceneNodeAnimator* delAnim = smgr->createDeleteAnimator(5000);
			if (selectedSceneNode->getID() == 1)bltnode->addAnimator(delAnim);
			delAnim->drop();
			bltnode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			bltnode->setMaterialFlag(video::EMF_LIGHTING, true);
			lastbullet = bltnode;
			/////////
			printf("intersection:%.2f %.2f %.2f\n", intersection.X, intersection.Y, intersection.Z);
			printf("ammo:%d\n", ammo);
			return selectedSceneNode->getID();
		}
	}


	return -1;
}
