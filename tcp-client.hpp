#ifndef __TCP_CLIENT_HPP__
#define __TCP_CLIENT_HPP__

#include "args.hpp"
#include "client.hpp"

class TcpClient : public Client {
    using Client::Client;

   public:
    void run();
};

#endif  // __TCP_CLIENT_HPP__
