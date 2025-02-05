#pragma once

#include "lib.hpp"
#include "Client.hpp"
#include "Parser.hpp"
#include "commands/CommandsList.hpp"

class Server
{
    private:
        static Server* _instance;
        int _fd;
        int _port;

        bool _isRunning;

        Parser _parser;

        std::string _name;
        std::string _password;
        sockaddr_in _server_adress;

        CommandsList _commandsList;
        std::vector<pollfd> _fds;
        std::map<int, Client*> _clients;
        std::map<std::string, Channel*> _channels;
        
        void initialisation(int argc, char** argv);
        int addNewClient();
        void receiveNewSignal(size_t& i);
        void handleCommands(Client* client, std::vector<std::string> commandLines);
        void sendWelcomeMessage(Client& client);

        Server(int argc, char**argv);

    public:
        ~Server();

        static Server* getInstance(int argc = 0, char** argv = NULL);

        std::map<std::string, Channel*>& getChannels();
        std::map<int, Client*>& getClients();

        void run();
};