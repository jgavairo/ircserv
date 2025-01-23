#pragma once

#include "lib.hpp"
#include "Client.hpp"

class Client;
class Channel;

class Server
{
    private:
        int _fd;
        int _port;

        bool _isRunning;

        std::string _name;
        std::string _password;
        sockaddr_in _server_adress;

        std::vector<pollfd> _fds;
        std::map<int, Client*> _clients;
        std::map<std::string, Channel*> _channels;
        
        void initialisation(int argc, char**argv);
    public:
        Server(int argc, char**argv);
        ~Server();
        
        void run();
};