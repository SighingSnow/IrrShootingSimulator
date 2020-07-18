#include "engine/irrlicht.h"
#include<iostream>

class sceneManager
{
public:
    sceneManager();
    
    // store the obstacle in the scene
    std::shared_ptr<std::vector<irr::scene::ISceneNode>> nodesPtr;

    irr::scene::IAnimatedMeshSceneNode* makeSceneNode();

    // make scene nodes, you can create cube/sphere here, mesh is forbidden to use
    void createSceneNodes();

    // drop one scene node/hide one node, after being hit
    void dropSceneNode(int index);
};