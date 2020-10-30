#pragma once
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "engine/irrlicht.h"
#include<iostream>
#include<vector>
using namespace irr;
class sceneManager
{
public:
	sceneManager();

	// store the obstacle in the scene
	std::shared_ptr<std::vector<irr::scene::ISceneNode>> nodesPtr;

	irr::scene::IAnimatedMeshSceneNode* makeSceneNode();

	// make scene nodes, you can create cube/sphere here, mesh is forbidden to use
	//void createSceneNodes(IrrlichtDevice& device);

	// drop one scene node/hide one node, after being hit
	//void dropSceneNode(int index);

};

sceneManager::sceneManager() {

}
/*
irr::scene::IAnimatedMeshSceneNode* sceneManager::makeSceneNode() {

}
*/
#endif
