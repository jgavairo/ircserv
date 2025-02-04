#include "Server.hpp"

int main (int argc, char**argv)
{
    try
    {
        Server* server = Server::getInstance(argc, argv);
        server->run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    } 
    return 0;
}