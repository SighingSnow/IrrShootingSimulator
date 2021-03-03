//
//  netManager.cpp
//  demo
//
//  Created by 宋庭聿 on 2020/12/5.
//

#include "netManager.h"

void NetManager::createPlayer(int index)
{
    if(index < playersNode.size() && index != 0){
        std::cerr<<" no need to create new player"<<std::endl;
        return;
    }
    playersNode.resize(playerInfo.index+1);
    ISceneManager* smgr = device->getSceneManager();
    scene::IAnimatedMesh* tmpmesh;
    tmpmesh = smgr->getMesh("sydney.md2");
    // TODO: Add more specific index, it's important to check collision
    playersNode[index] = smgr->addMeshSceneNode(tmpmesh);
    playersNode[index]->setScale(core::vector3df(1.5f));
    playersNode[index]->setPosition(core::vector3df(playersInfo[index].x, playersInfo[index].y, playersInfo[index].z));
    playersNode[index]->setMaterialFlag(video::EMF_LIGHTING, true);
    return;
}


void Server::initNet()
{
    peer = SLNet::RakPeerInterface::GetInstance();
    if (NULL == peer)
    {
        std::cerr << "GetInstance failed" << std::endl;
        return;
    }
 
    std::cout << "Start Game Server ......" << SERVER_PORT << std::endl;
 
    SLNet::SocketDescriptor sd(SERVER_PORT,0);
    
    int start_up = peer->Startup(MAX_CLIENT, &sd, 1);
    if (start_up > 0)
    {
        std::cerr << "Startup failed" << std::endl;
        return;
    }
    
    if(start_up == 0){
        std::cout<< "Start up succeed"<<std::endl;
    }
    
    //SetMaximumIncomingConnections容许最多的连接数量
    peer->SetMaximumIncomingConnections(MAX_CLIENT);
    
    return;
}

void Server::updateNet()
{
        for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
        {
            switch (packet->data[0])
            {
            case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                printf("Another client has disconnected.\n");
                break;
            case ID_REMOTE_CONNECTION_LOST:
                printf("Another client has lost the connection.\n");
                crtClients--;
                break;
            case ID_REMOTE_NEW_INCOMING_CONNECTION:
                printf("Another client has connected.\n");
                break;
            case ID_CONNECTION_REQUEST_ACCEPTED:
                printf("New connection request from a client has been accepted.\n");
                sendData();
                break;
            case ID_NEW_INCOMING_CONNECTION:
                printf("A connection is incoming.\n");
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                printf("The server is full.\n");
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                printf("A client has disconnected.\n");
                break;
            case ID_CONNECTION_LOST:
                printf("A client lost the connection.\n");
                break;
            case ID_USER_PACKET_ENUM:
                processClientMessage();
                break;
            default:
                printf("Message with identifier %i has arrived.\n", packet->data[0]);
                break;
            }
        }
}

void Server::processClientMessage(){
    int message_id = 0;
    SLNet::BitStream bs_in(packet->data, packet->length, false);
    bs_in.IgnoreBytes(sizeof(SLNet::MessageID));
    bs_in.Read(message_id);
    // test here
     switch (message_id){
        case sendClientInfo:
            this->receiveData(bs_in);
            break;
        default:
            break;
    }
}

void Server::sendData()
{
    SLNet::BitStream stream;
    stream.Write(( SLNet::MessageID )ID_USER_PACKET_ENUM);
    stream.Write(sendServerInfo);
    stream.Write(crtClients); // 传递目前共有所有clients包括server自己的个数
    for(int i = 0;i < crtClients;i++){
        // playerInfo is updating while the device running
        stream.Write(playersInfo[i].blood);
        stream.WriteVector(playersInfo[i].x,playersInfo[i].y,playersInfo[i].z);
    }
    //std::cout<<packet->systemAddress.ToString()<<std::endl;
    peer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
    return;
}

void  Server::sendScshot() {

}

void NetManager::recieveScshot(SLNet::BitStream &bs_in) {
	unsigned int pic_size;
	int w, h, n;
	bs_in.Read(pic_size);//read size;
	bs_in.Read(w);
	bs_in.Read(h);
	bs_in.Read(n);
	bs_in.Read(scshot.pictureData);//read picture data
	stbi_write_png("temp.png", w, h, n, scshot.pictureData, w*n);
}

void Server::receiveData(SLNet::BitStream &bs_in)
{
    PlayerInfo client_tmp;
    int tmp_crtClients;
    // no need to read message id again, because Read auto move offset
    bs_in.Read(tmp_crtClients);
    bs_in.Read(client_tmp.index);
    bs_in.Read(client_tmp.blood);
    bs_in.ReadVector(client_tmp.x, client_tmp.y, client_tmp.z);
    if(client_tmp.index == crtClients){
        crtClients++;
        playersInfo.push_back(client_tmp);
        // add iplayer to scene node
        createPlayer(client_tmp.index);
    }
    else{
        // manipulate the location
        if(playersNode.size() > client_tmp.index){
            playersNode[client_tmp.index]->setPosition(core::vector3df(client_tmp.x, client_tmp.y, client_tmp.z));
        }
        else
            std::cerr<<"Accessing IMeshSceneNode not exist"<<std::endl;
    }
    sendData();
    return;
}

