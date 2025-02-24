#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sstream>
#include <unistd.h>

class IrcBot
{
    private:
        int _socket;
        int _port;
        std::string _password;
        bool _running;
    public:
        IrcBot(int port, const std::string password);
        ~IrcBot();

        bool connect();
        void run();
        void handleCommand(const std::string& message);
};
