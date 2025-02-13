#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

using namespace std;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Socket creation error!" << endl;
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        cerr << "Invalid address!" << endl;
        return 1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connection failed!" << endl;
        return 1;
    }

    string message;
    while (true) {
        cout << "You: ";
        getline(cin, message);

        send(sock, message.c_str(), message.length(), 0);
        if (message == "bye") break;

        memset(buffer, 0, sizeof(buffer));
        int bytes_received = read(sock, buffer, sizeof(buffer));
        if (bytes_received <= 0) break;

        cout << "Chatbot: " << buffer << endl;
    }

    close(sock);
    return 0;
}
