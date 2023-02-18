#ifndef __UDP_CLIENT_HPP__
#define __UDP_CLIENT_HPP__

#include "args.hpp"
#include "client.hpp"

class UdpClient : public Client {
    using Client::Client;

   public:
    void run();
};

#endif  // __UDP_CLIENT_HPP__
