#include <stdio.h>
#include <winsock2.h>
#include <bits/stdc++.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 1024
// gcc -g cli.cpp -lstdc++ -lws2_32 -o cli.exe


int menu() {
    cout << "======= Select the menu ========" << endl;
    cout << "[1] Them sinh vien" << endl;
    cout << "[2] Sua sinh vien" << endl;
    cout << "[0] Thoat chuong trinh" << endl;
    cout << "================================" << endl;
    int choice;
    cin >> choice;
    return choice;
}

string createFunction() {
    fflush(stdin);
    string name, age, gpa;
    cout << "Nhap ten: ";
    getline(cin, name);
    cout << "Nhap tuoi: ";
    cin >> age;
    cout << "Nhap gpa: "; 
    cin >> gpa;
    string data = name + ";" + age + ";" + gpa;
    return data;
}

string editFunction() {
    fflush(stdin);
    string id, data;
    cout << "Nhap ID can sua: ";
    cin >> id;
    data = createFunction();
    return id + ";" + data;
}

void showFunction() {
    ifstream file;
    file.open("data.txt");
    if(!file.is_open()) {
        cout << "Error opening data.txt" << endl;
    } else {
        cout << "DANH SACH SINH VIEN: " << endl;
        string line;
        while(getline(file, line)) {
            stringstream ss(line);
            string str;
            vector<string> temp;
            while(ss >> str) {
                temp.push_back(str);
            }
            cout << "Sinh vien ID: " << temp[0] << " ";
            for(int i = 1; i < temp.size()-1; i++) {
                cout << temp[i] << " ";
            }
            cout << fixed << setprecision(2) << atof(temp[temp.size()-1].c_str()) << endl;
        }
    }
}

int main()
{
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;
    int slen = sizeof(server);

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
    while(true) {
        fflush(stdin);
        char buffer[1024];
        int choice = menu();
        string data;
        if(choice == 0) {
            break;
        } else if(choice == 1) {
            data = createFunction();
            data = to_string(choice) + ";" + data;
        } else if(choice == 2) {
            showFunction();
            data = editFunction();
            data = to_string(choice) + ";" + data;
        }
        // cout << data << endl;
        
        strcpy(buffer, data.c_str());

        if (sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server, slen) == SOCKET_ERROR) {
            printf("Error send to sever!.\n");
            return 1;
        }

        printf("Send to sever: %s\n", buffer);

        // Receive message from server
        char recv_buffer[BUFFER_SIZE];
        memset(recv_buffer, 0, sizeof(recv_buffer));

        int recv_len = recvfrom(sock, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *)&server, &slen);
        if (recv_len == SOCKET_ERROR) {
            cout << "Error receive from server " << strerror(errno) << endl;
            continue;
        }
        cout << "Receive from sever: " <<  recv_buffer << endl;
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
