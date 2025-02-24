#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <string>
#include <map>
#include "commands/ABotCommand.hpp"
#include "commands/Help.hpp"

class IrcBot
{
    private:
        bool _running;
        int _socket;
        int _port;
        std::string _password;
        std::map<std::string, ABotCommand*> _commandList;

    public:
        IrcBot(int port, const std::string password);
        ~IrcBot();

        bool connect();
        void run();
        void handleCommand(const std::string& message);
};
