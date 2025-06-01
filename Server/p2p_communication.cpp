#include "p2p_communication.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream> 
#include <string> 
#include <unistd.h>

#include <memory> 
#include <stdexcept>

#pragma comment(lib, "ws2_32.lib") // link winsock library..// 

// #define AF_UNIX 1
using namespace std; 

// P2PCommunication:: ~P2PCommunication(){}

// bool P2PCommunication :: sendMessage(const string &targetIP, int targetPort, const string &message)
// {
//     WSADATA wsaData ; 
//     SOCKET sock; 
//     struct sockaddr_in server; 
//     char message[26], response[512];
//     int result; 
// }

namespace 
{
    constexpr int BUFFER_SIZE = 1024; // Buffer size for message..// 
}

P2PCommunication :: P2PCommunication(const Node &localNode)
:localNode_(localNode)
{
    intitialwinsock(); 
}

P2PCommunication :: ~P2PCommunication()
{
    cleanupWinsock(); 
}

void P2PCommunication :: intitialwinsock()
{
    WSADATA wsadata; 
    int result = WSAStartup(MAKEWORD(2,2), &wsadata);
    if (result != 0)
    {
        throw runtime_error("WSAStartUP failed:" + to_string(result)); 
    }
}

void P2PCommunication :: cleanupWinsock()
{
    WSACleanup(); 
}

bool P2PCommunication :: sendMessage(const string &targetIP, int targetPort, const string &message )
{
    try 
    {
        // create socket .. 
        unique_ptr<SOCKET, void(*)(SOCKET*)> sock
        (
            new SOCKET(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)),
            [](SOCKET* s) { closesocket(*s); delete s; }
        );

        if (*sock == INVALID_SOCKET)
        {
            throw runtime_error ("Socket creation failed:" + to_string(WSAGetLastError()) ); 
        }

        // setup the target address structure .. // 
        sockaddr_in serveraddr{}; 
        serveraddr.sin_family = AF_INET; 
        serveraddr.sin_port  = htons(static_cast<u_short>(targetPort));
        inet_pton(AF_INET, targetIP.c_str(), &serveraddr.sin_addr);

        // Connect to the server .. // 
        if (connect(*sock, reinterpret_cast<sockaddr*> (&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR)
        {
            throw runtime_error("Connection failed:" + to_string(targetPort));
            // Bao loi khi khong the tim thay cong .. .
        }

        // send message ..// 
        int byteSent = send(*sock, message.c_str(), static_cast<int>(message.length()), 0); 
        if (byteSent == SOCKET_ERROR)
        {
            throw runtime_error ("Send failed :" + to_string(WSAGetLastError()));
        }

        cout <<"Sent :" << byteSent << "bytes to" << targetIP << ":" << targetPort  << endl;
        return true; 

    } 
    catch (const runtime_error &e)
    {
        cerr <<"SendMessage Error:" << e.what() << endl; 
    }
}

string P2PCommunication :: recieveMessage()
{
    try 
    {
        // placeholder for server - side receive 
        SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSock == INVALID_SOCKET) {
            throw std::runtime_error("Listen socket creation failed: " + std::to_string(WSAGetLastError()));
        }

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(static_cast<u_short>(localNode_.getPort()));

        if (bind(listenSock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(listenSock);
            throw std::runtime_error("Bind failed: " + std::to_string(WSAGetLastError()));
        }

        if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
            closesocket(listenSock);
            throw std::runtime_error("Listen failed: " + std::to_string(WSAGetLastError()));
        }

        SOCKET clientSock = accept(listenSock, nullptr, nullptr);
        if (clientSock == INVALID_SOCKET) {
            closesocket(listenSock);
            throw std::runtime_error("Accept failed: " + std::to_string(WSAGetLastError()));
        }

        char buffer[BUFFER_SIZE] = {0};
        int bytesReceived = recv(clientSock, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived == SOCKET_ERROR) {
            closesocket(clientSock);
            closesocket(listenSock);
            throw std::runtime_error("Receive failed: " + std::to_string(WSAGetLastError()));
        }

        closesocket(clientSock);
        closesocket(listenSock);
        return std::string(buffer, bytesReceived);
    }
    catch (const runtime_error &e)
    {
        cerr <<"ReceiveMessage Error:" << e.what() << endl; 
        return ""; 
    }
}




