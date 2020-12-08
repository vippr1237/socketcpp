#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

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
    printf("Could not receive data from client.\n");
    } else {
        buffer[iRc]=0;
        printf("Message from client: %s\n", buffer);
    }
    }
}
int main()
{
    //khai bao bien'
    WSADATA wsaData;
    SOCKADDR_IN serAddr, cliAddr;
    int iRc;
    int adsize;
    char buffer[256];

    //Nap thu vien
    iRc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET ssk = socket(AF_INET, SOCK_STREAM, 0);

    //Thiet lap dia chi server
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serAddr.sin_port = htons(1024);

    //Gan dia chi va bat dau listen
    iRc = bind(ssk, (LPSOCKADDR)&serAddr, sizeof(serAddr));
    iRc = listen(ssk, 5);//listen
    adsize=sizeof(cliAddr);
    SOCKET sk = accept(ssk, (LPSOCKADDR)&cliAddr, &adsize); //accept ket noi

    //Nhan du lieu tu client
    std::thread t1(sendMessage,sk,buffer,iRc);
    std::thread t2(recvMessage,sk,buffer,iRc);

    t1.join();
    t2.join();

    //dong socket giai phong tai nguyen
    closesocket(sk);
    closesocket(ssk);
    WSACleanup();
    return 0;
}
