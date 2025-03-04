#pragma once

#include "ACommand.hpp"

class Channel;

class BotCiseaux : public ACommand
{
    public:
        BotCiseaux();
        ~BotCiseaux();
        void execute(Client* client, std::string arguments);
        std::map<std::string, std::string> _output;
};