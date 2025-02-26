#pragma once

#include "ACommand.hpp"

class User : public ACommand
{
    public:
        User();
        ~User();
        void execute(Client* client, std::string arguments);
};