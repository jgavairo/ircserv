#include "Server.hpp"

int main (int argc, char**argv)
{
    try
    {
        Server* server = Server::getInstance(argc, argv);
        server->run();
        delete server;
        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        if (Server::getInstance())
            delete Server::getInstance();
        return -1;
    } 
}