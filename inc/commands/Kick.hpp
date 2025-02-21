#pragma once

#include "ACommand.hpp"

class Kick : public ACommand
{
    public:
        Kick();
        ~Kick();
        void execute(Client* client, std::string arguments);
};