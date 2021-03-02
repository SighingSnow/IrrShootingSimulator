//
//  netManager.h
//  demo
//
//  Created by 宋庭聿 on 2020/12/5.
//
#pragma once

#ifndef netManager_h
#define netManager_h

#include "common.h"
#include <string.h>
#include<string>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"  // MessageID
#include "include/irrlicht.h"

const int undefined_index = 100000;
const int dead_index = -1;
const int server_index = 0;

enum message_type
{
    sendServerInfo = 101,
    sendClientInfo,
};

#pragma pack(push,1)
struct PlayerInfo
{
    int index; /* used for locating the cash */
    int blood;
    float x;
    float y;
    float z;
    int alive;
};
#pragma pop

class NetManager{
public:
    IrrlichtDevice* device;
    
    std::vector<PlayerInfo> playersInfo;
    std::vector<IMeshSceneNode*> playersNode;
    PlayerInfo playerInfo;
    int crtClients;
    
    IMeshSceneNode* tgtnode;
    
    bool isServer;
    
    SLNet::RakPeerInterface *peer;
    SLNet::Packet *packet;
    SLNet::SystemAddress server_address;
    
    NetManager(){
        
    }
    
    /* set up */
    virtual void initNet()= 0;
    
    /* update the network status */
    virtual void updateNet() = 0;
    
    /* if it's the server, than set data to the client. vice versa */
    virtual void sendData() = 0;
    
    /* if it's the server, than receive data from the server. vice versa */
    virtual void receiveData(SLNet::BitStream &bs_in) = 0;
    
    //virtual ~netManager();
    void createPlayer(int index);
};

class Server : public NetManager{
public:
    int receive_index; // this is updating while the device run
    
    inline Server(){
        crtClients = 1;
        playerInfo.index = undefined_index;
        playerInfo.blood = 10;
    }
    
    ~Server(){
        delete this;
    }
    
    void initNet();
    
    void updateNet();
    
    void sendData();
    
    void receiveData(SLNet::BitStream &bs_in);

private:
    void processClientMessage();
};

class Client : public NetManager{
public:
    char server_ip[256];
    unsigned short server_port;
    
    inline Client():server_port(SERVER_PORT){
        crtClients = 0;
        playerInfo.index = undefined_index;
    }
    
    ~Client(){
        delete this;
    }
    
    void initNet();
    
    void updateNet();
    
    void sendData();
    
    void receiveData(SLNet::BitStream &bs_in);
    
private:
    void processServerMessage();
};

#endif /* netManager_h */
