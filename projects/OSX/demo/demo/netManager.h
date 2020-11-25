//
//  netManager.hpp
//  demo
//
//  Created by 宋庭聿 on 2020/10/19.
//

// This manager cannot run on windows platform

#ifndef netmanager_h
#define netmanager_h

#include <thread>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

const int serverSendPort = 6000;
const int serverReceivePort = 9000;

const int receiverSendPort[1] = {9000};
const int receiverReceivePort[1] = {6000};


enum netStatus{server,terminal};

class NetManager{
public:
    void* send(void*);
    void* recv(void*);
    int flag = 0;
};

#endif /* netManager_h */
