#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <thread>
#include <filesystem>
#include <filesystem>
#include <mutex>
#include <queue>
#define BUFFER_SIZE 1024
#define SERVER_DIR "./server_files/"
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <vector>

#define PORT 18756
#define BUFFER_SIZE 1024

/*void sendCommand(int clientSocket, const void* data, size_t size) {
    ssize_t bytesSent = send(clientSocket, data, size, 0);
    if (bytesSent <= 0) {
        perror("send error");
        exit(1);

    }
}

void recvData(int clientSocket, void* buffer, size_t size) {
    ssize_t bytesRead = recv(clientSocket, buffer, size, 0);
    if (bytesRead <= 0) {
        perror("recv error");
        exit(1);
    }
}*/





/*
std::string recvStr(int clientSocket) {
    uint32_t len;
    recvData(clientSocket, &len, sizeof(len));
    len = ntohl(len);
    char buffer[BUFFER_SIZE] = {0};
    recvData(clientSocket, buffer, len);
    return std::string(buffer);
}
*/










void receiveMessages(int clientSocket) {

    char buffer[BUFFER_SIZE] ;
    std::ofstream file ("message.txt", std::ios::app);


    while (true) {
         memset(&buffer, 0, sizeof(buffer));
        int valread = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (valread > 0) {
            uint16_t tag = (buffer[0] << 8) | buffer[1];
            uint16_t length = (buffer[2] << 8) | buffer[3];

            std::string message(buffer + 2, length);

            std::time_t now = std::time(nullptr);
            std::tm* localTime = std::localtime(&now);
            char timeBuffer[20];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", localTime);

            file << "[" << timeBuffer << "] Повідомлення від сервера: " << message << std::endl;

            std::cout << "Отримано повідомлення: " << message << std::endl;
        } else {
            break;
        }
    }

    file.close();


}












int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("error opening socket");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "3.78.28.71", &(serverAddr.sin_addr));

    if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        perror("err");
        close(clientSocket);
        return 1;
    }
    std::cout << " підключення є " << std::endl;


    std::cout << "Підключено до сервера!" << std::endl;


    std::cout << "Підключено до сервера!" << std::endl;

    std::thread receiveThread(receiveMessages, clientSocket);
    std::string response;
    while (true) {
       // receiveMessages(clientSocket);


        std::cout << "Ваша відповідь: ";
        std::getline(std::cin, response);
        if (response == "exit") break;




    }

    close(clientSocket);
    return 0;
}
