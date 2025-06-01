#include "p2p_communication.h"
#include <winsock2.h>
#include <iostream> 
#include <string> 
#include <unistd.h>

#define AF_UNIX 1
using namespace std; 

P2PCommunication:: P2PCommunication(const Node &localNode)
: localNode_(localNode){}

P2PCommunication:: ~P2PCommunication(){}

bool P2PCommunication :: sendMessage(const string &targetIP, int targetPort, const string &message)
{
    WSADATA wsaData ; 
    
}

