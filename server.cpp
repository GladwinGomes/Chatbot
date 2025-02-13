#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

using namespace std;

string chatbotResponse(const string& message) {
    if (message == "hello") return "Hello! How can I assist you?";
    if (message == "how are you") return "I'm just a bot, but I'm doing fine!";
    if (message == "bye") return "Goodbye! Have a nice day!";
    if (message == "what's your name") return "I'm a simple chatbot!";
    if (message == "what can you do") return "I can chat with you and answer simple questions.";
    if (message == "tell me a joke") return "Why don't scientists trust atoms? Because they make up everything!";
    return "I'm not sure how to respond to that.";
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Socket creation failed!" << endl;
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Binding failed!" << endl;
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        cerr << "Listening failed!" << endl;
        return 1;
    }

    cout << "Chatbot server listening on port " << PORT << "...\n";

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        cerr << "Accept failed!" << endl;
        return 1;
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(new_socket, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            cout << "Client disconnected.\n";
            break;
        }

        string client_message(buffer);
        cout << "Client: " << client_message << endl;

        if (client_message == "bye") {
            send(new_socket, "Goodbye!", 8, 0);
            break;
        }

        string response = chatbotResponse(client_message);
        send(new_socket, response.c_str(), response.length(), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
