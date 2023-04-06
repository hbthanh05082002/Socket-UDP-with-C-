#include <winsock2.h>
#include <bits/stdc++.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 1024
// gcc -g svr.cpp -lstdc++ -lws2_32 -o svr.exe


vector <string> newStringData;
vector <int> newIntegerData;
void tach_chuoi(char str[]) {
    newStringData.clear();
    newIntegerData.clear();
    stringstream ss(str);
    string token;
    newStringData.clear();
    while(ss >> token) {
        newStringData.push_back(token);
        newIntegerData.push_back(atoi(token.c_str()));
    }
}



class Triangle {
    private:
        int a, b, c;
    public:
        Triangle(int a, int b, int c) {
            this->a = a;
            this->b = b;
            this->c = c;
        }
        int getA() { return a; }
        int getB() { return b; }
        int getC() { return c; }

        bool checkTriangle(int a, int b, int c)
        {
            if((a + b  > c) && (a + c > b) && (b + c > a)) {
                return true;
            } else return false;
        }
};

int main()
{
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server, client;
    int slen = sizeof(client);
    char buf[512];

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
    server.sin_addr.s_addr = INADDR_ANY;

    // bind socket to server address
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Failed to bind socket.\n");
        return 1;
    }

    printf("Server started.\n");

    // wait for incoming messages
    
    while (1) {
        char receivedData[1024];
        memset(buf, 0, sizeof(buf));

        int recv_len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&client, &slen);
        if (recv_len == SOCKET_ERROR) {
            printf("Khong nhan duoc tu Client.\n");
            return 1;
        }
        printf("Nhan tu client: %s\n", buf);

        strcpy(receivedData, buf);
        receivedData[recv_len] = '\0';
        tach_chuoi(receivedData);

        // cout << newIntegerData.size() << endl;
        // for (int i = 0; i < newIntegerData.size(); i++) {
        //     cout << newIntegerData[i] << endl;
        // }
        Triangle tamgiac(newIntegerData[0], newIntegerData[1], newIntegerData[2]);
        char result[BUFFER_SIZE]; 
        if (tamgiac.checkTriangle(tamgiac.getA(), tamgiac.getB(), tamgiac.getC())) {
            strcpy(result, "Day la tam giac!");
            cout << result << endl;
        } else {
            strcpy(result, "Day khong la tam giac!");
            cout << result << endl;
        }

        // send back to client
        int send_len = sendto(sock, result, sizeof(result), 0, (struct sockaddr *)&client, slen);
        if(send_len < 0) {
            cout << "Gui lai cho client khong thanh cong!" << endl;
        }
        cout << "Tra lai ket qua cho client!" << endl;


    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
