#include "tcp-client.hpp"
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <string>

int sock_tcp = 0;

// Signal handler
void tcp_signalhandler(int signum) {
    char buffer[1024] = {0};
    send(sock_tcp, "BYE\n", 4, 0);
    int valread = read(sock_tcp, buffer, 1024);
    if (valread <= 0) {
        std::cerr << "Server closed the connection." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << buffer << std::flush;
    close(sock_tcp);
    exit(EXIT_SUCCESS);
}

void TcpClient::run() {
    int addrlen = sizeof(args.address);

    // Create socket
    if ((sock_tcp = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock_tcp, (struct sockaddr*)&args.address, addrlen) < 0) {
        std::cout << "Connection failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Register signal handler
    struct sigaction a;
    a.sa_handler = tcp_signalhandler;
    a.sa_flags = 0;
    sigemptyset(&a.sa_mask);
    sigaction(SIGINT, &a, NULL);

    std::string line;
    while (std::getline(std::cin, line)) {
        char buffer[1024] = {0};
        // LF is required by the server
        line += "\n";
        // Send a message to the server
        send(sock_tcp, line.c_str(), line.length(), 0);
        // Receive a message from the server
        int valread = read(sock_tcp, buffer, 1024);
        if (valread <= 0) {
            std::cerr << "Server closed the connection." << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << buffer << std::flush;
        // Check if we should exit
        if (buffer == "BYE\n") {
            close(sock_tcp);
            exit(EXIT_SUCCESS);
        }
    }

    // If we get here, it means the user didn't send a BYE, so we do it ourselves
    tcp_signalhandler(SIGINT);
}
