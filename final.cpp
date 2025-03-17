#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <ctime>


#define PORT 18756
#define BUFFER_SIZE 1024









void receiveMessages(int clientSocket) {

    char buffer[BUFFER_SIZE] ;
    std::ofstream file ("message.txt", std::ios::app);

    if (!file) {
        std::cerr << "err" << std::endl;
        return;
    }

    while (true) {
         memset(&buffer, 0, sizeof(buffer));
        int valread = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (valread > 0) {
            uint16_t tag = (buffer[0] << 8) | buffer[1];
            uint16_t length = (buffer[2] << 8) | buffer[3];

            std::string message(buffer + 4, length);

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
    receiveThread.join();
    return 0;
}
