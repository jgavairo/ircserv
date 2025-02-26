#pragma once

#include "ACommand.hpp"

class Channel;

class BotHelp : public ACommand
{
    public:
        BotHelp();
        ~BotHelp();
        void execute(Client* client, std::string arguments);
};