void Client::initNet()
{
    char str[256];

    peer = SLNet::RakPeerInterface::GetInstance();
    if (NULL == peer)
    {
        std::cout << "GetInstance failed" << std::endl;
        return;
    }
 
    printf("Enter server IP or hit enter for 127.0.0.1\n");
	std::string tmp;
	std::getline(std::cin, tmp);
	strcpy(str,tmp.data());
	//gets(str);
    if (str[0]==0){
        strcpy(str, "127.0.0.1");
    }
    
    strcpy(server_ip,str);
    
    SLNet::SocketDescriptor sd;
    
    int start_up = peer->Startup(1, &sd, 1);
    if (start_up > 0)
    {
        std::cout << "Startup failed" << std::endl;
        return;
    }
 
    //Connect 连接Serveer
    //successful 返回 CONNECTION_ATTEMPT_STARTED
    bool rs = (peer->Connect(this->server_ip, this->server_port, NULL, 0, 0) == SLNet::CONNECTION_ATTEMPT_STARTED);
    if (!rs)
    {
        std::cout << "connect server failed" << std::endl;
        return;
    }
    
    printf("Starting the client.\n");
    
    return;
}

void Client::updateNet()
{
    for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
        {
            switch (packet->data[0])
            {
            case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                std::cout << "Another client has disconnected" << std::endl;
                break;
            case ID_REMOTE_CONNECTION_LOST:
                std::cout << "Another client has lost the connection" << std::endl;
                break;
            case ID_REMOTE_NEW_INCOMING_CONNECTION:
                std::cout << "Another client has connected" << std::endl;
                break;
            //client连接上server后会触发
            case ID_CONNECTION_REQUEST_ACCEPTED:
                {
                    std::cout << "Our connection request has been accepted for server" << std::endl;
                    server_address = packet->systemAddress;
                    break;
                }
            case ID_NEW_INCOMING_CONNECTION:
                std::cout << "A connection is incoming  " << std::endl;
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                std::cout << "The server is full" << std::endl;
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                std::cout << "A client has disconnected" << std::endl;
                break;
            case ID_CONNECTION_LOST:
                std::cout << "A client lost the connection  " << std::endl;
                break;
            case ID_USER_PACKET_ENUM:
                processServerMessage();                //用户自定义消息入口
                break;
            default:
                std::cout << "Message with identifier %d has arrived" << packet->data[0] << std::endl;
                break;
            }
    }
}

void Client::sendData()
{
    
    SLNet::BitStream stream;
    stream.Write((SLNet::MessageID)ID_USER_PACKET_ENUM);
    stream.Write(sendClientInfo);
    stream.Write(crtClients);
    stream.Write(playerInfo.index); // 返回自己的index数值
    stream.Write(playerInfo.blood);
    stream.WriteVector(playerInfo.x, playerInfo.y, playerInfo.z);
    peer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
    return ;
}

void Client::receiveData(SLNet::BitStream &bs_in)
{
    // no need to read message id again, because Read auto move offset
    bs_in.Read(crtClients);// 读出目前活跃玩家数量
    int tmp_size = playersInfo.size();
    // 将从server那里得到的信息进行更新, 如原来的vector为0或者有新的就push
    if(playersInfo.size() < crtClients){
        playersInfo.resize(crtClients+1);
        playersInfo[crtClients].index = crtClients;
        playerInfo.index = playersInfo[crtClients].index;
        playersInfo[crtClients].x = playerInfo.x;
        playersInfo[crtClients].y = playerInfo.y;
        playersInfo[crtClients].z = playerInfo.z;
        crtClients++;
    }
    
    for(int i = 0;i < crtClients;i++){
        bs_in.Read(playersInfo[i].blood);
        bs_in.ReadVector(playersInfo[i].x, playersInfo[i].y,playersInfo[i].z);
        if(i>=tmp_size && i != playerInfo.index){
            createPlayer(i); // create new scene node
        }
        else if(i != playerInfo.index){
            playersNode[i]->setPosition(core::vector3df(playersInfo[i].x, playersInfo[i].y,playersInfo[i].z));
        }
    }
    sendData();
    return;
}

void Client::processServerMessage(){
    int message_id = 0;
    SLNet::BitStream bs_in(packet->data, packet->length, false);
    bs_in.IgnoreBytes(sizeof(SLNet::MessageID));
    bs_in.Read(message_id);
    switch (message_id){
    case sendServerInfo:
        this->receiveData(bs_in);
        break;
    default:
        break;
    }
}
