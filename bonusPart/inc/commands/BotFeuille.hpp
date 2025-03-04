#pragma once

#include "ACommand.hpp"

class Channel;

class BotFeuille : public ACommand
{
    public:
        BotFeuille();
        ~BotFeuille();
        void execute(Client* client, std::string arguments);
        std::map<std::string, std::string> _output;
};