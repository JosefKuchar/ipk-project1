#include "udp-client.hpp"
#include <unistd.h>
#include <iostream>
#include <string>

enum class Opcode {
    Request = 0,
    Response = 1,
};

enum class Status {
    Ok = 0,
    Error = 1,
};

void UdpClient::run() {
    char buffer[1024] = {0};
    int sock = 0;
    int len;
    int addrlen = sizeof(args.address);

    // Create socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cout << "Socket creation error." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    while (std::getline(std::cin, line)) {
        // Send a message to the server
        buffer[0] = (char)Opcode::Request;
        buffer[1] = line.length();  // TODO check if it fits
        line.copy(buffer + 2, line.length());

        sendto(sock, buffer, line.length() + 2, MSG_CONFIRM, (const struct sockaddr*)&args.address,
               sizeof(args.address));

        ssize_t n = recvfrom(sock, (char*)buffer, 1024, MSG_WAITALL,
                             (struct sockaddr*)&args.address, (socklen_t*)&len);
        buffer[n] = '\0';

        // TODO proper error handling
        if (buffer[0] == (char)Opcode::Response) {
            if (buffer[1] == (char)Status::Ok) {
                std::cout << "OK:";
            } else {
                std::cout << "ERR:";
            }
            std::cout << buffer + 3 << std::endl;
            // std::cout << n << std::endl;
        } else {
            std::cout << "Unknown response" << std::endl;
        }
    }

    close(sock);
}
