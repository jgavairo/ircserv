#pragma once

#include "lib.hpp"
#include "Client.hpp"
#include "Parser.hpp"
#include "commands/CommandsList.hpp"

class Client;
class Channel;
class Parser;
class CommandsList;

class Server
{
    private:
        int _fd;
        int _port;

        bool _isRunning;

        Parser _parser;
        CommandsList _commandsList;

        std::string _name;
        std::string _password;
        sockaddr_in _server_adress;

        std::vector<pollfd> _fds;
        std::map<int, Client*> _clients;
        std::map<std::string, Channel*> _channels;
        std::map<std::string, ACommand*> _commands;
        
        void initialisation(int argc, char** argv);
        int addNewClient();
        void receiveNewSignal(size_t& i);
        void handleCommand(Client* client, std::string commandName, std::string arguments);


    public:
        Server(int argc, char**argv);
        ~Server();
        
        void run();
};