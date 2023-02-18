#include "tcp-client.hpp"
#include <unistd.h>
#include <iostream>
#include <string>

void TcpClient::run() {
    char buffer[1024] = {0};
    int sock = 0;
    int addrlen = sizeof(args.address);

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&args.address, addrlen) < 0) {
        std::cout << "Connection failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    while (std::getline(std::cin, line)) {
        // LF is required by the server
        line += "\n";
        // Send a message to the server
        send(sock, line.c_str(), line.length(), 0);
        // Receive a message from the server
        int valread = read(sock, buffer, 1024);
        std::cout << buffer;
    }

    close(sock);
}
