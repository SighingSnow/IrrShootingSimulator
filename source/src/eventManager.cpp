#include "eventManager.h"

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
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		if (event.KeyInput.Key == KEY_KEY_Z && !KeyIsDown[KEY_KEY_Z])
			semiauto = !semiauto;
		if (event.KeyInput.Key == KEY_KEY_C && !KeyIsDown[KEY_KEY_C])
			spawn = 1;
		if (event.KeyInput.Key == KEY_KEY_R && !KeyIsDown[KEY_KEY_R] && reload == 0 && ammo != full)
			reload = reloadTime;
		if (event.KeyInput.Key == KEY_LSHIFT && !KeyIsDown[KEY_LSHIFT]) {
			u32 now = (*device)->getTimer()->getTime();
			if (now >= jumpTimer - 350) {
				jumpTimer = now + jumpTime;
				jumpvec = core::vector3df(0, 500, 0);
			}
		}
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}
	else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
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
	return false;
}

bool eventManager::allowfire() {
	s32 nowTimer = (*device)->getTimer()->getTime();
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
	scene::ISceneManager* smgr = (*device)->getSceneManager();
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
		PlaySound(TEXT("media/ak.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
		reTimer = (*device)->getTimer()->getTime() + reTime;
		upTimer = (*device)->getTimer()->getTime() + upTime;
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