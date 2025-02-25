#pragma once

#include "ACommand.hpp"

class Channel;

class BotQuizz : public ACommand
{
    public:
        BotQuizz();
        ~BotQuizz();
        void execute(Client* client, std::string arguments);
};