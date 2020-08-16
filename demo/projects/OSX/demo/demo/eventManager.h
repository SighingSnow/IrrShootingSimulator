#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H


#include "../include/irrlicht.h"
#include "sceneManager.h"
#include "api.h"
#include "../include/ICameraSceneNode.h"
using namespace irr;
class eventManager : public irr::IEventReceiver
{
public:
    IrrlichtDevice** device;

    struct SMouseState {
        irr::core::position2di Position;
        bool LeftButtonDown;
        SMouseState() :LeftButtonDown(false) {}
    }MouseState;

    virtual bool OnEvent(const irr::SEvent& event) {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
            /*
            if (event.KeyInput.Key == irr::KEY_KEY_W) {
                KeyIsDown[irr::KEY_UP] = event.KeyInput.PressedDown;
            }
            if (event.KeyInput.Key == irr::KEY_KEY_A) {
                KeyIsDown[irr::KEY_LEFT] = event.KeyInput.PressedDown;
            }
            if (event.KeyInput.Key == irr::KEY_KEY_S) {
                KeyIsDown[irr::KEY_DOWN] = event.KeyInput.PressedDown;
            }
            if (event.KeyInput.Key == irr::KEY_KEY_D) {
                KeyIsDown[irr::KEY_RIGHT] = event.KeyInput.PressedDown;
            }
            */
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

            default:
                break;
            }
        }
        return false;
    }

    // Keyboard
    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const{
        return KeyIsDown[keyCode];
    }

    // Mouse
    const SMouseState & GetMouseState(void) const{
        return MouseState;
    }

    // return the hitted object's index by sceneManager, return -1 if no objects are hit,
    int collideObject();

    // Construct
    eventManager(){
        for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
            KeyIsDown[i] = false;
        canfire = true;
        //device = 0;
    }

private:
    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
    bool canfire;
};
int eventManager::collideObject() {
    core::line3d<f32> ray;
    scene::ISceneManager* smgr = (*device)->getSceneManager();
    scene::ICameraSceneNode* camera = smgr->getActiveCamera();
    ray.start = camera->getPosition();
    ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 10000.0f;
    core::triangle3df hitTriangle;
    core::vector3df intersection;
    scene::ISceneCollisionManager* collMan = (smgr)->getSceneCollisionManager();
    scene::ISceneNode* selectedSceneNode =
        collMan->getSceneNodeAndCollisionPointFromRay(
            ray,
            intersection,
            hitTriangle,
            0,
            0);
    if (MouseState.LeftButtonDown && canfire) {
        canfire = false;
        if(selectedSceneNode)
            return selectedSceneNode->getID();
    }
    return -1;
}
#endif

