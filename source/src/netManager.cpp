//
//  netManager.cpp
//  demo
//
//  Created by 宋庭聿 on 2020/12/5.
//

#include "netManager.h"

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
    std::cout<<start_up<<std::endl;
    if (start_up > 0)
    {
        std::cerr << "Startup failed" << std::endl;
        return;
    }
    
    //SetMaximumIncomingConnections容许最多的连接数量
    peer->SetMaximumIncomingConnections(MAX_CLIENT);
    
    serverInfo.blood = 10;
    
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
                break;
            case ID_REMOTE_NEW_INCOMING_CONNECTION:
                printf("Another client has connected.\n");
                break;
            case ID_CONNECTION_REQUEST_ACCEPTED:
                printf("Our connection request has been accepted.\n");
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
    stream.Write(serverInfo.blood);
    stream.WriteVector(serverInfo.x, serverInfo.y, serverInfo.z);
    std::cout<<packet->systemAddress.ToString()<<std::endl;
    peer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
    return;
}

void Server::receiveData(SLNet::BitStream &bs_in)
{
    PlayerInfo client_tmp;
    // no need to read message id again, because Read auto move offset
    bs_in.Read(client_tmp.blood);
    bs_in.ReadVector(client_tmp.x, client_tmp.y, client_tmp.z);
    clientsInfo.push_back(client_tmp);
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
    gets(str);
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
                    sendData();
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
    stream.Write(clientInfo.blood);
    stream.WriteVector(clientInfo.x, clientInfo.y, clientInfo.z);
    peer->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
    return ;
}

void Client::receiveData(SLNet::BitStream &bs_in)
{
    bs_in.Read(serverInfo.blood);
    bs_in.ReadVector(serverInfo.x,serverInfo.y, serverInfo.z);
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
