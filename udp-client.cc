#include "udp-client.hpp"
#include <signal.h>
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

int sock_udp = 0;

// Signal handler
void udp_signalhandler(int signum) {
    close(sock_udp);
    exit(EXIT_SUCCESS);
}

void UdpClient::run() {
    char buffer[1024] = {0};
    int len;
    int addrlen = sizeof(args.address);

    // Create socket
    if ((sock_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Socket creation error." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set timeout
    struct timeval tv;
    tv.tv_sec = 1;  // 1 second
    tv.tv_usec = 0;
    if (setsockopt(sock_udp, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        std::cerr << "Socket option error." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Register signal handler
    struct sigaction a;
    a.sa_handler = udp_signalhandler;
    a.sa_flags = 0;
    sigemptyset(&a.sa_mask);
    sigaction(SIGINT, &a, NULL);

    // Read lines from stdin
    std::string line;
    while (std::getline(std::cin, line)) {
        // Check if line fits
        if (line.length() > 255) {
            std::cerr << "Line too long" << std::endl;
            continue;
        }

        // Send a message to the server
        buffer[0] = (char)Opcode::Request;
        buffer[1] = line.length();
        line.copy(buffer + 2, line.length());

        // Send and receive
        sendto(sock_udp, buffer, line.length() + 2, MSG_CONFIRM,
               (const struct sockaddr*)&args.address, sizeof(args.address));

        ssize_t n = recvfrom(sock_udp, (char*)buffer, 1024, MSG_WAITALL,
                             (struct sockaddr*)&args.address, (socklen_t*)&len);
        // Check if server responded
        if (n < 0) {
            std::cerr << "No response" << std::endl;
            continue;
        }
        buffer[n] = '\0';

        // Print response from server
        if (buffer[0] == (char)Opcode::Response) {
            if (buffer[1] == (char)Status::Ok) {
                std::cout << "OK:";
            } else {
                std::cout << "ERR:";
            }
            std::cout << buffer + 3 << std::endl;
        } else {
            std::cerr << "Unknown response" << std::endl;
        }
    }

    // Close socket if stdin "ends"
    close(sock_udp);
}
