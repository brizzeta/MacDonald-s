#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
using namespace std;

int main()
{
    WSADATA wsadata;

    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;    
    addr.sin_port = htons(23000);   
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }


    while(true) {
        const size_t receiveBufSize = 1024;
        char receiveBuf[receiveBufSize];
        sockaddr_in senderAddr;
        int senderAddrSize = sizeof(senderAddr);

        const char menu[][12] = { "coca cola", "gamburger", "bigmac", "free potato" };
        int time = 0;

        cout << "Receiving data...." << endl;
        int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);

        if (bytesReceived == SOCKET_ERROR)
        {
            cout << "recvfrom failed with error " << WSAGetLastError() << endl;
            return 4;
        }

        for (int i = 0, k = 3; i < 5; i++, k += 3)
        {
            if (strstr(receiveBuf, menu[i]))
            {
                time += k;
            }
        }

        cout << "Received from " << senderAddr.sin_addr.s_host << endl;
        cout << "Data: " << receiveBuf << endl;


        const size_t sendBufSize = 1024;

        char wait[10];
        _itoa_s(time, wait, 10);
        char sendBuf[sendBufSize] = "Please, wait ";
        strcat_s(sendBuf, sendBufSize, wait);

        int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
        if (sendResult == SOCKET_ERROR)
        {
            cout << "sendto failed with error " << WSAGetLastError() << endl;
            return 4;
        }

        Sleep(time * 1000);

        char end[sendBufSize] = "Ready!";
        sendResult = sendto(udpSocket, end, strlen(end), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
        if (sendResult == SOCKET_ERROR)
        {
            cout << "sendto failed with error " << WSAGetLastError() << endl;
            return 4;
        }
    }

    closesocket(udpSocket);
    WSACleanup();
}

