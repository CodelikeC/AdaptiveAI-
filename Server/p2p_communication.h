#ifndef P2P_COMMUNICATION_H 
#define P2P_COMMUNICATION_H 

#include "node.h"
#include <iostream> 
using namespace std ;

#include <string> 

class P2PCommunication
{
    public : 
     /**
     * @brief Constructor for P2PCommunication, initializes with a local node.
     * @param localNode The local node configuration (IP, port, etc.).
     */
    P2PCommunication(const Node &localNode);
    ~P2PCommunication(); 

    /**
     * @brief Sends a message to a target node using TCP.
     * @param targetIp The IP address of the target node.
     * @param targetPort The port number of the target node.
     * @param message The message to send.
     * @return true if successful, false otherwise.
     * @throws std::runtime_error if socket initialization or connection fails.
     */

    bool sendMessage(const string &targetIP, int targetPort, const string &message);
    explicit P2PCommunication(const Node &localNode);
    string recieveMessage(); 

    private: 
    const Node &localNode_ ;
    void intitialwinsock(); 
    void cleanupWinsock(); 

    // Disable copying to prevent resource duplication..// 
    P2PCommunication(const P2PCommunication &) = delete; 
    P2PCommunication &operator = (const P2PCommunication &) = delete; 
}; 

#endif 
