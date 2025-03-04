#pragma once

#include "ACommand.hpp"

class Channel;

class BotPierre : public ACommand
{
    public:
        BotPierre();
        ~BotPierre();
        void execute(Client* client, std::string arguments);
        std::map<std::string, std::string> _output;
};