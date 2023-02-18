#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "args.hpp"

class Client {
   protected:
    Args args;

   public:
    Client(Args args);
    static Client* create(Args args);
    virtual void run(){};
};

#endif  // __CLIENT_HPP__
