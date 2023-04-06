#include <stdio.h>
#include <winsock2.h>
#include <bits/stdc++.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 1024
// gcc -g cli.cpp -lstdc++ -lws2_32 -o cli.exe

int main()
{
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;
    int slen = sizeof(server);
    // char message[] = "Hello from client";

    // initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }

    // create a socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
        printf("Failed to create socket.\n");
        return 1;
    }

    // setup server address
    memset((char *)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // send message to server
    cout << "====Go \"exit\" de thoat chuong trinh====" << endl;
    while(true) {
        char buffer[1024];
        cout << "Nhap vao 3 canh cua tam giac: ";
        cin.getline(buffer, sizeof(buffer));
        if(strcmp(buffer, "exit") == 0) {
            return 1;
        }

        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server, slen) == SOCKET_ERROR) {
            printf("Gui loi.\n");
            return 1;
        }

        printf("Gui toi sever: %s\n", buffer);

        // Receive message from server
        char recv_buffer[BUFFER_SIZE];
        memset(recv_buffer, 0, sizeof(recv_buffer));

        int recv_len = recvfrom(sock, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *)&server, &slen);
        if (recv_len == SOCKET_ERROR) {
            cout << "Error recv from server " << strerror(errno) << endl;
            continue;
        }
        cout << "Nhan duoc tu sever: " <<  recv_buffer << endl;
        

        // Sao chép dữ liệu từ buffer vào biến data
        // char data[BUFFER_SIZE];
        // strcpy(data, recv_buffer);
        // data[recv_len] = '\0';
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
