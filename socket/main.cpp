#include <iostream>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>

using namespace std;
void sendMessage(SOCKET sk,char* buffer,int iRc){
    while (true){
    gets(buffer);
    if (strcmp(buffer,"exit") == 0){
        break;
    } else {
        iRc = send(sk, buffer, strlen(buffer), 0);
    }
    }
}
void recvMessage(SOCKET sk,char* buffer,int iRc){
    while (true){
    iRc = recv(sk, buffer, 256, 0);
    if (iRc == SOCKET_ERROR){
    printf("Could not receive data from server.\n");
    } else {
        buffer[iRc]=0;
        printf("Message from server: %s\n", buffer);
    }
    }
}

int main()
{
    //khai bao bien'
    WSADATA wsaData;
    SOCKADDR_IN rAddr;
    char buffer[20];
    int iRc;



    //Nap thu vien khoi tao socket
    iRc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET sk = socket(AF_INET, SOCK_STREAM, 0);

    //ket noi server
    rAddr.sin_family = AF_INET;
    rAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    rAddr.sin_port = htons(1024);
    iRc = connect(sk, (LPSOCKADDR)&rAddr, sizeof(rAddr));

    //nhan va gui message
    std::thread t1(sendMessage,sk,buffer,iRc);
    //nhan message
    std::thread t2(recvMessage,sk,buffer,iRc);

    t1.join();
    t2.join();
    //dong socket giai phong tai nguyen
    closesocket(sk);
    WSACleanup();
    return 0;
}
