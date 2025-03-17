#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <ctime>
#include <sstream>
#define PORT 18768
#define BUFFER_SIZE 1024


void SendN(int clientSocket) {

    std::string name="Anastasiia";
    uint8_t tag= 0x03;
    u_int32_t len= name.size();
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    buffer[0] = tag;

    buffer[1] = (len>> 24) & 0xFF;
    buffer[2] = (len>> 16) & 0xFF;
    buffer[3] = (len>> 8) & 0xFF;
    buffer[4] = (len) & 0xFF;

    memcpy(buffer+5, name.c_str(), len);

 int byteSent = send(clientSocket, buffer, BUFFER_SIZE, 0);

    if (byteSent == -1) {
        std::cerr<< "xdfcgvbjhn"<< std::endl;
    }
    else{ std::cerr<< "11111111111"<< std::endl; }


}

void BackID(int clientSocket, uint8_t tag, u_int32_t len, const std::string &messenge) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    buffer[0] = tag;

    buffer[1] = (len >> 24) & 0xff;
    buffer[2] = (len >> 16) & 0xff;
    buffer[3] = (len >> 8) & 0xff;
    buffer[4] = len & 0xff;

    memcpy(buffer + 5, messenge.c_str(), messenge.size());

    int byteSent = send(clientSocket, buffer, 5+ messenge.size(), 0);
    if (byteSent == -1) {
        std::cerr<< "xdfcgvbjhn"<< std::endl;
    }
    else {
        std::cerr<< "11111111111"<< std::endl;
        SendN(clientSocket);
    }


}


void sendSN(int clientSocket) {

    std::string name="Koptieva";
    uint8_t tag= 0x03;
    u_int32_t len= name.size();
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    buffer[0] = tag;

    buffer[1] = (len>> 24) & 0xFF;
    buffer[2] = (len>> 16) & 0xFF;
    buffer[3] = (len>> 8) & 0xFF;
    buffer[4] = (len) & 0xFF;

    memcpy(buffer+5, name.c_str(), len);

    int byteSent = send(clientSocket, buffer, BUFFER_SIZE, 0);

    if (byteSent == -1) {
        std::cerr<< "xdfcgvbjhn"<< std::endl;
    }
    else{ std::cerr<< "1111111111K"<< std::endl; }


}

void sendVAriant(int clientSocket, const std::string &var) {

    uint8_t tag = 0x12;
    u_int32_t len = var.size();
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    buffer[0] = tag;
    buffer[1] = (len >> 24) & 0xFF;
    buffer[2] = (len >> 16) & 0xFF;
    buffer[3] = (len >> 8) & 0xFF;
    buffer[4] = (len) & 0xFF;

     memcpy(buffer + 5, var.c_str(), len);

    int byteSent = send(clientSocket, buffer, 5 + len, 0);
    if (byteSent == -1) {
        std::cerr << "er" << std::endl;
    } else {
        std::cerr << "77777" << std::endl;
    }
}



void processMessage(uint8_t tag, uint32_t length, const std::string &message, int clientSocket) {



    if (tag == 0x01) {
        if (message == "Server offline!") {
            std::cerr << "Server offline!" << std::endl;

        } else if (message == "Server online!") {

            std::cerr << "Server online!" << std::endl;
        }
    } else if (tag == 0x02) {
        std::cout << "Session ID: " << message << std::endl;

        BackID(clientSocket, tag, length, message);
    } else if (tag == 0x03) {
        std::cerr << "lastname: " << message << std::endl;
        sendSN(clientSocket);
        int var = 18;
        sendVAriant(clientSocket, "18");
    } else if (tag == 0x05) {
         std::cerr << "variant: " << message << std::endl;


    } else if (tag >= 0x10 && tag <= 0x50) {
         std::cout << "   " << std::hex << (int)tag << ": " << message << std::dec << std::endl;
    } else {
        std::cout << " err tag: " << std::hex << (int)tag << std::dec << std::endl;
    }
}

 void receiveMessages(int clientSocket) {
    char buffer[BUFFER_SIZE];
    std::ofstream file("message.txt", std::ios::app);

    if (!file) {
        std::cerr << "err" << std::endl;
        return;
    }

    while (true) {
        memset(&buffer, 0, sizeof(buffer));
        int valread = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (valread > 0) {
            std::cout << "byte ";
            for (int i = 0; i < valread; ++i) {
                std::cout << std::hex << (int)(unsigned char)buffer[i] << " ";
            }
            std::cout << std::dec << std::endl;


            uint8_t tag = buffer[0];
            uint32_t length = (static_cast<uint8_t>(buffer[1]) << 24) |
                              (static_cast<uint8_t>(buffer[2]) << 16) |
                              (static_cast<uint8_t>(buffer[3]) << 8) |
                              static_cast<uint8_t>(buffer[4]);


            std::cout << "teg: " << std::hex << (int)tag << ", len: " << length << std::dec << std::endl;

            if (length > valread - 5) {
                std::cout << "Помилка: довжина повідомлення не відповідає кількості отриманих байтів!" << std::endl;
                continue;
            }

            std::string message(buffer + 5, length);
            std::time_t now = std::time(nullptr);
            std::tm* localTime = std::localtime(&now);
            char timeBuffer[20];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", localTime);

            file << "[" << timeBuffer << "] Повідомлення від сервера: " << message << std::endl;

             processMessage(tag, length, message, clientSocket);
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



    std::thread receiveThread(receiveMessages, clientSocket);
    std::string response;
    while (true) {
       // receiveMessages(clientSocket);


        std::cout << "Ваша відповідь: ";
        std::getline(std::cin, response);
        if (response == "exit") break;





    }

    close(clientSocket);
    receiveThread.join();
    return 0;
}
