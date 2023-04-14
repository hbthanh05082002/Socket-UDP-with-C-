#include <winsock2.h>
#include <bits/stdc++.h>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 1024
// gcc -g svr.cpp -lstdc++ -lws2_32 -o svr.exe

class SinhVien {
    private: 
        string id;
        string name;
        int age;
        float gpa;
    public:
        SinhVien(){

        }
        SinhVien(string id, string name, int age, float gpa) {
            this->id = id;
            this->name = name;
            this->age = age;
            this->gpa = gpa;
        }

        void setId(string id) {
            this->id = id;
        }
        
        void setName(string name) {
            this->name = name;
        }

        void setAge(int age) {
            this->age = age;
        }

        void setGpa(float gpa) {
            this->gpa = gpa;
        }
        
        string getId() {
            return this->id;
        }

        string getName() {
            return this->name;
        }

        int getAge() {
            return this->age;
        }

        float getGpa() {
            return this->gpa;
        }

        void printSinhVien() {
            cout << "ID: " << id << " " << name << " " << age << " " << gpa << endl;
        }

        string toString() {
            return id + " " + name + " " + to_string(age) + " " + to_string(gpa);
        }
};

vector <string> newStringData;
vector <SinhVien> SVList;
// vector <int> newIntegerData;
void tach_chuoi(char str[]) {
    newStringData.clear();
    stringstream ss(str);
    string token;
    newStringData.clear();
    while(getline(ss, token, ';')) {
        newStringData.push_back(token);
    }
}

void writeToFile(string str) {
    ofstream file;
    file.open("data.txt", std::ios::app);
    file << str << "\n";
    file.close();
}

void reWriteToFile() {
    ofstream file;
    file.open("data.txt");
    for(auto os: SVList) {
        file << os.toString() << "\n";
    }
    file.close();
}

void getListSinhVien() {
    ifstream file;
    file.open("data.txt", std::ios::in);
    if(!file.is_open()) {
        cout << "open file error!" << endl;
        return;
    } else {
        string line;
        while(getline(file, line)) {
            vector<string> tokens;
            stringstream ss(line);
            // cout << line << endl;
            string str;
            while(ss >> str) {
                tokens.push_back(str);
            }
            SinhVien sv;
            sv.setId(tokens[0]);
            sv.setGpa(atof(tokens[tokens.size() - 1].c_str()));
            sv.setAge(atoi(tokens[tokens.size() - 2].c_str()));
            string temp = "";
            for(int i = 1; i < tokens.size()-2; i++) {
                temp += tokens[i] + " ";
            }
            sv.setName(temp);
            SVList.push_back(sv);
            tokens.clear();
        }
    }
    file.close();
}


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

        int clientChoice = atoi(newStringData[0].c_str());
        string result;
        switch (clientChoice) {
            case 1: {
                getListSinhVien();
                cout << "Chuc nang 1" << endl;
                string newID;
                if(SVList.size() < 10) {
                    newID = "SV0" + to_string(SVList.size() + 1);
                } else {
                    newID = "SV" + to_string(SVList.size() + 1);
                }
                SinhVien sv(newID, newStringData[1], atoi(newStringData[2].c_str()), atof(newStringData[3].c_str()));
                writeToFile(sv.toString());
                //
                result = sv.toString();
                break;
            }
            case 2: {
                cout << "Chuc nang 2" << endl;
                getListSinhVien();
                bool check = false;
                for(int i = 0; i < SVList.size(); i++) {
                    if(SVList[i].getId() == newStringData[1]) {
                        SVList[i].setName(newStringData[2]);
                        SVList[i].setAge(atoi(newStringData[3].c_str()));
                        SVList[i].setGpa(atof(newStringData[4].c_str()));
                        // 2 ID Ten Tuoi Diem GPA
                        check = true;
                        result = SVList[i].toString();
                        break;
                    }
                }
                if(!check) result = "Unknown Sinh Vien!";
                reWriteToFile();
                SVList.clear();
                break;
            }
        }
        // Send back to client
        int send_len = sendto(sock, result.c_str(), sizeof(result), 0, (struct sockaddr *)&client, slen);
        if(send_len < 0) {
            cout << "Gui lai cho client khong thanh cong!" << endl;
        }
        cout << "Tra lai ket qua cho client!" << endl;
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
