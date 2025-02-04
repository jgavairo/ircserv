#pragma once

#include "ACommand.hpp"

class Channel;

class Join : public ACommand
{
    public:
        Join();
        ~Join();
        void execute(Client* client, std::string arguments);
};