#pragma once

#include "ACommand.hpp"

class Channel;

class BotPunchline : public ACommand
{
    public:
        BotPunchline();
        ~BotPunchline();
        void execute(Client* client, std::string arguments);
        std::map<std::string, std::string> _punchlines;
};