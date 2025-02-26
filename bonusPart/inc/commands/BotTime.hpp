#pragma once

#include "ACommand.hpp"

class Channel;

class BotTime : public ACommand
{
    public:
        BotTime();
        ~BotTime();
        void execute(Client* client, std::string arguments);
};