#include "args.hpp"
#include "client.hpp"

int main(int argc, char* argv[]) {
    Args args(argc, argv);
    Client* client = Client::create(args);
    client->run();
    delete client;
    return 0;
}
