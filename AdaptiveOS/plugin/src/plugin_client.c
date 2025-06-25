#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 

#include <winsock2.h>
#include <ws2tcpip.h>

#include "plugin_client.h"

#include <unistd.h> 

#pragma comment(lib, "ws2_32.lib")

int main ()
{
    WSADATA wsaData; 
    SOCKET sock; 
    struct sockaddr_in server; 
    char message[26], response[512];
    int result; 

    // 1. Khoi dong winsock .. // 
    if (WSAStartup (MAKEWORD(2,2), &wsaData) != 0 )
    {
        printf("WSA Startup failed.\n");
        return 1; 
    }

    // 2. Tao socket .. // 
    sock  = socket(AF_INET, SOCK_STREAM, 0); 
    if (sock == INVALID_SOCKET)
    {
        printf("SOCKET CREATION FAILED"); 
        WSACleanup(); 
        return 1; 
    }

    // 3. Thiết lập địa chỉ server
    server.sin_family = AF_INET;
    server.sin_port = htons(9001);
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // hoặc IP server thực

    // 4. Kết nối
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) 
    {
        printf("Connect failed.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");

    // 5. Gửi yêu cầu
    sprintf(message, "PLUGIN_LOAD:plugin_xyz:1.0:token123");
    send(sock, message, strlen(message), 0);

    // 6. Nhận phản hồi
    result = recv(sock, response, sizeof(response) - 1, 0);
    if (result > 0) 
    {
        response[result] = '\0';
        printf("Server response: %s\n", response);
    }

    // 7. Đóng kết nối
    closesocket(sock);
    WSACleanup();
    return 0; 
}
