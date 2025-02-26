#pragma once

#include "ACommand.hpp"

class Channel;

class Invite : public ACommand
{
    public:
        Invite();
        ~Invite();
        void execute(Client* client, std::string arguments);
};