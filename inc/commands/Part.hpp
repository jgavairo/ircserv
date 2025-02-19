#pragma once

#include "ACommand.hpp"

class Part : public ACommand
{
    public:
        Part();
        ~Part();
        void execute(Client* client, std::string arguments);
};