#include "client.hpp"
#include "tcp-client.hpp"
#include "udp-client.hpp"

Client::Client(Args args) {
    this->args = args;
}

Client* Client::create(Args args) {
    if (args.mode == "tcp") {
        return new TcpClient(args);
    } else if (args.mode == "udp") {
        return new UdpClient(args);
    } else {
        throw "Unknown protocol";
    }
}
