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
#include "net/RakPeerInterface.h"
#include "net/MessageIdentifiers.h"
#include "net/BitStream.h"
#include "net/RakNetTypes.h"  // MessageID

enum message_type
{
    sendServerInfo = 101,
    sendClientInfo,
};

#pragma pack(push,1)
struct PlayerInfo
{
    int blood;
    float x;
    float y;
    float z;
};
#pragma pop

class NetManager{
public:
    std::vector<PlayerInfo> clientsInfo;
    PlayerInfo serverInfo;
    PlayerInfo clientInfo;
    PlayerInfo playerInfo;
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

};

class Server : public NetManager{
public:

    int maxClient;
    
    Server(){
        
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
