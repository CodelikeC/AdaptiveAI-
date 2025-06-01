#ifndef P2P_COMMUNICATION_H 
#define P2P_COMMUNICATION_H 

#include "node.h"
#include <iostream> 
using namespace std ;

#include <string> 


class P2PCommunication
{
    public : 
    P2PCommunication(const Node &localNode);
    ~P2PCommunication(); 

    bool sendMessage(const string &targetIP, int targetPort, const string &message);

    string recieveMessage(); 

    private: 
    Node localNode_ ; 
}; 



#endif 
