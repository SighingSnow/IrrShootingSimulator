#include "engine/irrlicht.h"
#include "sceneManager.h"
#include "api.h"

class eventManager : public irr::IEventReceiver
{
public:
    sceneManager* scene;

    struct SMouseState{
        irr::core::position2di Position;
        bool LeftButtonDown;
        SMouseState():LeftButtonDown(false){}
    }MouseState;

    virtual bool OnEvent(const irr::SEvent& event);

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
        
    }

private:
    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
};